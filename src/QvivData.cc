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
#include <limits>
#include <fmt/core.h>


using namespace std;
using namespace fmt;

QvivDataSet::QvivDataSet(QvivColor _color, double line_width)
{
  SetDefaultVals();
  color = _color;
  this->line_width = line_width;
}

QvivDataSet::QvivDataSet(QVariantMap variant)
{
  SetDefaultVals();

  if (variant.contains("color"))
      color = QvivX11Colors::LookupColor(variant["color"].toString().toUtf8());
  if (variant.contains("line_width"))
      line_width = variant["line_width"].toFloat();
  if (variant.contains("is_visible"))
      is_visible = variant["is_visible"].toBool();

  dashes.clear();

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

QvivDataSet::QvivDataSet(const agg::svg::path_renderer& svg) :
    svg(svg),
    has_svg(true)
{
    SetDefaultVals();
    has_svg = true;
}

QvivBalloons::~QvivBalloons()
{
    clear();
}

void QvivBalloons::clear(void)
{
    balloon_strings.clear();
    sprites.clear();
}

int QvivBalloons::add_balloon(const char *balloon_string)
{
    balloon_strings.push_back(balloon_string);
    return (int)balloon_strings.size()-1;
}
    
// Takes over ownership!
int QvivBalloons::add_sprite(shared_ptr<QImage> sprite)
{
  sprites.push_back(sprite);
  return (int)sprites.size()-1;
}
    
const char *QvivBalloons::get_balloon_text(int balloon_index)
{
    if (resolver) {
        this->resolved_text = resolver->getString(balloon_index);
        return resolved_text.c_str();
    }
  
    if (balloon_index < 0 || balloon_index >= (int)balloon_strings.size())
        return nullptr;
  
    if (balloon_strings[balloon_index].size()==0)
        return nullptr;
  
    return balloon_strings[balloon_index].c_str();
}

const QImage *QvivBalloons::get_sprite(int sprite_index)
{
  if (sprite_index < 0 || sprite_index >= (int)sprites.size())
      return NULL;
  
  return sprites[sprite_index].get();
}


// Create a dataset from a QVariant
QvivData::QvivData()
{
}

QvivData::~QvivData(void)
{
}  

QvivData::QvivData(QVariant /*variant*/)
{
#if 0

  // disabled for now
  if (variant.type() != QVariant::Map)
      return;

  QVariantMap map = variant.toMap();
  if (map.contains("data_sets"))
      {
        QVariantList DataSets = map["data_sets"].toList();
        for (QVariantList::iterator it=DataSets.begin(); it!=DataSets.end(); ++it)
            data_sets.emplace_back(it->toMap());
      }
  if (map.contains("balloons"))
      {
        QVariantList BalloonList = map["balloons"].toList();
        for (QVariantList::iterator it=BalloonList.begin(); it!=BalloonList.end(); ++it)
            balloons.add_balloon(it->toString().toUtf8());
      }
#endif
}

void QvivData::get_bounds(double& xmin,
                          double& ymin,
                          double& xmax,
                          double& ymax)
{
    xmin = ymin = numeric_limits<double>::max();
    xmax = ymax = numeric_limits<double>::min();

    for (auto& ds : data_sets) {
        if (ds->has_svg) {
            double mx,my,Mx,My;
            ds->svg.bounding_rect(&mx,&my,&Mx,&My);
            
            if (mx < xmin)
                xmin = mx;
            if (Mx > xmax)
                xmax = Mx;
            if (my < ymin)
                ymin = my;
            if (My > ymax)
                ymax = My;
        }

        for (auto& p : ds->points) {
            if (p.x < xmin)
                xmin = p.x;
            if (p.x > xmax)
                xmax = p.x;
            if (p.y < ymin)
                ymin = p.y;
            if (p.y > ymax)
                ymax = p.y;
        }
    }

}

