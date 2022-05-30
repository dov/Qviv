#ifndef QVIVRENDERER_H
#define QVIVRENDERER_H

#include <vector>
#include "QvivPainter.h"

class QvivRenderer {
 public:
    QvivRenderer(std::shared_ptr<QvivData> data,
                 QvivPainter& painter,
                 double _scale_x,
                 double _scale_y,
                 double _shift_x,
                 double _shift_y,
                 double width,
                 double height
                 );
    void paint();
    void set_do_no_transparency(bool do_no_transparency)
    {
        this->do_no_transparency = do_no_transparency;
    }

 private:
    std::shared_ptr<QvivData> data; 
    QvivPainter& painter;
    double scale_x;
    double scale_y;
    double shift_x;
    double shift_y;
    double width;
    double height;
    bool do_no_transparency;
};

#endif /* QVIVRENDERER */
