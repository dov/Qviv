//======================================================================
//  lasso.h - A lasso for qt.
//
//  Dov Grobgeld <dov.grobgeld@gmail.com>
//  Sat May  4 22:20:46 2013
//----------------------------------------------------------------------
#ifndef LASSO_H
#define LASSO_H

typedef enum {
  DOVTK_LASSO_CONTEXT_PAINT,
  DOVTK_LASSO_CONTEXT_MASK,
  DOVTK_LASSO_CONTEXT_LABEL
} DovtkLassoContext;

typedef void (*DovtkLassoDrawing)(QPainter *painter,
                                  DovtkLassoContext Context,
                                  gpointer user_data);


#endif /* LASSO */
