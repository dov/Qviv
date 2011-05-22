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

#include <QImage>
#include <QAbstractScrollArea>

class QvivImageViewer : public QAbstractScrollArea {
    Q_OBJECT 

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

signals:
    // The image annotate callback is used for drawing overlays
    // on the image.
    void imageAnnotate(QImage image,
                       int x0, int y0,
                       double scale_x, double scale_y);

private:
    class Priv;
    Priv *d;
};


#endif /* QVIVIMAGEVIEWER */
