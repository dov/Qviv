//======================================================================
//  QvivWidget.cpp - A widget like GivWidget.
//
//  Dov Grobgeld <dov.grobgeld@gmail.com>
//  Tue May 10 12:00:19 2011
//----------------------------------------------------------------------

#include <QvivWidget.h>
#include <QPen>
#include <QPainter>
#include <QPaintEvent>

QSize QvivWidget::sizeHint() const
{
    return QSize(400, 200);
}

class QvivWidget::Priv
{
    double foo;
};

QvivWidget::QvivWidget(QWidget *parent)
    : QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    d = new Priv;
}

QvivWidget::~QvivWidget()
{
    delete d;
}

// See: http://doc.trolltech.com/4.5/painting-basicdrawing.html
// for more examples!
void QvivWidget::paintEvent(QPaintEvent *evt)
{
    QPainter painter(this);
    QImage image("maja.pgm");
    painter.setClipRegion(evt->region());
    double wscale = this->size().width()/100.0;
    double hscale = this->size().height()/100.0;
    painter.drawImage(QRect(wscale*25,
                            hscale*25,
                            wscale*50,
                            hscale*50),
                      image);
                                   

    // A list of points
    QPoint points[4] = {
         QPoint(10*wscale, 80*hscale),
         QPoint(20*wscale, 10*hscale),
         QPoint(80*wscale, 30*hscale),
         QPoint(90*wscale, 70*hscale)
     };
     painter.setRenderHint(QPainter::Antialiasing, true);

     // Use rgb color with alpha
     QPen pen(QColor(0,128,0,128), 3);
     
     painter.setPen(pen);
     painter.drawPolyline(points, 4);
}

