#ifndef Profile2D_h
#define Profile2D_h

#include <string>

#include "TProfile2D.h"
#include "TProfile.h"


class Profile2D : public TProfile2D
{
public:

   Profile2D();
   Profile2D(std::string name, std::string title, Int_t nbinsx, Double_t xlow,
             Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, std::string options="");
   Profile2D(std::string name, std::string title, Int_t nbinsx, const Double_t* xbins,
             Int_t nbinsy, const Double_t* ybins, std::string options="");

   void ResetBinEntries(double val);
   using TProfile2D::Multiply;
   Bool_t Multiply(const Profile2D &prof2);
   TProfile *ProfileX(const char *name="_pfx", Int_t firstybin=0, Int_t lastybin=-1, Option_t *option="") const;
   TProfile *ProfileY(const char *name="_pfy", Int_t firstxbin=0, Int_t lastxbin=-1, Option_t *option="") const;

protected:

   virtual TProfile *DoProfile(bool onX, const char *name, Int_t firstbin, Int_t lastbin, Option_t *option) const;

   ClassDef(Profile2D, 1)
};

#endif
