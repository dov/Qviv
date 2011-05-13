// A widget like GivWidget
#ifndef QVIVWIDGET_H
#define QVIVWIDGET_H

#include <QWidget>

class QvivWidget : public QWidget {
public:
    QvivWidget(QWidget *parent = 0);
    ~QvivWidget();

protected:
    void paintEvent(QPaintEvent *evt);
    QSize sizeHint() const;
    QSize Resize(QResizeEvent) const;

private:
    class Priv;
    Priv *d;
};


#endif /* QVIVWIDGET */
