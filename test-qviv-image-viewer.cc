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
#include <QPainter>
#include "QvivImageViewer.h"
#include "MyApp.h"


class MyApp::Priv {
public:
    QvivImageViewer *w_imgv;
    QImage *image;
    QPushButton *w_quit;
};

void MyApp::MyImageAnnotate(QImage* image,
                            int shift_x, int shift_y,
                            double scale_x, double scale_y)
{
  int width = d->image->size().width();
  int height = d->image->size().height();
  double x = width/2 * scale_x - shift_x;
  double y = height/2 * scale_y - shift_y;

  QPainter painter(image);
  painter.setRenderHint(QPainter::Antialiasing, true);
  QPen pen(QColor(255,0,0,128), 5);
  painter.setPen(pen);
  painter.drawEllipse(x,y,10,10);
}

MyApp::MyApp(int argc, char *argv[])
    : QApplication(argc, argv)
{
    d = new Priv;
    if (argc<2) {
        fprintf(stderr, "Need image filename!\n");
        ::exit(-1);
    }
        
    d->image = new QImage(argv[1]);

    QWidget *window = new QWidget;
    d->w_imgv = new QvivImageViewer(window,*d->image);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(d->w_imgv);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    //    window->resize(500,400);
    window->setLayout(layout);
    window->show();

    d->w_imgv->grabKeyboard(); 
    QObject::connect(d->w_imgv,
                     SIGNAL(imageAnnotate(QImage*,int,int,double,double)),
                     this, SLOT(MyImageAnnotate(QImage*,int,int,double,double)));
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

