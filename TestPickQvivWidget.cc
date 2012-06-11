// An overloaded QvivWidget that adds picking.

#include <QKeyEvent>
#include "TestPickQvivWidget.h"
#include "stdio.h"

class TestPickQvivWidget::Priv {
public:
  int foo;
};
    
TestPickQvivWidget::TestPickQvivWidget(QWidget *parent, QImage& image)
      : QvivWidget(parent, image)
{
}

void TestPickQvivWidget::keyPressEvent (QKeyEvent * event)
{
    QvivWidget::keyPressEvent(event);
    QString k = event->text();

    if (k=="p")
    {
        QPointF point;
        if (pick_point(point) == 0)
            printf("Picked point=(%f,%f)\n",point.rx(),point.ry());
        else
            printf("Picking aborted!\n");
    }
    if (k=="M")
    {
        QPointF point_start, point_end;
        if (pick_point(point_start) == 0)
            printf("Start point=(%f,%f)\n",point_start.rx(),point_start.ry());
        else
            printf("Picking aborted!\n");
        if (rubber_band_pick(point_start,
                             // output
                             point_end) == 0)
            printf("End point=(%f,%f)\n",point_end.rx(),point_end.ry());
    }
}    
