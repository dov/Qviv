//======================================================================
//  Overlay.h - 
//
//  Dov Grobgeld <dov.grobgeld@gmail.com>
//  Sat May  4 22:03:41 2013
//----------------------------------------------------------------------
#ifndef QVIV_OVERLAY_H
#define QVIV_OVERLAY_H

#include <QWidget>

class QvivOverlayPainter {
  public:
    virtual ~QvivOverlayPainter() {}
    virtual void draw(QPainter *painter) = 0;
};

class QvivOverlay : public QWidget
{
public:
  QvivOverlay(QWidget *parent);
  ~QvivOverlay();
  void setPainter(QvivOverlayPainter *overlayPainter);

protected:
  void paintEvent(QPaintEvent *event);

private:
  class Priv;
  Priv *d;
};

#endif /* OVERLAY */
