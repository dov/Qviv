//======================================================================
//  test-qviv-widget.cc - Test the QvivWidget
//
//  Dov Grobgeld <dov.grobgeld@gmail.com>
//  Mon Jun  6 08:02:03 2011
//----------------------------------------------------------------------

/** 
 * Testing the use of drawing primitives in a custom widget.
 *
 */
#include <stdio.h>
#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPainter>
#include <QMouseEvent>
#include <QLabel>
#include <QRect>
#include "QvivWidget.h"
#include "QvivData.h"
#include <math.h>

class MyApp : public QApplication {
public:
    MyApp(int argc, char *argv[]);
    ~MyApp();
  
private:
    class Priv;
    Priv *d;
};


class MyApp::Priv {
public:
    QWidget *window;
    QvivWidget *w_imgv;
    QImage *image;
};

MyApp::MyApp(int argc, char *argv[])
    : QApplication(argc, argv)
{
    d = new Priv;
    d->image = new QImage();
    d->window = new QWidget;
    d->w_imgv = new QvivWidget(d->window,*d->image);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(d->w_imgv);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    //    window->resize(500,400);
    d->window->setLayout(layout);
    d->window->show();

    // Add some points
    QvivData *data = new QvivData;
    QImage *sprite = new QImage("drop.svg");
    int sprite_id = data->balloons.add_sprite(sprite);

    QvivDataSet data_set(QvivColor(0x00ff0080),10);
    for (int i=0; i<5; i++) {
        double theta = i*M_PI/2;
        double x=cos(theta)*100;
        double y=sin(theta)*100;
        data_set.add_point(OP_DRAW, x,y);
    }
    data->data_sets.push_back(data_set);
    
    QvivDataSet data_set1;
    data_set1.add_point(OP_SPRITE, 50,50, sprite_id);
    data->data_sets.push_back(data_set1);

    d->w_imgv->set_qviv_data(data);
    if (argc>=2) {
        d->image = new QImage(argv[1]);
        d->w_imgv->set_image(*(d->image));
    }

    d->w_imgv->zoom_fit();
}

MyApp::~MyApp()
{
    delete d;
}

int main(int argc, char **argv)
{
    MyApp app(argc, argv);
    return app.exec();
}

