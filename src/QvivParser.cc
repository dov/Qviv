//======================================================================
//  QvivParser.cpp - Parse a giv file.
//
//  Note that only a subset of the giv standard is supported.
//
//  Dov Grobgeld <dov.grobgeld@gmail.com>
//  Fri Sep  2 14:40:47 2011
//----------------------------------------------------------------------

#include <QString>
#include <QDebug>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include "QvivData.h"
#include "QvivX11Colors.h"

using namespace std;

#if defined(_WIN32) || defined(_WIN64)
  #define strncasecmp _strnicmp
#endif
  
// A class containing word boundaries on string.
class CBoundaries : public vector<pair<int,int> >
{
private:
  // String under inspection
  const std::string *InspectionString;

public:
  CBoundaries() {
    reserve(100);
  }
  void ParseBoundaries(const std::string& s);

  // Answer whether the string TestString matches the Index'th word.
  bool CheckMatch(int Index, const char* Candidate) const;

  // Get the rest of a string starting at the position.
  const char *GetRestAsString(int Index);

  // Get a word as a floating point value.
  double GetFloat(int Index);

  // Get a word as a floating point value.
  int GetInt(int Index);

  // Get a word as string
  string GetWordAsString(int Index) const;
};

// Build the boundaries of all space separated words in the string.
void CBoundaries::ParseBoundaries(const std::string& s)
{
  clear();

  InspectionString = &s;
  int p = 0;
  while (s[p])
  {
    // Find first character of word
    while(s[p] && isspace(s[p]))
      p++;

    // Find last word character
    int pe=p;
    while (s[pe] && !isspace(s[pe]))
      pe++;

    push_back(pair<int,int>(p,pe));
    p = pe;
  }
}

// Check if the Index' word matches the Candidate.
bool CBoundaries::CheckMatch(int Index, const char* Candidate) const
{
  if (Index >= (int)size())
    return false;

  const pair<int,int>& p((*this)[Index]);

  if ((int)strlen(Candidate) != (int)(p.second - p.first))
      return false;

  return strncasecmp(Candidate,InspectionString->c_str()+p.first,
                     p.second-p.first) == 0;
}

// Get the remaining string starting at word number Index
const char *CBoundaries::GetRestAsString(int Index)
{
  return InspectionString->c_str() + (*this)[Index].first;
}

// Get a word as a floating point value.
double CBoundaries::GetFloat(int Index)
{
  return atof(GetRestAsString(Index));
}

// Get a word as a floating point value.
int CBoundaries::GetInt(int Index)
{
  return atoi(GetRestAsString(Index));
}

// Get a word as a floating point value.
string CBoundaries::GetWordAsString(int Index) const
{
    auto p = (*this)[Index];
    return string(InspectionString->begin() + p.first,
                  InspectionString->begin() + p.second);
}

std::ostream& operator<<(std::ostream& os, const CBoundaries&b)
{
  for (auto p: b)
    os << "[" << p.first << ',' << p.second << ") ";
  return os;
}

QvivMarkType ParseMarks(const CBoundaries& Boundaries,
                        int index)
{
    if (Boundaries.CheckMatch(index, "fcircle"))
        return MARK_TYPE_FCIRCLE;
    if (Boundaries.CheckMatch(index, "circle"))
        return MARK_TYPE_CIRCLE;
    if (Boundaries.CheckMatch(index, "fsquare"))
        return MARK_TYPE_FSQUARE;
    if (Boundaries.CheckMatch(index, "square"))
        return MARK_TYPE_SQUARE;
    if (Boundaries.CheckMatch(index, "pixel"))
        return MARK_TYPE_PIXEL;

    // Default
    return MARK_TYPE_FCIRCLE;
}

QvivArrowType ParseArrow(const CBoundaries& Boundaries,
                         int index)
{
    if (Boundaries.CheckMatch(index, "start"))
        return ARROW_TYPE_START;
    if (Boundaries.CheckMatch(index, "end"))
        return ARROW_TYPE_END;
    if (Boundaries.CheckMatch(index, "both"))
        return ARROW_TYPE_BOTH;

    // Default
    return ARROW_TYPE_NONE;
}

