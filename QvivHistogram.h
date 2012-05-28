//======================================================================
//  QvivHistogram.h - Like GivContrast
//
//  Dov Grobgeld <dov.grobgeld@gmail.com>
//  Mon May 28 21:44:34 2012
//----------------------------------------------------------------------
#ifndef QVIVHISTOGRAM_H
#define QVIVHISTOGRAM_H

#include <QWidget>

class QvivHistogram : public QWidget {
    Q_OBJECT
    
public:
    QvivHistogram(QWidget *parent,
                  QImage *image = NULL);
    ~QvivHistogram(void);
    void setImage(QImage *image);
    void setCursorGrayLevel(int GrayLevel);

protected:
    void paintEvent(QPaintEvent *evt);
    void mouseMoveEvent (QMouseEvent *event);
    void wheelEvent (QWheelEvent *event);

    QSize sizeHint() const
    {
        return QSize(400, 200);
    }

    QSize Resize(QResizeEvent) const;


private:
    class Priv;
    Priv *d;
};
    

#endif /* QVIVHISTOGRAM */
