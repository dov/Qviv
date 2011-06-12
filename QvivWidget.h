// A widget like GivWidget
#ifndef QVIVWIDGET_H
#define QVIVWIDGET_H

#include <QWidget>
#include "QvivImageViewer.h"

class QvivData;

class QvivWidget : public QvivImageViewer {
public:
    QvivWidget(QWidget *parent,
               QImage image );
    ~QvivWidget();
    void set_qviv_data(QvivData& qviv_data);

protected:
    void imageAnnotate(QImage*, int, int, double, double);
    void mouseMoveEvent (QMouseEvent *event);
    void leaveEvent(QEvent *event);

private:
    class Priv;
    Priv *d;
};


#endif /* QVIVWIDGET */
