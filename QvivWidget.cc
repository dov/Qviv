//======================================================================
//  QvivWidget.cpp - A widget like GivWidget.
//
//  Dov Grobgeld <dov.grobgeld@gmail.com>
//  Tue May 10 12:00:19 2011
//----------------------------------------------------------------------

#include <QPen>
#include <QPainter>
#include <QPaintEvent>
#include <QLabel>
#include <QEventLoop>
#include <stdio.h>
#include "QvivWidget.h"
#include "QvivData.h"
#include "QvivRenderer.h"
#include "QvivPainterAgg.h"

class QvivWidget::Priv
{
public:
    QLabel* w_balloon;
    QImage label_image;
    QvivData *qviv_data;
    bool do_no_transparency;
    bool do_show_balloon;
    bool do_show_marks;
    bool do_pick_point;
    QEventLoop *event_loop;
    QPointF picked_point;
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
    d->do_pick_point = false;
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

    if (!d->do_show_marks)
        return;
    QvivPainterAgg qviv_painter(image,true);
    QvivRenderer renderer(d->qviv_data, qviv_painter,
                          scale_x, scale_y,
                          shift_x, shift_y,
                          image->width(), image->height());
    renderer.set_do_no_transparency(d->do_no_transparency);
    renderer.paint();

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

void QvivWidget::mouseMoveEvent (QMouseEvent *event)
{

    // Call parent event
    QvivImageViewer::mouseMoveEvent(event);

    if (!d->do_show_balloon)
        return;

    unsigned int label_color = d->label_image.pixel(event->x(),event->y());
    
    // TBD - Move this a common place
    unsigned int label = (((label_color >> 16)&0xff)
                          +(((label_color >> 8)&0xff)<<8)
                          +((label_color & 0xff) << 16));

    if (label_color < 0xff000000 && label > 0)
    {
        char *balloon_text = d->qviv_data->balloons.get_balloon_text(label-1);
        if (balloon_text)
        {
          d->w_balloon->setText(balloon_text);
          d->w_balloon->adjustSize();
          d->w_balloon->move(window()->geometry().x()+x()+event->x()+5,
                             window()->geometry().y()+y()+event->y()-d->w_balloon->geometry().height()-5);
          d->w_balloon->show();
          free(balloon_text);
        }
    }
    else
      d->w_balloon->hide();
}

void QvivWidget::leaveEvent(QEvent *event)
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

    // Change the cursor. Should be configurable
    setCursor(Qt::CrossCursor);

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
