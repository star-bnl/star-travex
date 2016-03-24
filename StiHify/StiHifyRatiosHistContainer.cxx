#include "StiHify/StiHifyRatiosHistContainer.h"

#include "TH1.h"


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

   if (myRatio->GetDimension() == 1)
   {
      myRatio->Fit("pol0", "Q"); // Fit a line to the data points
   }

   mHs[ std::string(myRatio->GetName()) ].reset(myRatio);
}
