#include <QWidget>
#include "QvivLasso.h"
#include "QvivOverlay.h"


class MyOverlayPainter : public QvivOverlayPainter
{
  public:
    MyOverlayPainter(QvivLasso *lasso);

    virtual void draw(QPainter *painter);
  
  private:
    QvivLasso *lasso;
};

// Constructor
MyOverlayPainter::MyOverlayPainter(QvivLasso *_lasso)
{
  lasso = _lasso;
}

void MyOverlayPainter::draw(QPainter *painter)
{
    lasso->draw(painter);
}

class QvivLasso::Priv
{
  public:
    QvivLassoDrawing* lassoDrawing;
    MyOverlayPainter* overlayPainter;
    QWidget *widget;
    QvivOverlay *overlay;
};


// Constructor
QvivLasso::QvivLasso(QWidget *widget)
{
  d = new Priv;
  d->widget = widget;
  d->overlay = new QvivOverlay(widget);
  d->overlayPainter = new MyOverlayPainter(this);
  d->overlay->setPainter(d->overlayPainter);
}

// Destructor
QvivLasso::~QvivLasso()
{
  delete d;
}

QColor QvivLasso::getColorForLabel(int label)
{
  int bb = label & 0xff;
  int gg = (label>>8) & 0xff;
  int rr = (label>>16) & 0xff;

  return QColor(rr,gg,bb);
}

void QvivLasso::setLassoDrawing(QvivLassoDrawing *lassoDrawing)
{
  d->lassoDrawing = lassoDrawing;
}

void QvivLasso::update(void)
{
  // TBD - generate a list of drawing rectangles. For now update
  // the entire widget that is being handled.
  d->widget->update();

}
void QvivLasso::resize(const QSize& size)
{
  d->overlay->resize(size);
}

QWidget *QvivLasso::widget(void)
{
  return d->widget;
}

void QvivLasso::draw(QPainter *painter)
{
  int width = d->widget->width();
  int height = d->widget->height();

  if (d->lassoDrawing)
    d->lassoDrawing->draw(painter, QVIV_LASSO_CONTEXT_PAINT);
  else
  {
    // Some default drawing
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(Qt::green));
    painter->drawLine(width/8, height/8, 7*width/8, 7*height/8);
    painter->drawLine(width/8, 7*height/8, 7*width/8, height/8);
  }
}

// This is slow as the label image is recalculated every time.
// Not clear how it can be made faster though.
int QvivLasso::getLabelForPixel(int colIdx, int rowIdx)
{
  QImage labelImage(d->widget->width(), d->widget->height(), QImage::Format_ARGB32);
  QPainter painter(&labelImage);
  painter.fillRect(0,0,labelImage.width(),labelImage.height(),
                   QColor(0,0,0,0));
  d->lassoDrawing->draw(&painter, QVIV_LASSO_CONTEXT_LABEL);
  QRgb pixel = labelImage.pixel(colIdx, rowIdx);
  if (pixel &0xff000000 == 0)
    return -1;
  return int(pixel&0xffffff);
}
