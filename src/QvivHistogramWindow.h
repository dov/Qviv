//======================================================================
//  QvivHistogramWindow.h - 
//
//  Dov Grobgeld <dov.grobgeld@gmail.com>
//  Sun Dec 30 21:39:16 2012
//----------------------------------------------------------------------
#ifndef QVIVHISTOGRAMWINDOW_H
#define QVIVHISTOGRAMWINDOW_H

#include <QWidget>

//forward declarations
class QHBoxLayout;
class QImage;
class QHBoxLayout;
class QSlider;
class QvivHistogram;
 
class QvivHistogramWindow : public QWidget //inherit from QWidget
{
    Q_OBJECT
public:
    QvivHistogramWindow(QWidget* parent = 0, QImage *image=NULL); //don't forget to pass the parent
    virtual ~QvivHistogramWindow() {}
    void setImage(QImage *image, bool do_reset_scale = false);
    void setCursorGrayLevel(int GrayLevel);
 
private:
    //contained widgets:
    QHBoxLayout *layout;
    QvivHistogram *histogram;
    QSlider *slider;
};


#endif /* QVIVHISTOGRAMWINDOW */
