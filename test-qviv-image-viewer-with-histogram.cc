//======================================================================
//  test-qviv-image-viewer.cc - 
//
//  Dov Grobgeld <dov.grobgeld@gmail.com>
//  Tue May 10 12:28:05 2011
//----------------------------------------------------------------------

#include <stdio.h>
#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPainter>
#include <QMouseEvent>
#include <QLabel>
#include <QRect>
#include "QvivImageViewer.h"
#include "QvivHistogram.h"
#include "MyHistApp.h"

class MyHistApp::Priv {
public:
    QWidget *window;
    QvivImageViewer *w_imgv;
    QvivHistogram *w_histo;
    QImage *image;
    QLabel* w_balloon;
};

void MyHistApp::MyImageAnnotate(QImage* image,
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

void MyHistApp::MyMouseMoveEvent (QMouseEvent *event)
{
    static char label_text[100];
    sprintf(label_text, "This is a\nmultiline\n(%d, %d)\nlabel",
            event->x(), event->y());

    // How do you get this to fit the internal text???
    d->w_balloon->setText(label_text);
    d->w_balloon->adjustSize();
    d->w_balloon->move(d->window->geometry().x()+event->x()+15,
                       d->window->geometry().y()+event->y()-15);
    d->w_balloon->show();

    // Get gray level
    int gl = qGray(d->image->pixel(event->x(),
                                   event->y()));
    d->w_histo->setCursorGrayLevel(gl);
}

void MyHistApp::MyLeaveEvent(QEvent */*event*/)
{
    d->w_balloon->hide();
}

MyHistApp::MyHistApp(int argc, char *argv[])
    : QApplication(argc, argv)
{
    d = new Priv;
    if (argc<2) {
        fprintf(stderr, "Need image filename!\n");
        ::exit(-1);
    }
        
    d->image = new QImage(argv[1]);

    d->window = new QWidget;
    d->w_imgv = new QvivImageViewer(d->window,*d->image);
    d->w_histo = new QvivHistogram(NULL, d->image);
    d->w_histo->show();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(d->w_imgv);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    //    window->resize(500,400);
    d->window->setLayout(layout);
    d->window->show();

    d->w_balloon = new QLabel(NULL,Qt::FramelessWindowHint);
    d->w_balloon->setStyleSheet("QLabel { background-color : yellow; color : black; }");

#if 0
    d->w_balloon->setText("foo");
    d->w_balloon->show();
#endif
    d->w_imgv->grabKeyboard(); 

    QObject::connect(d->w_imgv,
                     SIGNAL(qvivImageAnnotate(QImage*,int,int,double,double)),
                     this, SLOT(MyImageAnnotate(QImage*,int,int,double,double)));
    QObject::connect(d->w_imgv,
                     SIGNAL(qvivMouseMoveEvent(QMouseEvent*)),
                     this, SLOT(MyMouseMoveEvent(QMouseEvent *)));

    QObject::connect(d->w_imgv,
                     SIGNAL(qvivLeaveEvent(QEvent*)),
                     this, SLOT(MyLeaveEvent(QEvent*)));
}

MyHistApp::~MyHistApp()
{
    delete d;
}

int main(int argc, char **argv)
{
    MyHistApp app(argc, argv);
    return app.exec();
}

