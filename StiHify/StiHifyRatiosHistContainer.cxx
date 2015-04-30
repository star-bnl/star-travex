#include <cmath>

#include "TCanvas.h"
#include "TClonesArray.h"
#include "TH2S.h"
#include "TProfile2D.h"
#include "TVector3.h"

#include "StiHify/StiHifyRatiosHistContainer.h"
#include "StiRootIO/TStiKalmanTrackNode.h"


StiHifyRatiosHistContainer::StiHifyRatiosHistContainer(const char* name, TDirectory* motherDir, Option_t* option) :
   StiHistContainer(name, motherDir, option)
{
}


void StiHifyRatiosHistContainer::CreateRatioHist(const TH1* hNumer, const TH1* hDenom)
{
   this->cd();

   TH1 *myRatio = static_cast<TH1*>(hNumer->Clone());
   myRatio->SetOption("colz");
  
   myRatio->Divide(hNumer, hDenom, 1, 1, "B");

   mHs[std::string(myRatio->GetName())] = myRatio;
}
