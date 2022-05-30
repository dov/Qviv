// A widget like GivWidget
#ifndef QVIVWIDGET_H
#define QVIVWIDGET_H

#include <QWidget>
#include "QvivImageViewer.h"

class QvivData;

class QvivWidget : public QvivImageViewer {
    Q_OBJECT 

public:
    QvivWidget(QWidget *parent,
               QImage image );
    virtual ~QvivWidget();
    void set_qviv_data(std::shared_ptr<QvivData> qviv_data);

    // Let the user pick a point with the mouse and returns the chosen
    // point. Returns 0 if a point was chosen with the mouse. Returns
    // -1 if Escape was pressed to indicate aborting the picking.
    int pick_point(// output
                   QPointF& picked_point);
    int rubber_band_pick(QPointF start_point,
                         // output
                         QPointF& picked_point);
    void set_view_overlay(bool do_view_overlay);
    void set_view_balloon(bool do_view_overlay);
    void set_measure_scale_and_unit(double scale,
                                    const QString& unit);
    void set_measure(bool do_measure);
    void abort_pick_point(void);
    bool is_measuring(void);
      
protected:
    void keyPressEvent (QKeyEvent * event) override;
    void mousePressEvent (QMouseEvent * event) override;
    void mouseReleaseEvent (QMouseEvent * event) override;
    void imageAnnotate(QImage*, int, int, double, double) override;
    void mouseMoveEvent (QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

signals:
    void qvivOverlayChanged(bool view_overlay);
    void qvivBalloonChanged(bool view_overlay);
    void qvivMeasureChanged(bool measure_changed);

private:
    class Priv;
    Priv *d;
};


#endif /* QVIVWIDGET */
