//======================================================================
//  QvivWidget.cpp - A widget like GivWidget.
//
//  Dov Grobgeld <dov.grobgeld@gmail.com>
//  Tue May 10 12:00:19 2011
//----------------------------------------------------------------------

#include <QPen>
#include <QBitmap>
#include <QPainter>
#include <QPaintEvent>
#include <QLabel>
#include <QEventLoop>
#include <stdio.h>
#include "QvivWidget.h"
#include "QvivData.h"
#include "QvivRenderer.h"
#include "QvivPainterAgg.h"
#include "QRubberBand"

class QvivWidget::Priv
{
public:
    QLabel* w_balloon;
    QImage label_image;
    QvivData *qviv_data;
    QvivData *qviv_measure_data;
    bool do_no_transparency;
    bool do_show_balloon;
    bool do_show_marks;
    bool do_pick_point;
    bool do_draw_rubber_band;
    QPointF rubber_band_start;
    QEventLoop *event_loop;
    QPointF picked_point;
    QRubberBand *rubber_band;
};

QvivWidget::QvivWidget(QWidget *parent,
                       QImage image)
    : QvivImageViewer(parent,
                      image)
{
    d = new Priv;
    d->w_balloon = new QLabel(NULL,
                              Qt::FramelessWindowHint
                              |Qt::X11BypassWindowManagerHint
                              |Qt::ToolTip);
    d->w_balloon->setStyleSheet("QLabel { background-color : yellow; color : black; }");
    d->do_no_transparency = false;
    d->do_show_balloon = false;
    d->do_show_marks = true;
    d->qviv_data = NULL;
    d->qviv_measure_data = NULL;
    d->do_pick_point = false;
    d->do_draw_rubber_band = false;
    d->rubber_band = NULL;
}

QvivWidget::~QvivWidget()
{
    delete d;
}

void QvivWidget::set_qviv_data(QvivData *qviv_data)
{
  d->qviv_data = qviv_data;
}

void QvivWidget::imageAnnotate(QImage *image, 
                               int shift_x, int shift_y,
                               double scale_x, double scale_y)
{
    if (get_mouse_scrolling())
        return;

    QvivPainterAgg qviv_painter(image,true);
    if (d->do_show_marks)
    {
      QvivRenderer renderer(d->qviv_data, qviv_painter,
                            scale_x, scale_y,
                            shift_x, shift_y,
                            image->width(), image->height());
      renderer.set_do_no_transparency(d->do_no_transparency);
      renderer.paint();
    }

    if (d->qviv_measure_data)
    {
        QvivRenderer renderer(d->qviv_measure_data, qviv_painter,
                              scale_x, scale_y,
                              shift_x, shift_y,
                              image->width(), image->height());
        renderer.set_do_no_transparency(d->do_no_transparency);
        renderer.paint();
        
    }
    
    // Create the label image necessary for the balloon popup. The
    // balloon image is always the size of the total displayed area.
    // It would be faster to store and scroll the label image. But
    // we currently don't have any way of doing that. 
    if (d->do_show_balloon) {
        int li_shift_x, li_shift_y;
        double li_scale_x, li_scale_y;

        d->label_image = QImage(width(), height(), QImage::Format_ARGB32);
        d->label_image.fill(0);

        // Get scroll and shift for the current image
        get_scale_and_shift(li_scale_x, li_scale_y,
                            li_shift_x, li_shift_y);

        QvivPainterAgg painter(&d->label_image, false);

        painter.set_do_paint_by_index(true);
        
        QvivRenderer renderer(d->qviv_data, painter,
                              li_scale_x, li_scale_y,
                              li_shift_x, li_shift_y,
                              width(), height());
        renderer.set_do_no_transparency(d->do_no_transparency);
        renderer.paint();
    }
}

void QvivWidget::mousePressEvent (QMouseEvent * event)
{
    // Allow control zooming by not catching the event when control is pressed
    if (d->do_pick_point
        && (event->modifiers() & Qt::ControlModifier)==0
        && event->button() == 1)
    {
        // We got an acceptable picked point. Convert it to
        // image coordinates and return.
        double imgx, imgy;
        canv_coord_to_img_coord(event->x(), event->y(),
                                // output
                                imgx, imgy);
        d->picked_point = QPointF(imgx,imgy);
        d->event_loop->exit(0);
        return;
    }
    QvivImageViewer::mousePressEvent(event);
}

void QvivWidget::abort_pick_point()
{
  if (d->do_pick_point && d->event_loop)
  {
    d->do_pick_point = false;
    d->event_loop->exit(-1);
  }
}

