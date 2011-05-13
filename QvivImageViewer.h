#ifndef QVIVIMAGEVIEWER_H
#define QVIVIMAGEVIEWER_H

#include <QWidget>

class QvivImageViewer : public QWidget {
public:
    QvivImageViewer(QWidget *parent,
                    QImage image);
    ~QvivImageViewer();
    int zoom_around_fixed_point(double new_scale_x,
                                double new_scale_y,
                                double old_x,
                                double old_y,
                                double new_x,
                                double new_y);
    int zoom_in(int x, int y, double factor);
    int zoom_out(int x, int y, double factor);
    int zoom_translate(int dx, int dy);

protected:
    void paintEvent(QPaintEvent * event);
    void mousePressEvent (QMouseEvent * event);
    void mouseReleaseEvent (QMouseEvent * event);
    void mouseMoveEvent (QMouseEvent *event);
    QSize sizeHint() const;
    QSize Resize(QResizeEvent) const;

private:
    class Priv;
    Priv *d;
};


#endif /* QVIVIMAGEVIEWER */
