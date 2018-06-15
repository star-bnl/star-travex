#ifndef H3_h
#define H3_h

#include "TH3D.h"


class H3D : public TH3D
{
public:

TH2D *DoProject2D(const char* name, const char * title, TAxis* projX, TAxis* projY,  
                    bool computeErrors=false, bool originalRange=false,
                    bool useUF=false, bool useOF=false) const;
   void Print(Option_t *option) const;

   ClassDef(H3D, 1)
};

#endif
