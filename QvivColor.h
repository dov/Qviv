//======================================================================
//  QvivColor.h - 
//
//  Dov Grobgeld <dov.grobgeld@gmail.com>
//  Thu Feb  9 22:28:45 2012
//----------------------------------------------------------------------
#ifndef QVIVCOLOR_H
#define QVIVCOLOR_H

struct QvivColor {

  QvivColor(void):
    red(0xffff),
    green(0x0000),
    blue(0x0000),
    alpha(0xffff) {}

  QvivColor(unsigned short _red,
            unsigned short _green,
            unsigned short _blue,
            unsigned short _alpha = 0xffff) :
    red(_red),
    green(_green),
    blue(_blue),
    alpha(_alpha) {}

  QvivColor(unsigned int color) {
    red = 256*(color >> 24);
    green = 256*((color >> 16)&0xff);
    blue = 256*((color >> 8)&0xff);
    alpha = 256*(color&0xff);
  }
  
  unsigned short red;
  unsigned short green;
  unsigned short blue;
  unsigned short alpha;
};

#endif /* QVIVCOLOR */
