/*
 * Copyright (C) 2011 Dov Grobgeld <dov.grobgeld@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */
#ifndef QVIVIMAGEVIEWER_H
#define QVIVIMAGEVIEWER_H

#include <QObject>
#include <QImage>
#include <QAbstractScrollArea>

class QvivImageViewer : public QAbstractScrollArea {
    Q_OBJECT 

public:
    QvivImageViewer(QWidget *parent,
                    QImage image);
    virtual ~QvivImageViewer();
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
    void canv_coord_to_img_coord(double cx, double cy,
                                 // output
                                 double& imgx, double& imgy);

    void img_coord_to_canv_coord(double imgx, double imgy,
                                 // output
                                 double& canvx, double& canvy);


    // Get a pointer to the current image
    QImage *get_image(void);

    // Change the image being displayed
    void set_image(QImage& image);

    // Set the scroll area - This is ignored if an image is being used
    void set_scroll_area(double scroll_min_x,
                         double scroll_min_y,
                         double scroll_max_x,
                         double scroll_max_y);

    // Check if we are doing mouse dragging
    bool get_mouse_scrolling(void);
    void get_scale_and_shift(double &scale_x,
                             double &scale_y,
                             int& shift_x,
                             int& shift_y);
    // Force a redraw
    void redraw();
    void set_frozen(bool frozen);

protected:
    void paintEvent(QPaintEvent * event);
    void resizeEvent ( QResizeEvent * event );
    void mousePressEvent (QMouseEvent * event);
    void mouseReleaseEvent (QMouseEvent * event);
    void mouseMoveEvent (QMouseEvent *event);
    void wheelEvent (QWheelEvent *event);
    void keyPressEvent (QKeyEvent * event);
    void scrollContentsBy (int dx, int dy);
    void leaveEvent(QEvent *event);
    QSize sizeHint() const;
    virtual void imageAnnotate(QImage* image,
                               int x0, int y0,
                               double scale_x, double scale_y);


signals:
    // The image annotate callback is used for drawing overlays
    // on the image.
    void qvivImageAnnotate(QImage* image,
                           int x0, int y0,
                           double scale_x, double scale_y);
    void qvivMouseMoveEvent (QMouseEvent *event);
    void qvivLeaveEvent (QEvent *event);

private:
    class Priv;
    Priv *d;
};


#endif /* QVIVIMAGEVIEWER */
