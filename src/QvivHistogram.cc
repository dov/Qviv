//======================================================================
//  QvivHistogram.h - Like GivContrast
//
//  Dov Grobgeld <dov.grobgeld@gmail.com>
//  Mon May 28 21:44:34 2012
//----------------------------------------------------------------------

#include <QPainter>
#include <QPaintEvent>
#include <QPainterPath>
#include "QvivHistogram.h"
#include <QPainterPath>
#include <vector>

using namespace std;

class QvivHistogram::Priv 
{
public:
    QWidget *parent;
    QImage *image;
    int cursorGrayLevel;
    int histoMax;
    int margin;
    double scale;

    // Currently only support gray level histogram
    vector<int> histogram;
    double glToCanvasX(double gl);
    double canvasXtoGL(double canvasX);
};

QvivHistogram::QvivHistogram(QWidget *parent,
                             QImage *image)
  : QWidget(parent)
{
    d = new Priv;
    d->parent = this;
    d->histogram.resize(256);
    d->margin = 20;
    d->cursorGrayLevel = 128;
    d->scale= 1.0;
    setImage(image);

    setWindowTitle("Histogram");
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    setMouseTracking(true);
}

void QvivHistogram::setImage(QImage *image, bool do_reset_scale)
{
    // Create histogram
    d->image = image;
    d->histogram.clear();
    d->histogram.resize(256,0);
    d->histoMax = 0;

    if (do_reset_scale)
      d->scale = 1.0;
    if (!image)
        return;

    int width = image->width();
    int height = image->height();
    int bpp = image->depth()/8;

    // Assume 8-bit gray and only create histogram of first pixel
    for (int row_idx=0; row_idx < height; row_idx++) {
        const uchar *p = image->scanLine ( row_idx );
        for (int col_idx=0; col_idx < width; col_idx++) {
            int gl = *p;
            d->histogram[gl]++;
            p+= bpp;
        }
    }

    // Find the maximum of the histogram
    for (int i=0; i<256; i++)
        if (d->histogram[i] > d->histoMax)
            d->histoMax = d->histogram[i];
    update();
}

void QvivHistogram::setCursorGrayLevel(int grayLevel)
{
    d->cursorGrayLevel = grayLevel;
    update();
}

QvivHistogram::~QvivHistogram(void)
{
    delete d;
}

// See: http://doc.trolltech.com/4.5/painting-basicdrawing.html
// for more examples!
void QvivHistogram::paintEvent(QPaintEvent *evt)
{
    QPainter painter(this);
    painter.setClipRegion(evt->region());
    int height = this->height();
    int maxHeight = height - 2.5 * d->margin;

    QColor color(100,100,128);
    for (int i=0; i<256; i++) {
        int gl = i;
        double x0 = d->glToCanvasX(gl);
        double x1 = d->glToCanvasX(gl+1);
        double binHeight = maxHeight * d->histogram[gl] * d->scale / d->histoMax;
        // Clip
        if (binHeight > maxHeight)
            binHeight = maxHeight;
        QRectF rectangle(x0,
                         d->margin + maxHeight - binHeight,
                         x1-x0,
                         binHeight);
        painter.fillRect(rectangle,color);
    }

    // Draw the cursor
    color = QColor(128,0,0,255);
    double xCenter = d->glToCanvasX(d->cursorGrayLevel);
    double xLeft = xCenter - 7;
    double xRight = xCenter + 7;
    double yTop = d->margin+maxHeight;
    double yBottom = yTop + 7;
    QPainterPath path(QPointF(xCenter,yTop));
    path.lineTo(QPointF(xLeft,yBottom));
    path.lineTo(QPointF(xRight,yBottom));
    path.closeSubpath();
    painter.fillPath(path,color);

    // Draw text below cursor
    
    painter.setPen(color); 
    painter.drawText(QRectF(xCenter,yBottom+3,0,0),
                     Qt::TextDontClip | Qt::AlignTop | Qt::AlignHCenter,
                     QString("%1").arg(int(d->cursorGrayLevel)));
}

double QvivHistogram::Priv::glToCanvasX(double gl)
{
    int width = parent->width();
    double histoWidth = width - 2*margin;

    // The histogram is currently fixed between 0 and 256
    double min = 0;
    double max = 256;
    
    return margin+1.0*histoWidth/(max-min)*(gl-min);
}

double QvivHistogram::Priv::canvasXtoGL(double canvasX)
{
    int width = parent->width();
    double histoWidth = width - 2*margin;

    // The histogram is currently fixed between 0 and 256
    double min = 0;
    double max = 256;

    return min+(canvasX-margin) * (max-min)/histoWidth;
}

void QvivHistogram::mouseMoveEvent (QMouseEvent *event)
{
    int x = event->x();

    double GL = d->canvasXtoGL(x);
    if (GL < 0)
        GL = 0;
    else if (GL > 255)
        GL = 255;
    d->cursorGrayLevel = GL;
    
    update();
}

void QvivHistogram::wheelEvent (QWheelEvent *event)
{
    int delta=event->delta();

    if (delta < 0)
        d->scale /= 1.1;
    else
        d->scale *= 1.1;
    update();
}
