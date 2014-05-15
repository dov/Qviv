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
#include <QApplication>
#include <QClipboard>
#include <QTextDocument>
#include <stdio.h>
#include <math.h>
#include "QvivWidget.h"
#include "QvivData.h"
#include "QvivRenderer.h"
#include "QvivPainterAgg.h"
#include "QvivLasso.h"
#include <QRegExp>

static const double RAD2DEG = 180 / 3.1415926535;

// Draw a caliper. This should be made configurable.
static void drawCaliper(QPainter *painter,
                        QvivLassoContext context,
                        double x0, double y0,
                        double x1, double y1,
                        const QString& distString)
{
    int margin = 0;
    painter->resetTransform();
    if (context == QVIV_LASSO_CONTEXT_PAINT)
        painter->setRenderHint(QPainter::Antialiasing, true);
    else
    {
        margin = 5;
        painter->setRenderHint(QPainter::Antialiasing, false);
    }

    double angle = atan2(y1-y0,x1-x0);
    QTransform t = painter->deviceTransform();
    painter->translate(-t.dx(), -t.dy());
    painter->translate(0.5 * (x0+x1),
                       0.5 * (y0+y1));
    painter->rotate(angle * RAD2DEG);
    double dist = sqrt((x1-x0)*(x1-x0) + (y1-y0)*(y1-y0));

    // Middle stroke
    QColor color;
    QPen pen;
    if (context == QVIV_LASSO_CONTEXT_PAINT)
        color = QColor(255,0xaa,0,128);
    else if (context == QVIV_LASSO_CONTEXT_LABEL)
    {
        color = QvivLasso::getColorForLabel(1);
        pen.setWidth(5);
    }
    else
    {
        color = Qt::black;
        pen.setWidth(5);
    }

    painter->fillRect(-dist/2-margin, -20-margin,
                      dist+2*margin, 20+2*margin, color);

    // Side left
    if (context == QVIV_LASSO_CONTEXT_PAINT)
        color = QColor(0x50,0x2d,0x16,128);
    else if (context == QVIV_LASSO_CONTEXT_LABEL)
        color = QvivLasso::getColorForLabel(2);

    painter->setBrush(QBrush(color));
    painter->setPen(pen);

    double calip_height = 50;
    QPainterPath path;
    double x = -dist/2+margin;
    double y = calip_height/2+margin;
    path.moveTo(x,y);
    double dy = -(calip_height+3*margin)/3;
    path.cubicTo(x-15-2*margin,y+dy,
                 x-15-2*margin,y+2.5*dy,
                 x-15-2*margin,y+3*dy);
    x = -dist/2+margin;
    y = -calip_height/2-margin;
    path.lineTo(x,y);
    path.closeSubpath();
    if (context != QVIV_LASSO_CONTEXT_PAINT)
        painter->strokePath(path, pen);
    painter->drawPath(path);

    if (context == QVIV_LASSO_CONTEXT_LABEL)
    {
        color = QvivLasso::getColorForLabel(3);
        pen.setColor(color); 
    }  
  
    path = QPainterPath();  // Empty the path
    painter->setBrush(QBrush(color));
    painter->setPen(pen);

    x = dist/2-margin;
    y = calip_height/2+margin;
    path.moveTo(x,y); 
    path.cubicTo(x + 15+2*margin,y + dy,
                 x + 15+2*margin,y + 2.5*dy,
                 x + 15+2*margin,y + 3*dy);
    path.lineTo(dist/2-margin,-calip_height/2-margin); 
    path.closeSubpath();

    if (context != QVIV_LASSO_CONTEXT_PAINT)
        painter->strokePath(path, pen);
    painter->drawPath(path);

    if (context == QVIV_LASSO_CONTEXT_PAINT)
    {
        // Draw label within the caliper
        QFont Font("Sans",14);
        painter->setFont(Font);
        painter->setPen(QColor(0,0,0,255));
        QFontMetrics fm(Font);
        QRect rect = fm.tightBoundingRect(distString);
        rect.moveTop(-10-rect.height()/2);
        rect.moveLeft(-rect.width()/2);
        painter->drawText(rect, Qt::AlignCenter, distString);
    }
}

class MyLassoDrawing : public QvivLassoDrawing {
public:
    MyLassoDrawing(QvivWidget *widget)
    {
        this->x0=this->y0=-HUGE;
        this->x1=this->y1=-HUGE;
        this->moving = false;
        this->widget = widget;
        this->unit = "pix";
        this->scale = 1.0;
    }
    void draw(QPainter *painter,
              QvivLassoContext Context)
    {
        if (!widget->is_measuring())
            return;

        // Convert distance to image distance
        double img_x0, img_y0, img_x1, img_y1;
        widget->canv_coord_to_img_coord(x0,y0,
                                        // output
                                        img_x0, img_y0);
        widget->canv_coord_to_img_coord(x1,y1,
                                        // output
                                        img_x1, img_y1);
        double dist = sqrt((img_x1-img_x0)*(img_x1-img_x0)
                           + (img_y1-img_y0)*(img_y1-img_y0));

        QString DistString = QString("%1%2").arg(dist,0,'f',1).arg(unit);
        drawCaliper(painter,Context,x0,y0,x1,y1, DistString);
    }        
    void setXY0(double x, double y)
    {
        this->x0 = x;
        this->y0 = y;
    }
    void setXY1(double x, double y)
    {
        this->x1 = x;
        this->y1 = y;
    }
    void incXY0(double dx, double dy)
    {
        this->x0 += dx;
        this->y0 += dy;
    }
    void incXY1(double dx, double dy)
    {
        this->x1 += dx;
        this->y1 += dy;
    }
    void setMoving(bool moving)
    {
        this->moving = moving;
    }
    void setUnitAndScale(const QString& unit, double scale)
    {
        this->unit = unit;
        this->scale = scale;
    }

