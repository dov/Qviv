//======================================================================
//  Overlay.cpp - 
//
//  Dov Grobgeld <dov.grobgeld@gmail.com>
//  Sat May  4 22:04:53 2013
//----------------------------------------------------------------------

#include <QPainter>
#include <QPen>
#include "QvivOverlay.h"

class QvivOverlay::Priv {
public:
  QvivOverlayPainter *overlayPainter;
};

QvivOverlay::QvivOverlay(QWidget *parent)
  : QWidget(parent)
{
  setPalette(Qt::transparent);
  setAttribute(Qt::WA_TransparentForMouseEvents);
  d = new Priv;
  d->overlayPainter = NULL;
}

// Destructor
QvivOverlay::~QvivOverlay(void)
{
  delete d;
}

void QvivOverlay::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);
  if (d->overlayPainter)
    d->overlayPainter->draw(&painter);
}

void QvivOverlay::setPainter(QvivOverlayPainter *overlayPainter)
{
  d->overlayPainter = overlayPainter;
}
