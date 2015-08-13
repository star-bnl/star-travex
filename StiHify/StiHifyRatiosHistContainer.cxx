#include "StiHify/StiHifyRatiosHistContainer.h"

#include "TH1.h"
#include "StiRootIO/Profile2D.h"


StiHifyRatiosHistContainer::StiHifyRatiosHistContainer(const char* name, TDirectory* motherDir, Option_t* option) :
   StiHistContainer(name, motherDir, option)
{
}


void StiHifyRatiosHistContainer::CreateRatioHist(const TH1* hNumer, const TH1* hDenom)
{
   if (!hNumer || !hDenom) {
      Error(__FUNCTION__, "Cannot calculate ratio from non-existing histograms: "
                            "Numerator hist %p / Denumerator hist %p, ", hNumer, hDenom);
      return;
   }

   this->cd();

   TH1 *myRatio = static_cast<TH1*>(hNumer->Clone());

   myRatio->SetOption("colz");
   myRatio->Sumw2();
   myRatio->Divide(hNumer, hDenom, 1, 1, "B");

   mHs[std::string(myRatio->GetName())] = myRatio;

   // Create a 2D profile form the 2D ratio histogram
   if (myRatio->GetDimension() == 2)
   {
      Profile2D* myRatioProfile = new Profile2D(static_cast<TH2D&>(*myRatio));
      myRatioProfile->SetName( (std::string(myRatio->GetName()) + "_pxy").c_str() );

      mHs[std::string(myRatioProfile->GetName())]          = myRatioProfile;
      mHs[std::string(myRatioProfile->GetName()) + "_px"]  = myRatioProfile->ProfileX();
   }
}
