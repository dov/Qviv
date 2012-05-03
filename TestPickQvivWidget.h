//======================================================================
//  TestPickQvivWidget.h - 
//
//  Dov Grobgeld <dov.grobgeld@gmail.com>
//  Thu May  3 22:56:05 2012
//----------------------------------------------------------------------
#ifndef TESTPICKQVIVWIDGET_H
#define TESTPICKQVIVWIDGET_H

#include <QvivWidget.h>

class TestPickQvivWidget : public QvivWidget {
    Q_OBJECT

public:
    TestPickQvivWidget(QWidget *parent, QImage& image);

protected:
    void keyPressEvent (QKeyEvent * event);

private:
    class Priv;
    Priv *d;
};

#endif /* TESTPICKQVIVWIDGET */
