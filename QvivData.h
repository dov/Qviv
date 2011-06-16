// Datastructure for Qviv data
#ifndef QVIVDATA_H
#define QVIVDATA_H

#include <QColor>
#include <vector>

enum QvivOp
{
    OP_MOVE = 0,
    OP_DRAW = 1,
    OP_QUIVER = 2,
    OP_TEXT = 3
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

// A balloon manager. 
class QvivBalloons
{
  private:
    std::vector<char*> balloon_strings;
    BalloonIndexToStringResolver *resolver;

  public:
    QvivBalloons(BalloonIndexToStringResolver *_resolver=NULL)
        : resolver(_resolver) {}
    ~QvivBalloons();
    int add_balloon(const char *balloon_string);
    char *get_balloon_text(int balloon_index);
    void setResolver(BalloonIndexToStringResolver *_resolver=NULL)
    {
        resolver = _resolver;
    }
};

struct QvivColor {
    QvivColor(void):
        red(0xffff),
        green(0),
        blue(0),
        alpha(0xffff) {}
    QvivColor(unsigned short red,
              unsigned short green,
              unsigned short blue,
              unsigned short alpha);
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

class QvivDataSet
{
  public:
    QvivDataSet(QvivColor _color=QvivColor(0xff0000ff),
                double line_width=1.0):
        color(_color)
    {
        this->color = color;
        this->line_width = line_width;
        is_visible = true;
        num_dashes = 0;
        dashes = NULL;
        do_draw_lines = true;
        do_draw_marks = true;
        do_draw_polygon = false;
        has_quiver = false;
        do_draw_polygon_outline = true;
        do_scale_marks = false;
        quiver_scale = 1.0;
        font_name = NULL;
        text_size = 0;
        do_scale_fonts = false;
        mark_type = MARK_TYPE_FCIRCLE;
        mark_size = 10;
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
    int num_dashes;
    double *dashes;
    bool do_draw_lines;
    bool do_draw_marks;
    bool do_draw_polygon;
    bool has_quiver;
    bool do_draw_polygon_outline;
    bool do_scale_marks;
    double quiver_scale;
    char *font_name;
    double text_size;
    double do_scale_fonts;

    void add_point(QvivOp op, double x, double y, int ballon_index=-1)
    {
        points.push_back(QvivPoint(op,x,y,ballon_index));
    }
};

struct QvivData
{
    std::vector<QvivDataSet> data_sets;
    QvivBalloons balloons;
};

#endif /* QVIVDATA */
