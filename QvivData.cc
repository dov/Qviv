//======================================================================
//  QvivData.cc - 
//
//  Dov Grobgeld <dov.grobgeld@gmail.com>
//  Sun Jun 12 02:32:51 2011
//----------------------------------------------------------------------
#include "QvivData.h"
#include <malloc.h>

QvivBalloons::~QvivBalloons()
{
    for (int i=0; i<(int)balloon_strings.size(); i++)
        free(balloon_strings[i]);
}

int QvivBalloons::add_balloon(const char *balloon_string)
{
    balloon_strings.push_back(strdup(balloon_string));
    return balloon_strings.size()-1;
}

char *QvivBalloons::get_balloon_text(int balloon_index)
{
    if (resolver)
      return resolver->getString(balloon_index);

    if (balloon_index < 0 || balloon_index > (int)balloon_strings.size())
        return NULL;

    return strdup(balloon_strings[balloon_index]);
}

