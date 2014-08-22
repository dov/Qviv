//======================================================================
//  QvivData.cc - 
//
//  Dov Grobgeld <dov.grobgeld@gmail.com>
//  Sun Jun 12 02:32:51 2011
//----------------------------------------------------------------------
#include <QImage>
#include "QvivData.h"
#include "QvivX11Colors.h"
#include <malloc.h>
#include <string.h>

QvivDataSet::QvivDataSet(QVariantMap variant)
{
  SetDefaultVals();

  if (variant.contains("color"))
      color = QvivX11Colors::LookupColor(variant["color"].toString().toUtf8());
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
  font_size_in_points = 16;
  do_scale_fonts = false;
  mark_type = MARK_TYPE_FCIRCLE;
  mark_size = 10;
  arrow_type = ARROW_TYPE_NONE;

  if (variant.contains("points"))
      {
        const QVariantList& points(variant["point"].toList());
        int num_points = points.size();
        for (int i=0; i<num_points; i++)
            {
              const QVariantMap& point(points[i].toMap());
              double x = point["x"].toDouble();
              double y = point["y"].toDouble();
              QvivOp op = OP_DRAW; // Default
              if (point.contains("op"))
                  op = QvivOp(point["op"].toInt());
              int balloon_index = -1;
              if (point.contains("balloon"))
                  balloon_index = point["balloon"].toInt();
              this->points.push_back(QvivPoint(op,x,y,balloon_index));
            }
      }
}

QvivBalloons::~QvivBalloons()
{
    clear();
}

void QvivBalloons::clear(void)
{
    for (int i=0; i<(int)balloon_strings.size(); i++)
        free(balloon_strings[i]);
    for (int i=0; i<(int)sprites.size(); i++)
        delete sprites[i];
    balloon_strings.clear();
    sprites.clear();
    if (resolved_text) {
        free(resolved_text);
        resolved_text = NULL;
    }
}

int QvivBalloons::add_balloon(const char *balloon_string)
{
  balloon_strings.push_back(strdup(balloon_string));
  return balloon_strings.size()-1;
}
    
// Takes over ownership!
int QvivBalloons::add_sprite(QImage *sprite)
{
  sprites.push_back(sprite);
  return sprites.size()-1;
}
    
const char *QvivBalloons::get_balloon_text(int balloon_index)
{
    if (resolver) {
        if (resolved_text)
            free(resolved_text);
        resolved_text = resolver->getString(balloon_index);
        return resolved_text;
    }
  
    if (balloon_index < 0 || balloon_index >= (int)balloon_strings.size())
        return NULL;
  
    if (strlen(balloon_strings[balloon_index])==0)
        return NULL;
  
    return strdup(balloon_strings[balloon_index]);
}

const QImage *QvivBalloons::get_sprite(int sprite_index)
{
  if (sprite_index < 0 || sprite_index >= (int)sprites.size())
      return NULL;
  
  return sprites[sprite_index];
}


// Create a dataset from a QVariant
QvivData::QvivData(QVariant variant)
{
  if (variant.type() != QVariant::Map)
      return;

  QVariantMap map = variant.toMap();
  if (map.contains("data_sets"))
      {
        QVariantList DataSets = map["data_sets"].toList();
        for (QVariantList::iterator it=DataSets.begin(); it!=DataSets.end(); ++it)
            data_sets.push_back(QvivDataSet(it->toMap()));
      }
  if (map.contains("balloons"))
      {
        QVariantList BalloonList = map["balloons"].toList();
        for (QVariantList::iterator it=BalloonList.begin(); it!=BalloonList.end(); ++it)
            balloons.add_balloon(it->toString().toUtf8());
      }
}
