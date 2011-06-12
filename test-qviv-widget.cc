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
    if (argc<2) {
        fprintf(stderr, "Need image filename!\n");
        ::exit(-1);
    }
        
    d->image = new QImage(argv[1]);

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
    QvivDataSet data_set(QColor(0,255,0),5);

    for (int i=0; i<5; i++) 
        data_set.add_point(OP_MOVE, points[i].x,points[i].y);
    data.data_sets.push_back(data_set);
    
    QvivDataSet data_set1(QColor(255,0,0),5);
    for (int i=0; i<5; i++) 
        data_set1.add_point(OP_MOVE, points[i].x-2,points[i].y);
    data.data_sets.push_back(data_set1);

    QvivDataSet data_set2(QColor("#FFA500"),3);
    for (int i=0; i<5; i++) 
        data_set2.add_point(OP_MOVE, points[i].x+2,points[i].y);
    data.data_sets.push_back(data_set2);

    //    painter.drawEllipse(-15,-15,30,30);
    d->w_imgv->set_qviv_data(data);
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

