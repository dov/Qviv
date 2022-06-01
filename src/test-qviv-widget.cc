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
#include <fmt/core.h>

using namespace std;
using namespace fmt;

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

    struct {
        double x,y;
    } points[] =  { { 186.5, 176.5 },
                    {186, 176},
                    {186, 177},
                    {187, 177},
                    {187, 176}};

    // Add some points
    auto data = make_shared<QvivData>();
    int balloonA = data->balloons.add_balloon("Dataset A");
    int balloonB = data->balloons.add_balloon("Dataset B");
    int balloonC = data->balloons.add_balloon("Dataset C");

    auto data_set = make_shared<QvivDataSet>(QvivColor(0x00ff00ff),10);

    for (int i=0; i<5; i++) 
        data_set->add_point(OP_DRAW, points[i].x,points[i].y,balloonA);
    data->data_sets.push_back(data_set);
    
    auto data_set1 = make_shared<QvivDataSet>(QvivColor(0xff0000ff),5);
    for (int i=0; i<5; i++) 
      data_set1->add_point(OP_MOVE, points[i].x-2,points[i].y,balloonB);
    data_set1->do_draw_marks=true;
    data->data_sets.push_back(data_set1);

    auto data_set2 = make_shared<QvivDataSet>(QvivColor(0xffa500ff),3);
    for (int i=0; i<5; i++) 
      data_set2->add_point(OP_MOVE, points[i].x+2,points[i].y,balloonC);
    data_set2->do_draw_marks=true;
    data->data_sets.push_back(data_set2);

    // Create a large dataset.
    auto data_set_large = make_shared<QvivDataSet>(QvivColor(0x80606040));
    int n = 100;
    for (int i=0; i<n*n; i++)
    {
      int balloon_idx = data->balloons.add_balloon(format("Dataset A, i={}",i).c_str());
      data_set_large->add_point(OP_MOVE, 181+1.0/n*(i%n), 171+1.0/n*(i/n),balloon_idx);
    }
    data_set_large->do_draw_marks=true;
    data->data_sets.push_back(data_set_large);

    // Test text
    int text_id = data->balloons.add_balloon("TopLeft");
    auto text_data_set = make_shared<QvivDataSet>(QvivColor(0x0000ffff));
    text_data_set->add_point(OP_TEXT,200,30,text_id);
    data->data_sets.push_back(text_data_set);

    // Test text Center bottom aligned
    int text_id2 = data->balloons.add_balloon("CenterBottom");
    auto text_data_set2 = make_shared<QvivDataSet>(QvivColor(0x0080ffff));
    text_data_set2->text_align = 6+1;
    text_data_set2->font_size_in_points = 5;
    text_data_set2->add_point(OP_TEXT,200,30,text_id2);
    data->data_sets.push_back(text_data_set2);

    // Draw a dot at the same position for comparison
    auto red_dot = make_shared<QvivDataSet>(QvivColor(0xff0000ff));
    red_dot->add_point(OP_MOVE,200,30,0);
    data->data_sets.push_back(red_dot);

    //    painter.drawEllipse(-15,-15,30,30);
    d->w_imgv->set_qviv_data(data);
    //    d->w_imgv->set_scroll_area(180,170,190,180);
    //d->w_imgv->set_scroll_area(0,0,190,180);

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

