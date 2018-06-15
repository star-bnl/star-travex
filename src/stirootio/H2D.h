#ifndef H2_h
#define H2_h

#include "TH2D.h"
#include "TProfile2D.h"


class H2D : public TH2D
{
public:

   H2D();
   H2D(const TProfile2D& prof2d);

   ClassDef(H2D, 1)
};

#endif
