#include "StiScan/StiScanRatiosHistContainer.h"

#include "TH1.h"
#include "TProfile2D.h"
#include "StiRootIO/H2D.h"
#include "StiRootIO/Profile2D.h"


StiScanRatiosHistContainer::StiScanRatiosHistContainer(const char* name, TDirectory* motherDir, Option_t* option) :
   tvx::HistContainer(name, motherDir, option)
{
}


void StiScanRatiosHistContainer::CreateRatioHist(const TH1* hNumer, const TH1* hDenom)
{
   this->cd();

   TH1* myRatio = static_cast<TH1*>(hNumer->Clone());

   myRatio->SetOption("");

   // Take care of a special case when the objects are of TProfile2D class
   if ( hNumer->InheritsFrom("TProfile2D") && hDenom->InheritsFrom("TProfile2D") )
   {
      delete myRatio;
      myRatio = new H2D( static_cast<const TProfile2D&>(*hNumer) );
      myRatio->SetOption("colz whit_zro");
   }

   myRatio->Add(hDenom, -1);
   myRatio->Divide(hDenom);

   Add(myRatio);
}
