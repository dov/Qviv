#ifndef MYAPP_H
#define MYAPP_H

#include <QApplication>
#include <QImage>

class MyApp : public QApplication {
  Q_OBJECT

public:
    MyApp(int argc, char *argv[]);
    ~MyApp();
  
  public slots:
    void MyImageAnnotate(QImage*, int, int, double, double);


private:
    class Priv;
    Priv *d;
};



#endif /* MYAPP */
