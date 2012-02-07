//======================================================================
//  QvivData.cc - 
//
//  Dov Grobgeld <dov.grobgeld@gmail.com>
//  Sun Jun 12 02:32:51 2011
//----------------------------------------------------------------------
#include "QvivData.h"
#include <malloc.h>

#if 0
QvivData::QvivDataSet(QVariantMap& variant)
{
  // color = colorNameToQtColor(variant["color"]);
  if (variant.contains("line_width"))
    line_width = variant["line_width"].toFloat();
  if (variant.contains("is_visible"))
    is_visible = variant["is_visible"].toBool();

  num_dashes = 0;
  dashes = NULL;

  if (variant.contains("do_draw_lines"))
    do_draw_lines = variant["do_draw_lines"].toBool();
  if (variant.contains("do_draw_marks"))
    do_draw_marks = variant["do_draw_marks"].toBool();
  if (variant.contains("do_draw_polygon"))
    do_draw_polygon = variant["do_draw_polygon"].toBool();

  has_quiver = false;

  if (variant.contains("do_draw_polygon_outline"))
    do_draw_polygon_outline = variant["do_draw_polygon_outline"].toBool();
  if (variant.contains("do_scale_marks"))
    do_scale_marks = variant["do_scale_marks"].toBool();

  quiver_scale = 1.0;
  font_name = NULL;
  text_size = 0;
  do_scale_fonts = false;
  mark_type = MARK_TYPE_FCIRCLE;
  mark_size = 10;
  arrow_type = ARROW_TYPE_NONE;
}
#endif

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