    double x0, y0;
    double x1, y1;
    bool moving;
    QvivWidget *widget;
    QString unit;
    double scale;
};

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
    bool do_measure;
    int picking; // Used in measuring
    QPointF rubber_band_start;
    QEventLoop *event_loop;
    QPointF picked_point;
    QvivLasso *lasso;
    MyLassoDrawing *lassoDrawing;
    double mx0,my0,mx1,my1; // Measuring in image coordinates
    int last_x, last_y;     // Last measuring coord in canvas coordinates
    double last_shift_x, last_shift_y, last_scale_x, last_scale_y;
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
    d->do_measure = false;
    d->lasso = new QvivLasso(this);
    d->lassoDrawing = new MyLassoDrawing(this);
    d->lasso->setLassoDrawing(d->lassoDrawing);
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
    // The following is ugly and should be replaced with a proper
    // signal whenever the scaling is changed!
    if (d->do_measure
        && (shift_x != d->last_shift_x
            || shift_y != d->last_shift_y
            || scale_x != d->last_scale_x
            || scale_y != d->last_scale_y))
    {
        double x0, y0, x1, y1;
        img_coord_to_canv_coord(d->mx0,d->my0,
                                // output
                                x0,y0);
        img_coord_to_canv_coord(d->mx1,d->my1,
                                // output
                                x1,y1);
        d->lassoDrawing->setXY0(x0,y0);
        d->lassoDrawing->setXY1(x1,y1);
    }

    if (get_mouse_scrolling())
        return;

    d->last_shift_x = shift_x;
    d->last_shift_y = shift_y;
    d->last_scale_x = scale_x;
    d->last_scale_y = scale_y;

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
    if (d->do_show_balloon)
    {
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

void QvivWidget::resizeEvent(QResizeEvent *event)
{
  d->lasso->resize(event->size());
  event->accept();
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
    if (d->do_measure
        && (event->modifiers() & Qt::ControlModifier)==0
        && event->button() == 1)
    {
        int x=event->x();
        int y=event->y();
        d->last_x = x;
        d->last_y = y;
        double imgx,imgy;
        canv_coord_to_img_coord(x,y,
                                // output
                                imgx,imgy);
        
        int label = d->lasso->getLabelForPixel(x,y);
        
        d->picking = label;
        
        if (d->picking == 0)
        {
            d->lassoDrawing->setXY0(x,y);
            d->lassoDrawing->setXY1(x,y);
            d->picking = 3;
        }
        d->lassoDrawing->setMoving(true);
        d->mx0 = d->mx1 = imgx;
        d->my0 = d->my1 = imgy;
        d->lasso->update();
        redraw();
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

void QvivWidget::mouseReleaseEvent (QMouseEvent * event)
{
  d->lassoDrawing->setMoving(false);
  QvivImageViewer::mouseReleaseEvent(event);
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
              // Strip trailing whitespace.
              QString s(QString::fromUtf8(balloon_text));
              s.remove(QRegExp("\\s+$"));
        

              // Keep newlines
              d->w_balloon->setText(s);
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

    if (d->do_measure && d->lassoDrawing->moving)
    {
        double x = event->x();
        double y = event->y();
        double dx = x - d->last_x;
        double dy = y - d->last_y;
        d->last_x = x;
        d->last_y = y;
        
        if (d->picking == 1 || d->picking == 2)
            d->lassoDrawing->incXY0(dx,dy);
        if (d->picking == 1 || d->picking == 3)
            d->lassoDrawing->incXY1(dx,dy);
        
        // Save as image coordinates
        canv_coord_to_img_coord(d->lassoDrawing->x0,
                                d->lassoDrawing->y0,
                                // output
                                d->mx0,d->my0);
        canv_coord_to_img_coord(d->lassoDrawing->x1,
                                d->lassoDrawing->y1,
                                // output
                                d->mx1,d->my1);
        
        d->lasso->update();
        redraw();
        return;
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
    // Control-c copies the contents of a balloon
    else if (event->matches(QKeySequence::Copy))
    {
        if (d->do_show_balloon && d->w_balloon->isVisible())
        {
            // Convert to plain text via qtextdocument. This is kind of hackish, but it
            // should be fast enough.
            QTextDocument doc;
            doc.setHtml( d->w_balloon->text() );
            QApplication::clipboard()->setText(doc.toPlainText());
        }
    }    
    else if (k=="z")
    {
        d->do_measure = !d->do_measure;
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


bool QvivWidget::is_measuring(void)
{
  return d->do_measure;
}

