/** 
 * Testing the use of drawing primitives in a custom widget.
 *
 * @return 
 */
#include <stdio.h>
#include <QApplication>
#include <QPushButton>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QGraphicsRectItem>
#include <QPaintEvent>
#include <QResizeEvent>
#include "overlay.h"
#include "qvivLasso.h"


class MyApp : public QApplication {
public:
    MyApp(int argc, char *argv[]);
    ~MyApp();

private:
    class Priv;
    Priv *d;
};

class MyDrawingWidget : public QWidget {
public:
    MyDrawingWidget(QWidget *parent = 0);
    ~MyDrawingWidget();
protected:
    void paintEvent(QPaintEvent *evt);
    void resizeEvent(QResizeEvent *event);

    QSize sizeHint() const
    {
        return QSize(400, 200);
    }

    QSize Resize(QResizeEvent) const;

private:
    class Priv;
    Priv *d;
};

class MyDrawingWidget::Priv
{
  public:
    QvivLasso *lasso;
    QvivLassoDrawing *lassoDrawing;
};

class MyLassoDrawing : public QvivLassoDrawing {
public:
  MyLassoDrawing(QWidget *widget) {
    this->widget = widget;
  }
  virtual void draw(QPainter *painter,
                    QvivLassoContext Context);
  QWidget *widget;
};

MyDrawingWidget::MyDrawingWidget(QWidget *parent)
    : QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    d = new Priv;
    d->lasso = new QvivLasso(this);
    d->lassoDrawing = new MyLassoDrawing(this);
    d->lasso->setLassoDrawing(d->lassoDrawing);
}

MyDrawingWidget::~MyDrawingWidget()
{
  delete d->lasso;
  delete d->lassoDrawing;
  delete d;
}

// See: http://doc.trolltech.com/4.5/painting-basicdrawing.html
// for more examples!
void MyDrawingWidget::paintEvent(QPaintEvent *evt)
{
    QPainter painter(this);
    QImage image("maja.pgm");
    painter.setClipRegion(evt->region());
    double wscale = this->size().width()/100.0;
    double hscale = this->size().height()/100.0;
    painter.drawImage(QRect(wscale*25,
                            hscale*25,
                            wscale*50,
                            hscale*50),
                      image);
                                   

    // A list of points
    QPoint points[4] = {
         QPoint(10*wscale, 80*hscale),
         QPoint(20*wscale, 10*hscale),
         QPoint(80*wscale, 30*hscale),
         QPoint(90*wscale, 70*hscale)
     };
     painter.setRenderHint(QPainter::Antialiasing, true);

     // Use rgb color with alpha
     QPen pen(QColor(0,128,0,128), 3);
     
     painter.setPen(pen);
     painter.drawPolyline(points, 4);
}

void MyDrawingWidget::resizeEvent(QResizeEvent *event)
{
  d->lasso->resize(event->size());
  event->accept();
}

class MyApp::Priv {
public:
    MyDrawingWidget *w_canvas;
    QPushButton *w_quit;
};

MyApp::MyApp(int argc, char *argv[])
    : QApplication(argc, argv)
{
    d = new Priv;
    d->w_canvas = new MyDrawingWidget();
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

void MyLassoDrawing::draw(QPainter *painter,
                          QvivLassoContext Context)
{
  int width = widget->width();
  int height = widget->height();

  // Some default drawing
  painter->setRenderHint(QPainter::Antialiasing);
  painter->setPen(QPen(Qt::blue));
  painter->drawLine(width/8, height/8, 7*width/8, 7*height/8);
  painter->drawLine(width/8, 7*height/8, 7*width/8, height/8);
}


int main(int argc, char **argv)
{
    MyApp app(argc, argv);
    return app.exec();
}

