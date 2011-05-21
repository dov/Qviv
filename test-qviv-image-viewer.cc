//======================================================================
//  test-qviv-image-viewer.cc - 
//
//  Dov Grobgeld <dov.grobgeld@gmail.com>
//  Tue May 10 12:28:05 2011
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
#include "QvivImageViewer.h"


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
    QvivImageViewer *w_imgv;
    QPushButton *w_quit;
};

MyApp::MyApp(int argc, char *argv[])
    : QApplication(argc, argv)
{
    d = new Priv;
    if (argc<2) {
        fprintf(stderr, "Need image filename!\n");
        ::exit(-1);
    }
        
    QImage *image = new QImage(argv[1]);

    QWidget *window = new QWidget;
    d->w_imgv = new QvivImageViewer(window,*image);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(d->w_imgv);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    //    window->resize(500,400);
    window->setLayout(layout);
    window->show();

    d->w_imgv->grabKeyboard(); 
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

