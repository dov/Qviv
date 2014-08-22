//======================================================================
//  QvivHistogramWindow.cpp - 
//
//  Dov Grobgeld <dov.grobgeld@gmail.com>
//  Sun Dec 30 21:42:24 2012
//----------------------------------------------------------------------

#include <QHBoxLayout>
#include <QSlider>
#include "QvivHistogramWindow.h"
#include "QvivHistogram.h"

QvivHistogramWindow::QvivHistogramWindow(QWidget* parent,
                                         QImage *image) : QWidget(parent)
{
    layout = new QHBoxLayout();

    slider = new QSlider();
    histogram = new QvivHistogram(this, image);

    layout->addWidget(slider);
    layout->addWidget(histogram);
}

void QvivHistogramWindow::setImage(QImage *image, bool do_reset_scale)
{
    histogram->setImage(image, do_reset_scale);
}

void QvivHistogramWindow::setCursorGrayLevel(int GrayLevel)
{
    histogram->setCursorGrayLevel(GrayLevel);
}
