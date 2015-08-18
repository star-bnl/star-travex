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
      std::string ratioHistName(myRatio->GetName());
      ratioHistName += "_pxy";

      Profile2D* myRatioProfile = new Profile2D(static_cast<TH2D&>(*myRatio));

      myRatioProfile->SetName( ratioHistName.c_str() );
      mHs[ratioHistName] = myRatioProfile;

      // Create a profile from a 2D profile
      TProfile* myRatioProfileX   = myRatioProfile->ProfileX();

      mHs[ratioHistName + "_pfx"]  = myRatioProfileX;
   }
}
