//======================================================================
//  Overlay.cpp - 
//
//  Dov Grobgeld <dov.grobgeld@gmail.com>
//  Sat May  4 22:04:53 2013
//----------------------------------------------------------------------

#include <QPainter>
#include <QPen>
#include "overlay.h"

class Overlay::Priv {
public:
  OverlayPainter *overlayPainter;
};

Overlay::Overlay(QWidget *parent)
  : QWidget(parent)
{
  setPalette(Qt::transparent);
  setAttribute(Qt::WA_TransparentForMouseEvents);
  d = new Priv;
  d->overlayPainter = NULL;
}

// Destructor
Overlay::~Overlay(void)
{
  delete d;
}

void Overlay::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);
  if (d->overlayPainter)
    d->overlayPainter->draw(&painter);
  else
  {
    // Some default drawing
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::red));
    painter.drawLine(width()/8, height()/8, 7*width()/8, 7*height()/8);
    painter.drawLine(width()/8, 7*height()/8, 7*width()/8, height()/8);
  }
}

void Overlay::setPainter(OverlayPainter *overlayPainter)
{
  d->overlayPainter = overlayPainter;
}
