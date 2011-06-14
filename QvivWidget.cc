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
#include <stdio.h>
#include "QvivWidget.h"
#include "QvivData.h"

class QvivWidget::Priv
{
public:
    QLabel* w_balloon;
    QvivData qviv_data;
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
}

QvivWidget::~QvivWidget()
{
    delete d;
}

void QvivWidget::set_qviv_data(QvivData& qviv_data)
{
  d->qviv_data = qviv_data;
}

#if 0
void QvivWidget::imageAnnotate(QImage *image, 
                               int shift_x, int shift_y,
                               double scale_x, double scale_y)
{
    struct {
        double x,y;
    } points[] =  { { 186.5, 176.5 },
                    {186, 176},
                    {186, 177},
                    {187, 177},
                    {187, 176}};

    QPainter painter(image);
    QPen pen(QColor(255,0,0,255), 5);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(pen);

    for (int i=0; i<5; i++) {
        double x = points[i].x*scale_x -shift_x;
        double y = points[i].y*scale_y-shift_y;

        painter.drawEllipse(x-5,y-5,10,10);
    }
    //    painter.drawEllipse(-15,-15,30,30);
}
#endif

void QvivWidget::imageAnnotate(QImage *image, 
                               int shift_x, int shift_y,
                               double scale_x, double scale_y)
{
    QPainter painter(image);
    painter.setRenderHint(QPainter::Antialiasing, true);

    for (int ds_idx=0; ds_idx<(int)d->qviv_data.data_sets.size(); ds_idx++) {
        QvivDataSet& ds = d->qviv_data.data_sets[ds_idx];
        QPen pen(ds.color, 0);   // 0 is the stroke width
        painter.setPen(Qt::NoPen);  // Don't stroke
        painter.setBrush(ds.color); // this is the fill color

        for (int i=0; i<(int)ds.points.size(); i++) {
            QvivPoint& pt=ds.points[i];
            double x = pt.x*scale_x -shift_x;
            double y = pt.y*scale_y-shift_y;
            
            painter.drawEllipse(x-5,y-5,10,10);
        }
    }
}

void QvivWidget::mouseMoveEvent (QMouseEvent *event)
{
    static char label_text[100];

    // Call parent event
    QvivImageViewer::mouseMoveEvent(event);

    int cnv_x = event->x();
    int cnv_y = event->y();
    double img_x, img_y;
    canv_coord_to_img_coord(cnv_x,cnv_y,
                            // output
                            img_x,img_y);

    sprintf(label_text, "This is a\nmultiline\n(%.3f, %.3f)\nlabel",
            img_x, img_y);

    d->w_balloon->setText(label_text);
    d->w_balloon->adjustSize();
    d->w_balloon->move(window()->geometry().x()+event->x()+5,
                       window()->geometry().y()+event->y()-d->w_balloon->geometry().height()-5);
    d->w_balloon->show();
}

void QvivWidget::leaveEvent(QEvent *event)
{
    d->w_balloon->hide();
}

