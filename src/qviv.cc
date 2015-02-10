//======================================================================
//  qviv.cc - A giv like application in qt.
//
//  Dov Grobgeld <dov.grobgeld@gmail.com>
//  Mon May  9 21:33:58 2011
//----------------------------------------------------------------------


/** 
 * Testing the use of drawing primitives in a custom widget.
 *
 */
#include <stdio.h>
#include <fstream>
#include "QvivWidget.h"
#include <QApplication>
#include <QPushButton>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QGraphicsRectItem>
#include <QPaintEvent>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include "QvivData.h"
#include "QvivParser.h"
#include "agg/agg_svg_parser.h"
#include <iostream>

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
    QvivWidget *w_canvas;
    QPushButton *w_quit;
};

MyApp::MyApp(int argc, char *argv[])
    : QApplication(argc, argv)
{
    d = new Priv;
    int my_argc = this->arguments().count();
    printf("my_argc = %d\n", my_argc);

    int argp = 1;
    while(argp < my_argc && arguments().at(argp)[0]=='-') {
        QString S_ = arguments().at(argp++);
        // tbd parse
    }

    // Assume a single image.
    QImage Image;
    QvivData *Data = new QvivData;
    QString RelPath;
    if (argp < my_argc) {
        QString Filename = arguments().at(argp++);
        QFileInfo fi(Filename);
        RelPath = fi.absoluteDir().absolutePath();

        if (Filename.right(4) == ".svg") {
          agg::svg::parser Parser(Data->svg);
          Parser.set_swap_red_blue(true);  // Needed for Qt
          string SvgString;;
          std::ifstream Stream;
        
          Stream.open((const char*)Filename.toUtf8(),ios_base::in|ios::binary);
          if (!Stream.good()) {
              // tbd die gracefully
            printf("Failed opening file!");
            exit(-1);
          }
        
          SvgString.assign((std::istreambuf_iterator<char>(Stream)), std::istreambuf_iterator<char>());
          Stream.close();
          
          Parser.parse_string(SvgString.c_str());
          
        }
        else if (Filename.right(4) == ".giv") {
            qDebug() << "TODO: a giv file!";
            ParseFile(Filename,
                      // output
                      *Data);
        }
        else
            Image.load(Filename);
    }
    
    d->w_quit = new QPushButton(QString::fromUtf8("quit"));
    d->w_canvas = new QvivWidget(NULL, Image);
    d->w_canvas->set_qviv_data(Data);
    double xmin,ymin,xmax,ymax;
    Data->get_bounds(// output
                     xmin,ymin,xmax,ymax);
    // Check for image references and load the first image
    if (Data->images.size()) {
        QString Filename = Data->images[0];
        // TBD - add path from GivFilename to image so that it is relative.
        if (!(Filename.contains("/") || Filename.contains("\\")))
            Filename = RelPath + "/" + Filename;
        Image.load(Filename);
        d->w_canvas->set_image(Image);
    }

    d->w_canvas->set_scroll_area(xmin,ymin,xmax,ymax);
    QObject::connect(d->w_quit,
                     SIGNAL(clicked()),
                     this,
                     SLOT(quit()));

    QWidget *window = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(d->w_canvas);
    layout->addWidget(d->w_quit);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    //    window->resize(500,400);
    window->setLayout(layout);
    window->show();
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