QvivColor ParseColor(const CBoundaries& Boundaries,
                     int index)
{
    string ColorString(Boundaries.GetWordAsString(index));
    if (ColorString == "none")
        return 0;
    size_t p;

    if (ColorString[0] == '#') {
        if (ColorString.size() == 7)
            return QvivColor((strtol(ColorString.c_str()+1, NULL, 16) << 8) + 0xff);
        else
            return QvivColor(strtol(ColorString.c_str()+1, NULL, 16));
    }
    if ((p=ColorString.find('/'))!= string::npos) {
        unsigned int alpha = int(0xffff * atof(ColorString.c_str() + p+1));
        printf("p s alpha=%d %s %x\n", p, ColorString.c_str()+p+1, alpha);
        ColorString.erase(p);
        QvivColor Color = QvivX11Colors::LookupColor(ColorString.c_str());
        Color.alpha = alpha;
        return Color;
    }

    return QvivX11Colors::LookupColor(ColorString.c_str());
}

void ParseFile(const QString& FileName,
               // Output
               QvivData& Data)
{
    ifstream is((const char*)FileName.toUtf8());
    string line;
    CBoundaries Boundaries;
    QvivDataSet DataSet;

    while (getline(is,line)) {
        Boundaries.ParseBoundaries(line);
        
        if (Boundaries.size() == 0) {
            if (DataSet.points.size()) {
                Data.data_sets.push_back(DataSet);
                DataSet = QvivDataSet(); // Clear
            }
            continue;
        }

        if (line[0]=='$') {
          if (Boundaries.CheckMatch(0,"$marks")) {            
              DataSet.mark_type = ParseMarks(Boundaries,1);
              DataSet.do_draw_marks = true;
              continue;
          }
          if (Boundaries.CheckMatch(0,"$arrow")) {            
              DataSet.arrow_type = ParseArrow(Boundaries,1);
              continue;
          }
          if (Boundaries.CheckMatch(0,"$noline")) {            
              DataSet.do_draw_lines = false;
              continue;
          }
          if (Boundaries.CheckMatch(0,"$color")) {            
              DataSet.color= ParseColor(Boundaries,1);
              continue;
          }
          if (Boundaries.CheckMatch(0,"$outline_color")) {            
              DataSet.outline_color= ParseColor(Boundaries,1);
              continue;
          }
          if (Boundaries.CheckMatch(0,"$qviver_color")) {
              DataSet.quiver_color= ParseColor(Boundaries,1);
              continue;
          }
          if (Boundaries.CheckMatch(0,"$polygon")) {            
              DataSet.do_draw_polygon = true;
              continue;
          }
          if (Boundaries.CheckMatch(0,"$lw")) {            
              DataSet.line_width = Boundaries.GetFloat(1);
              continue;
          }
          if (Boundaries.CheckMatch(0,"$mark_size")) {            
              DataSet.mark_size = Boundaries.GetFloat(1);
              continue;
          }
          if (Boundaries.CheckMatch(0,"$scale_marks")) {
              if (Boundaries.size()==1)
                  DataSet.do_scale_marks = 1;
              else
                  DataSet.do_scale_marks = Boundaries.GetInt(1);
              continue;
          }
          if (Boundaries.CheckMatch(0,"$scale_fonts")) {
              if (Boundaries.size()==1)
                  DataSet.do_scale_fonts = 1;
              else
                  DataSet.do_scale_fonts = Boundaries.GetInt(1);
              continue;
          }
  
          // Ignore unhandled directives
          continue;
        }

        if (Boundaries.CheckMatch(0,"m")) {
            DataSet.add_point(OP_MOVE,
                              Boundaries.GetFloat(1),
                              Boundaries.GetFloat(2));
        }
        // Handle text and errors, etc.
        else
            DataSet.add_point(OP_DRAW,
                              Boundaries.GetFloat(0),
                              Boundaries.GetFloat(1));
    }
    if (DataSet.points.size()) 
        Data.data_sets.push_back(DataSet);
}
    
