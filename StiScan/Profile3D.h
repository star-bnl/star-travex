#ifndef Profile3D_h
#define Profile3D_h

#include <string>

#include "TProfile3D.h"
#include "TAxis.h"
#include "StRoot/StHftPool/EventT/H3D.h"
#include "StRoot/StHftPool/EventT/Profile2D.h"


class Profile3D : public TProfile3D
{
public:

   Profile3D();
   Profile3D(std::string name, std::string title, Int_t nbinsx, Double_t xlow, Double_t xup,
             Int_t nbinsy, Double_t ylow, Double_t yup, Int_t nbinsz, Double_t zlow, Double_t zup,
             std::string options="");

   void  ResetBinCumulMode();
   Int_t FillAsCumulative(Double_t x, Double_t y, Double_t z, Double_t t);
   virtual Profile2D *DoProjectProfile2D(const char* name, const char * title, TAxis* projX, TAxis* projY,
                                           bool originalRange, bool useUF, bool useOF) const;
   Profile2D *Project3DProfile(Option_t *option) const;
   virtual Double_t RetrieveBinContent(Int_t bin) const { return (fBinEntries.fArray[bin] > 0) ? fArray[bin]/fBinEntries.fArray[bin] : 0; }
   virtual void Print(Option_t *option) const;
   TH3D *ProjectionXYZ(const char *name, Option_t *option) const;

   static TH2D *DoProject2D(const H3D & h, const char* name, const char * title, TAxis* projX, TAxis* projY, 
                            bool computeErrors, bool originalRange, bool useUF, bool useOF)
   { 
      return h.DoProject2D(name, title, projX,projY, computeErrors, originalRange, useUF, useOF);
   }

protected:

   TArrayC fBinCumulMode;

   ClassDef(Profile3D, 1)
};

#endif
