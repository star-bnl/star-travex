#include "StiRootIO/H2D.h"


H2D::H2D() : TH2D()
{
}


H2D::H2D(const TProfile2D& prof2d) : TH2D()
{
   prof2d.TH2D::Copy(*this);

   // Correct bin contents after initialization
   for (int i=0; i<fNcells; i++) {
      SetBinContent(i, prof2d.GetBinContent(i));
      SetBinError(i, prof2d.GetBinError(i));
   }
}
