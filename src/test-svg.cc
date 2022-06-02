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
#include "agg/agg_svg_parser.h"
#include <string>

using namespace std;

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

    // Add an svg file
    string svgfilename;
    if (argc > 1)
        svgfilename = argv[1];
    else
        svgfilename = "oliveoil.svg";
    agg::svg::path_renderer path;
    agg::svg::parser p(path);
    p.set_swap_red_blue(true);
    try {
      p.parse(svgfilename.c_str());
    }
    catch (agg::svg::exception e) {
      fprintf(stderr, "%s\n", e.msg());
      exit(-1);
    }
    auto svg_data_set = make_shared<QvivDataSet>(path);
    auto data = make_shared<QvivData>();

    data->add_dataset(svg_data_set, svgfilename);

    d->w_imgv->set_qviv_data(data);
#if 0
    // For testing image underlay
    if (argc>=2) {
        d->image = new QImage(argv[1]);
        d->w_imgv->set_image(*(d->image));
    }
#endif

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

