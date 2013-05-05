//======================================================================
//  lasso.h - A lasso for qt.
//
//  Dov Grobgeld <dov.grobgeld@gmail.com>
//  Sat May  4 22:20:46 2013
//----------------------------------------------------------------------
#ifndef LASSO_H
#define LASSO_H

#include <QPainter>
#include <QColor>

class QWidget;

typedef enum {
  DOVTK_LASSO_CONTEXT_PAINT,
  DOVTK_LASSO_CONTEXT_MASK,
  DOVTK_LASSO_CONTEXT_LABEL
} QvivLassoContext;


// The interface used for the drawing callback widget
class QvivLassoDrawing {
public:
  virtual ~QvivLassoDrawing() {}

  virtual void draw(QPainter *painter,
                    QvivLassoContext Context) = 0;
};
  
// The lasso functionality
class QvivLasso {
public:
  QvivLasso(QWidget *widget);
  ~QvivLasso();
  int getLabelForPixel(int colIdx, int rowIdx);
  static QColor getColorForLabel(int label);
  void setLassoDrawing(QvivLassoDrawing *lassoDrawing);

  // Call the current lasso drawing. Should typically not be used directly.
  void draw(QPainter *painter);

  // Expose and update the lasso.
  void update(void);

  // This *must* be called by in the owners resizeEvent as follows
  //
  //   lasso->resize(event->size());
  void resize(const QSize& size);

  // Accessor to the widget handled by the lasso.
  QWidget *widget(void);

private:
  class Priv;
  Priv *d;
};

#endif /* LASSO */
