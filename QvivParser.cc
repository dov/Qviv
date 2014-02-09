//======================================================================
//  QvivParser.cpp - Parse a giv file.
//
//  Note that only a subset of the giv standard is supported.
//
//  Dov Grobgeld <dov.grobgeld@gmail.com>
//  Fri Sep  2 14:40:47 2011
//----------------------------------------------------------------------

#include <stdio.h>

using namespace std;

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
  bool CheckMatch(int Index, const char* Candidate);

  // Get the rest of a string starting at the position.
  const char *GetRestAsString(int Index);

  // Get a word as a floating point value.
  double GetFloat(int Index);
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
bool CBoundaries::CheckMatch(int Index, const char* Candidate)
{
  if (Index >= (int)size())
    return false;

  pair<int,int>& p((*this)[Index]);

  if ((int)strlen(Candidate) != (int)(p.second - p.first))
    return false;

  return InspectionString->find(Candidate,p.first)==0;
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

std::ostream& operator<<(std::ostream& os, const CBoundaries&b)
{
  for (auto p: b)
    os << "[" << p.first << ',' << p.second << ") ";
  return os;
}

void ParseFile(const char *FileName,
               // Output
               QvivData& *Data)
{
    FILE *GIV;

    GIV = fopen(filename, "rb");
    if (!GIV)
	return -1;

    is_new_set = TRUE;
    gboolean empty_line = false;
    while(!empty_line) {
	char S_[256];
	int len;
	
	linenum++;
	fgets(S_, sizeof(S_), GIV);
	len = strlen(S_);

        // Skip damaged sections with all NULLS
        if (len==0)
            empty_line = true;

	// Get rid of CR and LF at end of line
        int org_len = len;
	while (len>0 && (S_[len-1] == '\r' || S_[len-1] == '\n')) {
	    S_[len-1] = 0;
	    len--;
	}

        // Get out if we didn't get a \r or \n at the end of the line!
        if (org_len == len)
            break;
	
	if (is_new_set || marks==NULL) {
	    marks = new_giv_dataset(num_sets);
	    marks->color = set_colors[num_sets % nmarks_colors];
	    marks->file_name = g_strdup(filename);
            g_ptr_array_add(gp->giv_datasets, marks);
	    
	    is_new_set = FALSE;
	    num_sets++;
	}
	
	if (len == 0) {
	    if (marks && ((GArray*)marks->points)->len > 0)
		is_new_set++;
	    continue;
	}

        giv_parser_giv_marks_data_add_line(gp, marks, S_, filename, linenum);

        if (feof(GIV))
            break;
    }
    fclose(GIV);
}
    
