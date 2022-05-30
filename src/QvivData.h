// Datastructure for Qviv data
#ifndef QVIVDATA_H
#define QVIVDATA_H

#include "agg/agg_svg_path_renderer.h"
#include <QVariant>
#include <QColor>
#include <vector>
#include <memory>
#include "QvivColor.h"

enum QvivOp
{
    OP_MOVE = 0,
    OP_DRAW = 1,
    OP_CLOSEPATH = 2,
    OP_QUIVER = 3,
    OP_TEXT = 4,
    OP_SPRITE = 5   // A sub image
};

/* Mark types */
enum QvivMarkType
{
    MARK_TYPE_FCIRCLE = 1,
    MARK_TYPE_FSQUARE = 2,
    MARK_TYPE_CIRCLE = 3,
    MARK_TYPE_SQUARE = 4,
    MARK_TYPE_PIXEL = 5,
};

enum QvivArrowType
{
    ARROW_TYPE_NONE = 0,
    ARROW_TYPE_START=1,
    ARROW_TYPE_END=2,
    ARROW_TYPE_BOTH=3
};

class BalloonIndexToStringResolver {
  public:
    BalloonIndexToStringResolver() {}
    virtual ~BalloonIndexToStringResolver() {}

    // Turns the pointer into a string and adds it to the file
    virtual char *getString(int balloon_index) = 0;
};

// Contain data of a point and the points resource index.
class QvivPoint
{
  public:
    QvivPoint(QvivOp op,
              double x, double y,
              int balloon_index=-1)
    {
        this->op = op;
        this->x = x;
        this->y = y;
        this->balloon_index = balloon_index;
    }
    QvivOp op;
    double x,y;
    int balloon_index;
};

// Dataset resources. Currently Balloon, text, and sprites and balloon manager. 
class QvivBalloons
{
  private:
    std::vector<std::string> balloon_strings;
    std::vector<std::shared_ptr<QImage>> sprites;
    std::shared_ptr<BalloonIndexToStringResolver> resolver;
    std::string resolved_text;

  public:
    QvivBalloons(BalloonIndexToStringResolver *_resolver=NULL)
      : resolver(_resolver) {}
    ~QvivBalloons();
    int add_balloon(const char *balloon_string);
    int add_sprite(std::shared_ptr<QImage> sprite);
    void clear(void);
    int get_num_ballons(void) {
        return (int)balloon_strings.size();
    }
    int get_num_sprites(void) {
        return (int)sprites.size();
    }
    const char *get_balloon_text(int balloon_index);
    const QImage *get_sprite(int sprite_index);

    void setResolver(std::shared_ptr<BalloonIndexToStringResolver> _resolver)
    {
        resolver = _resolver;
    }
};

class QvivDataSet
{
  private:
    void SetDefaultVals(void)
    {
        color = 0xff0000ff;
        line_width = 1;
        is_visible = true;
        dashes.clear();
        do_draw_lines = true;
        do_draw_marks = false;
        do_draw_polygon = false;
        has_quiver = false;
        do_draw_polygon_outline = true;
        do_scale_marks = false;
        quiver_scale = 1.0;
        font_name = "";
        font_size_in_points = 16; 
        text_align = 0;
        do_scale_fonts = false;
        mark_type = MARK_TYPE_FCIRCLE;
        mark_size = 10;
        arrow_type = ARROW_TYPE_NONE;
    }

  public:
    QvivDataSet(QvivColor _color=QvivColor(0xff0000ff),
                double line_width=1.0);
    QvivDataSet(QVariantMap variant);
    QvivDataSet(const agg::svg::path_renderer& svg);

    // Use default copy constructor
    // QvivDataSet(const QvivDataSet &other);
    

    void clear()
    {
      SetDefaultVals();
      has_svg = false;
      points.clear();
    }

    QvivColor color;
    QvivColor outline_color;
    QvivColor quiver_color;
    double line_width;
    QvivArrowType arrow_type;
    QvivMarkType mark_type;
    double mark_size;
    bool scale_marks;
    std::vector<QvivPoint> points;
    bool is_visible;
    std::vector<double> dashes;
    bool do_draw_lines;
    bool do_draw_marks;
    bool do_draw_polygon;
    bool has_quiver;
    bool do_draw_polygon_outline;
    bool do_scale_marks;
    double quiver_scale;
    QString font_name;
    int text_align;
    double font_size_in_points;
    double do_scale_fonts;
    QString balloon_text;
    QString path;  // visibility path
    agg::svg::path_renderer svg;  // svg path
    bool has_svg { false };

    void add_point(QvivOp op, double x, double y, int ballon_index=-1)
    {
        points.push_back(QvivPoint(op,x,y,ballon_index));
    }
};

class QvivData
{
  public:
    QvivData(void);
    QvivData(QVariant Variant);
    ~QvivData(void);

    std::vector<std::shared_ptr<QvivDataSet>> data_sets;
    std::vector<QString> images;  // Reference to external files
    QvivBalloons balloons;
  
    void clear(void) {
        balloons.clear();
        data_sets.clear();
    }

    // Get the numeric bounds of all the datasets
    void get_bounds(double& xmin,
                    double& ymin,
                    double& xmax,
                    double& ymax);
};

#endif /* QVIVDATA */
