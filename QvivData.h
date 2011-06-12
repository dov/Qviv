// Datastructure for Qviv data
#ifndef QVIVDATA_H
#define QVIVDATA_H

#include <QColor>
#include <vector>

enum QvivOp
{
    OP_MOVE = 0,
    OP_DRAW = 1,
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

enum QvivLineStyle
{
    LS_NO_LINE,
    LS_LINE,
    LS_LINE_ARROW_START,
    LS_LINE_ARROW_END,
    LS_LINE_ARROW_BOTH,
};

class QvivPoint
{
  public:
    QvivPoint(QvivOp op,
              double x, double y,
              int ballon_index=-1)
    {
        this->op = op;
        this->x = x;
        this->y = y;
        this->ballon_index = ballon_index;
    }
    QvivOp op;
    double x,y;
    int ballon_index;
};

// A balloon manager
class QvivBalloons
{
  private:
    std::vector<char*> balloon_strings;

  public:
    ~QvivBalloons();
    void add_balloon(const char *balloon_string);
    const char *get_balloon_text(int balloon_index);
};

class QvivDataSet
{
  public:
    QvivDataSet(QColor color=QColor(255,0,0),
                double line_width=1.0)
    {
        this->color = color;
        this->line_width = line_width;
    }
    QColor color;
    double line_width;
    QvivLineStyle line_style;
    QvivMarkType mark_type;
    double mark_size;
    bool scale_marks;
    std::vector<QvivPoint> points;

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
