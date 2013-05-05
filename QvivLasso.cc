#include <QWidget>
#include "QvivLasso.h"
#include "QvivOverlay.h"
#include <stdio.h>


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
    printf("overlaypainter: draw...\n");
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
  printf("lasso update...\n");
  d->widget->update();

}
void QvivLasso::resize(const QSize& size)
{
  printf("lasso (and overlay) resize to %d,%d\n", size.width(), size.height());
  d->overlay->resize(size);
}

QWidget *QvivLasso::widget(void)
{
  return d->widget;
}

void QvivLasso::draw(QPainter *painter)
{
  if (d->lassoDrawing)
    d->lassoDrawing->draw(painter, QVIV_LASSO_CONTEXT_PAINT);
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
  return labelImage.pixel(colIdx, rowIdx) & 0xffffff;
}
