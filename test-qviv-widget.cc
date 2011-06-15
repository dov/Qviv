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
    if (argc>=2) 
      d->image = new QImage(argv[1]);
    else
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

    struct {
        double x,y;
    } points[] =  { { 186.5, 176.5 },
                    {186, 176},
                    {186, 177},
                    {187, 177},
                    {187, 176}};

    // Add some points
    QvivData data;
    QvivDataSet data_set(QvivColor(0x00ff00ff),10);

    for (int i=0; i<5; i++) 
        data_set.add_point(OP_MOVE, points[i].x,points[i].y);
    data.data_sets.push_back(data_set);
    
    QvivDataSet data_set1(QvivColor(0xff0000ff),5);
    for (int i=0; i<5; i++) 
        data_set1.add_point(OP_MOVE, points[i].x-2,points[i].y);
    data.data_sets.push_back(data_set1);

    QvivDataSet data_set2(QvivColor(0xffa500ff),3);
    for (int i=0; i<5; i++) 
        data_set2.add_point(OP_MOVE, points[i].x+2,points[i].y);
    data.data_sets.push_back(data_set2);

    // Create a large dataset.
    QvivDataSet data_set_large(QvivColor(0x80606040));
    int n = 100;
    for (int i=0; i<n*n; i++)
      data_set_large.add_point(OP_MOVE, 181+1.0/n*(i%n), 171+1.0/n*(i/n));
    data.data_sets.push_back(data_set_large);

    //    painter.drawEllipse(-15,-15,30,30);
    d->w_imgv->set_qviv_data(data);
    d->w_imgv->set_scroll_area(180,170,190,180);
    //d->w_imgv->set_scroll_area(0,0,190,180);
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

