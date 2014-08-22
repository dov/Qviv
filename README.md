# Qviv - A vector and image viewer for Qt

* Author: Dov Grobgeld <dov.grobgeld@gmail.com>
* Created: 2011-05-13
* Keywords: Qt
* URL: <http://github.com/dov/Qviv>
* Version: 0.1

# Description

Qviv is an image and vector viewer widget for Qt. Is is modeled on my
similar widget for Gtk, gtk-image-viewer. See: http://giv.sourceforge.net/gtk-image-viewer/ . A stand alone program 
using this widget is also under construction. The syntax and functionality
of this widget is similar to giv. See: http://giv.sourceforge.net/giv/ .

# Licence

Qviv is licensed under LGPL3.

See COPYING for details.

# Building and testing

## Building test programs

Currently Qviv is built together with one of several test programs, that 
also work as examples (in lack of documtation) about the usage of Qviv.

     cd src
     qmake-qt4 test-qviv-widget.pro  
     make
     ./debug/test-qviv-widget ../test-data/maja.pgm 

## Building qviv

The qviv executable may be built by

     qmake-qt4 qviv.pro
     make
     ./debug/qviv ../test-data/cat-contour.giv

# Interaction with the widget

* ScrollWheel 	Zoom in and out by 10%
* Mouse button 1 	Zoom in by a factor of 2 and center coordinate under mouse
* Mouse button 2 	Pan
* Mouse button 3 	Zoom out by a factor of 2
* F 	Fill image in window
* H 	Horizontal flip
* V 	Vertical flip
* 1 or N 	Show native size
* > or + or = 	Zoom in
* < or - 	Zoom out 

# Documentation 

Currently qviv is lacking documentation. Please refer to the test-*.cc
programs for examples of how to use Qviv and to the documentation
of gtk-image-viewer and giv.