void QvivWidget::mouseMoveEvent (QMouseEvent *event)
{
    // Call parent event
    QvivImageViewer::mouseMoveEvent(event);

    if (d->do_show_balloon)
    {
      unsigned int label_color = d->label_image.pixel(event->x(),event->y());
      
      // TBD - Move this a common place
      unsigned int label = (((label_color >> 16)&0xff)
                            +(((label_color >> 8)&0xff)<<8)
                            +((label_color & 0xff) << 16));
  
      if (label_color >= 0xff000000 && label > 0)
      {
          char *balloon_text = d->qviv_data->balloons.get_balloon_text(label-1);
  
          if (balloon_text)
          {
            d->w_balloon->setText(balloon_text);
            d->w_balloon->adjustSize();
            QPoint p = mapToGlobal(QPoint(event->x(), event->y()));
  #if 0
            d->w_balloon->move(window()->geometry().x()+x()+event->x()+5,
                               window()->geometry().y()+y()+event->y()-d->w_balloon->geometry().height()-5);
  #endif
            d->w_balloon->move(p.x()+5,
                               p.y()-d->w_balloon->geometry().height()-5);
            d->w_balloon->show();
            free(balloon_text);
          }
      }
      else
        d->w_balloon->hide();
    }

    if (d->do_draw_rubber_band)
    {
        double imgx, imgy;

        // Create a rubber band from first to rast point
        canv_coord_to_img_coord(event->x(), event->y(),
                                // output
                                imgx,imgy);

#if 1
        d->qviv_measure_data->clear();
        QvivDataSet ds;
        ds.add_point(OP_MOVE,
                     d->rubber_band_start.x(),
                     d->rubber_band_start.y());
        ds.add_point(OP_DRAW, imgx, imgy);
        d->qviv_measure_data->data_sets.push_back(ds);

        // This should be made more efficient!
        redraw();
#else
        double x0,y0;
        img_coord_to_canv_coord(d->rubber_band_start.x(),
                                d->rubber_band_start.y(),
                                // output
                                x0,y0);
        d->rubber_band->setGeometry(x0,y0,
                                    event->x()-x0, event->y()-y0);
#endif
    }
}

void QvivWidget::leaveEvent(QEvent */*event*/)
{
    d->w_balloon->hide();
}

void QvivWidget::keyPressEvent (QKeyEvent * event)
{
    // Abort picking
    if (d->do_pick_point && event->key() == Qt::Key_Escape)
        d->event_loop->exit(-1);

    QString k = event->text();

    // Toggle balloons
    if (k=="b")
    {
        d->do_show_balloon = !d->do_show_balloon;
        if (d->do_show_balloon)
            redraw();
        else
            d->w_balloon->hide();
        emit qvivBalloonChanged(d->do_show_balloon);
    }
    // Toogle anti-aliasing
    else if (k=="a")
    {
        d->do_no_transparency= !d->do_no_transparency;
        redraw();
    }
    // Toggle the display of overlay
    else if (k=="m")
    {
        d->do_show_marks= !d->do_show_marks;
        emit qvivOverlayChanged(d->do_show_marks);

        redraw();
    }

    QvivImageViewer::keyPressEvent(event);
}

// Request the user to pick a point from the widget. This is
// implemented by entering a second eventloop.

int QvivWidget::pick_point(// output
                           QPointF& picked_point
                           )
{
    d->do_pick_point = true;

    // Create a cross cursor. Must be divisible by 2 on Windows
    const int Size=32;
    const int S2 = Size/2;
    QBitmap Cursor(Size,Size);
    QBitmap Mask(Size,Size);

    // This is convoluted and could be cleaned up.
    QPainter CursorPainter(&Cursor);
    QPainter MaskPainter(&Mask);
    for (int Row=0; Row<Size; Row++)
      for (int Col=0; Col<Size; Col++)
      {
        QColor B(Qt::color0);
        QColor M(Qt::color0);
        if ((Col>0 && (Row >= S2-1 && Row <= S2+1))
            || (Row>0 && (Col >= S2-1 && Col <= S2+1)))
          M = Qt::color1;
        if ((Col>0&&Row == S2) || (Row>0&&Col == S2))
          B = Qt::color1;
        CursorPainter.setPen(B);
        CursorPainter.drawPoint(QPoint(Col,Row));
        MaskPainter.setPen(M);
        MaskPainter.drawPoint(QPoint(Col,Row));
      }

    // Change the cursor. Should be configurable
    setCursor(QCursor(Cursor,Mask,S2,S2));

    // Enter secondary loop and wait for reply
    QEventLoop event_loop;
    d->event_loop = &event_loop;
    int ret = event_loop.exec();

    // Restore cursor to default
    unsetCursor();

    d->event_loop=NULL;
    d->do_pick_point = false;

    // If aborted return -1
    if (ret != 0)
        return -1;

    // Return the picked point coordinate
    picked_point = d->picked_point;

    return 0;
}

// Given a point start_point, draw a rubber band to another point.
int QvivWidget::rubber_band_pick(QPointF start_point,
                                 // output
                                 QPointF& picked_point)
{
    d->rubber_band_start = start_point;
    d->do_draw_rubber_band = true;
    d->qviv_measure_data = new QvivData;
    d->rubber_band = new QRubberBand(QRubberBand::Rectangle, this);
    d->rubber_band->setGeometry(start_point.x(), start_point.y(),
                                0,0);
    d->rubber_band->show();
        

    // Meanwhile ignore the start point
    int ret =  pick_point(picked_point);
    d->do_draw_rubber_band = false;
    delete d->qviv_measure_data;
    d->qviv_measure_data = NULL;
    delete d->rubber_band;
    d->rubber_band = NULL;

    redraw();

    return ret;
}


void QvivWidget::set_view_overlay(bool do_view_overlay)
{
  d->do_show_marks = do_view_overlay;
  redraw();
}

void QvivWidget::set_view_balloon(bool do_view_balloon)
{
  d->do_show_balloon = do_view_balloon;
  if (d->do_show_balloon)
    redraw();
  else
    d->w_balloon->hide();
}


