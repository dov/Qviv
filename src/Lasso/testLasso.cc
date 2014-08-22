/** 
 * Testing the use of drawing primitives in a custom widget.
 *
 * @return 
 */
#include <stdio.h>
#include <math.h>
#include <QApplication>
#include <QPushButton>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QGraphicsRectItem>
#include <QPaintEvent>
#include <QResizeEvent>
#include "QvivLasso.h"


static const double RAD2DEG = 180 / 3.1415926535;

void drawCaliper(QPainter *painter,
                 QvivLassoContext context,
                 double x0, double y0,
                 double x1, double y1)
{
  int margin = 0;

  if (context == QVIV_LASSO_CONTEXT_PAINT)
    painter->setRenderHint(QPainter::Antialiasing, true);
  else
  {
    margin = 5;
    painter->setRenderHint(QPainter::Antialiasing, false);
  }
  
  double angle = atan2(y1-y0,x1-x0);
  painter->translate(0.5 * (x0+x1),
                     0.5 * (y0+y1));
  painter->rotate(angle * RAD2DEG);
  double dist = sqrt((x1-x0)*(x1-x0) + (y1-y0)*(y1-y0));

  // Middle stroke
  QColor color;
  QPen pen;
  if (context == QVIV_LASSO_CONTEXT_PAINT)
    color = QColor(255,0xaa,0,128);
  else if (context == QVIV_LASSO_CONTEXT_LABEL)
  {
    color = QvivLasso::getColorForLabel(1);
    pen.setWidth(5);
  }
  else
  {
    color = Qt::black;
    pen.setWidth(5);
  }

  painter->fillRect(-dist/2-margin, -20-margin,
                    dist+2*margin, 20+2*margin, color);

  // Side left
  if (context == QVIV_LASSO_CONTEXT_PAINT)
    color = QColor(0x50,0x2d,0x16);
  else if (context == QVIV_LASSO_CONTEXT_LABEL)
    color = QvivLasso::getColorForLabel(2);

  painter->setBrush(QBrush(color));
  painter->setPen(pen);

  double calip_height = 50;
  QPainterPath path;
  double x = -dist/2+margin;
  double y = calip_height/2+margin;
  path.moveTo(x,y);
  double dy = -(calip_height+3*margin)/3;
  path.cubicTo(x-15-2*margin,y+dy,
                 x-15-2*margin,y+2.5*dy,
                 x-15-2*margin,y+3*dy);
  x = -dist/2+margin;
  y = -calip_height/2-margin;
  path.lineTo(x,y);
  path.closeSubpath();
  if (context != QVIV_LASSO_CONTEXT_PAINT)
    painter->strokePath(path, pen);
  painter->drawPath(path);

  if (context == QVIV_LASSO_CONTEXT_LABEL)
  {
    color = QvivLasso::getColorForLabel(3);
    pen.setColor(color); 
  }  
  
  path = QPainterPath();  // Empty the path
  painter->setBrush(QBrush(color));
  painter->setPen(pen);

  x = dist/2-margin;
  y = calip_height/2+margin;
  path.moveTo(x,y); 
  path.cubicTo(x + 15+2*margin,y + dy,
               x + 15+2*margin,y + 2.5*dy,
               x + 15+2*margin,y + 3*dy);
  path.lineTo(dist/2-margin,-calip_height/2-margin); 
  path.closeSubpath();

  if (context != QVIV_LASSO_CONTEXT_PAINT)
    painter->strokePath(path, pen);
  painter->drawPath(path);

  if (context == QVIV_LASSO_CONTEXT_PAINT)
  {
    // Draw label within the caliper
    QFont Font("Sans",14);
    painter->setFont(Font);
    painter->setPen(QColor(0,0,0,255));
    QString distString = QString("%1").arg(dist,0,'f',1);
    QFontMetrics fm(Font);
    QRect rect = fm.tightBoundingRect(distString);
    rect.moveTop(-14);
    rect.moveLeft(-rect.width()/2);
    int width = fm.width(distString);
    painter->drawText(rect, Qt::AlignCenter, distString);
  }
}



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
    void mousePressEvent (QMouseEvent * event);
    void mouseReleaseEvent (QMouseEvent * event);
    void mouseMoveEvent (QMouseEvent *event);

    QSize sizeHint() const
    {
        return QSize(400, 200);
    }

    QSize Resize(QResizeEvent) const;

private:
    class Priv;
    Priv *d;
};

class MyLassoDrawing : public QvivLassoDrawing {
public:
  MyLassoDrawing(QWidget *widget)
  {
    this->widget = widget;
    this->x0=this->y0=0;
    this->x1=this->y1=0;
    this->moving = false;
  }
  void draw(QPainter *painter,
            QvivLassoContext Context);
  void setXY0(double x, double y)
  {
    this->x0 = x;
    this->y0 = y;
  }
  void setXY1(double x, double y)
  {
    this->x1 = x;
    this->y1 = y;
  }
  void incXY0(double dx, double dy)
  {
    this->x0 += dx;
    this->y0 += dy;
  }
  void incXY1(double dx, double dy)
  {
    this->x1 += dx;
    this->y1 += dy;
  }

  void setMoving(bool moving)
  {
    this->moving = moving;
  }

  QWidget *widget;
  double x0, y0;
  double x1, y1;
  bool moving;
};

class MyDrawingWidget::Priv
{
  public:
    QvivLasso *lasso;
    MyLassoDrawing *lassoDrawing;
    int last_x, last_y;
    int picking;
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
    d->last_x = d->last_y = -1;
    d->picking = 0;
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

void MyDrawingWidget::mousePressEvent (QMouseEvent * event)
{
  // Update line coordinate to mouse coordinate
  if ((event->modifiers() & Qt::ControlModifier)==0
      && event->button() == 1)
  {
    int x=event->x();
    int y=event->y();
    int label = d->lasso->getLabelForPixel(x,y);

    d->picking = label;
    
    if (d->picking == 0)
    {
      d->lassoDrawing->setXY0(x,y);
      d->lassoDrawing->setXY1(x,y);
      d->picking = 3;
    }
    d->lassoDrawing->setMoving(true);
    d->last_x = x;
    d->last_y = y;
    d->lasso->update();
  }
  event->accept();
}

void MyDrawingWidget::mouseReleaseEvent (QMouseEvent * event)
{
  d->lassoDrawing->setMoving(false);
}

void MyDrawingWidget::mouseMoveEvent (QMouseEvent * event)
{
  // Update line coordinate to mouse coordinate
  if (d->lassoDrawing->moving)
  {
    int x = event->x();
    int y = event->y();
    int dx = x - d->last_x;
    int dy = y - d->last_y;
    d->last_x = x;
    d->last_y = y;

    if (d->picking == 1 || d->picking == 2)
      d->lassoDrawing->incXY0(dx,dy);
    if (d->picking == 1 || d->picking == 3)
      d->lassoDrawing->incXY1(dx,dy);
    
    d->lasso->update();
  }
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
#if 0
  int width = widget->width();
  int height = widget->height();

  // Some default drawing
  painter->setRenderHint(QPainter::Antialiasing);
  painter->setPen(QPen(Qt::blue));
  painter->drawLine(x0,y0,x1,y1);
#endif
  drawCaliper(painter,Context,x0,y0,x1,y1);
}


int main(int argc, char **argv)
{
    MyApp app(argc, argv);
    return app.exec();
}

