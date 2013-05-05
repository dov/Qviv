//======================================================================
//  Overlay.h - 
//
//  Dov Grobgeld <dov.grobgeld@gmail.com>
//  Sat May  4 22:03:41 2013
//----------------------------------------------------------------------
#ifndef OVERLAY_H
#define OVERLAY_H

#include <QWidget>

class OverlayPainter {
  public:
    virtual ~OverlayPainter() {}
    virtual void draw(QPainter *painter) = 0;
};

class Overlay : public QWidget
{
public:
  Overlay(QWidget *parent);
  ~Overlay();
  void setPainter(OverlayPainter *overlayPainter);

protected:
  void paintEvent(QPaintEvent *event);

private:
  class Priv;
  Priv *d;
};

#endif /* OVERLAY */
