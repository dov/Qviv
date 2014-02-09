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
#include "QvivWidget.h"
#include <QApplication>
#include <QPushButton>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QGraphicsRectItem>
#include <QPaintEvent>
#include "QvivParser.h"
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
    if (argp < my_argc)
        Image.load(arguments().at(argp++));
    
    d->w_canvas = new QvivWidget(NULL, Image);
    d->w_quit = new QPushButton(QString::fromUtf8("quit"));

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
