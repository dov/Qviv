//======================================================================
//  QvivParser.cpp - Parse a giv file.
//
//  Note that only a subset of the giv standard is supported.
//
//  Dov Grobgeld <dov.grobgeld@gmail.com>
//  Fri Sep  2 14:40:47 2011
//----------------------------------------------------------------------

#include <stdio.h>

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
    
