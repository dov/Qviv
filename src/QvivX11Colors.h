//======================================================================
//  QvivColors.h - Parse X11 colors into Qviv colors.
//
//  Dov Grobgeld <dov.grobgeld@gmail.com>
//  Thu Feb  9 22:03:42 2012
//----------------------------------------------------------------------
#ifndef QVIVX11COLORS_H
#define QVIVX11COLORS_H

#include <map>
#include <string>
#include "QvivColor.h"

// Singleton class for color lookups
class QvivX11Colors {
  public:    
    static std::map<std::string,unsigned int> m_NameToColor;
    static QvivColor LookupColor(const char *ColorName);

  private:
    // Constructor - private so that it will not be called
    QvivX11Colors(void);
    static QvivX11Colors *Instance;
};      


#endif /* QVIVX11COLORS */
