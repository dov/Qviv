#ifndef QVIVIMAGEVIEWER_H
#define QVIVIMAGEVIEWER_H

#include <QWidget>
#include <QAbstractScrollArea>

class QvivImageViewer : public QAbstractScrollArea {
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
    int zoom_to_box(double world_min_x,
                    double world_min_y,
                    double world_max_x,
                    double world_max_y,
                    double pixel_margin,
                    bool preserve_aspect) ;
    void zoom_fit(void);
    void zoom_reset(void);

protected:
    void paintEvent(QPaintEvent * event);
    void resizeEvent ( QResizeEvent * event );
    void mousePressEvent (QMouseEvent * event);
    void mouseReleaseEvent (QMouseEvent * event);
    void mouseMoveEvent (QMouseEvent *event);
    void wheelEvent (QWheelEvent *event);
    void keyPressEvent (QKeyEvent * event);
    void scrollContentsBy (int dx, int dy);
    QSize sizeHint() const;

private:
    class Priv;
    Priv *d;
};


#endif /* QVIVIMAGEVIEWER */
