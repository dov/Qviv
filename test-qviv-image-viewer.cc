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
#include <QvivImageViewer.h>
#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>


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
    QvivImageViewer *w_canvas;
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

    d->w_quit = new QPushButton(QString::fromUtf8("quit"));
    
    QObject::connect(d->w_quit,
                     SIGNAL(clicked()),
                     this,
                     SLOT(quit()));

    QWidget *window = new QWidget;
    d->w_canvas = new QvivImageViewer(window,*image);

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

