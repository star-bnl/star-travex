#include "StiScan/StiScanRatiosHistContainer.h"

#include "TH1.h"


StiScanRatiosHistContainer::StiScanRatiosHistContainer(const char* name, TDirectory* motherDir, Option_t* option) :
   StiHistContainer(name, motherDir, option)
{
}


void StiScanRatiosHistContainer::CreateRatioHist(const TH1* hNumer, const TH1* hDenom)
{
   this->cd();

   TH1 *myRatio = static_cast<TH1*>(hNumer->Clone());
   myRatio->SetOption("colz");

   myRatio->Divide(hNumer, hDenom, 1, 1, "B");

   mHs[std::string(myRatio->GetName())] = myRatio;
}
