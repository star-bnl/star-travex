#include <cmath>

#include "TCanvas.h"
#include "TClonesArray.h"
#include "TH2S.h"
#include "TProfile2D.h"
#include "TVector3.h"

#include "StiHify/StiHifyRatiosHistContainer.h"
#include "StiRootIO/TStiKalmanTrackNode.h"


StiHifyRatiosHistContainer::StiHifyRatiosHistContainer() : TDirectoryFile(),
   mHs()
{
}


StiHifyRatiosHistContainer::StiHifyRatiosHistContainer(const char* name, TDirectory* motherDir, Option_t* option) :
   TDirectoryFile(name, name, option, motherDir),
   mHs()
{
}


StiHifyRatiosHistContainer::~StiHifyRatiosHistContainer()
{
   while (!mHs.empty()) {
      delete mHs.begin()->second;
      mHs.erase(mHs.begin());
   }
}


void StiHifyRatiosHistContainer::CreateRatioHist(const TH1* hNumer, const TH1* hDenom)
{
   this->cd();

   TH1 *myRatio = static_cast<TH1*>(hNumer->Clone());
   myRatio->SetOption("colz");
  
   myRatio->Divide(hNumer, hDenom, 1, 1, "B");

   mHs[std::string(myRatio->GetName())] = myRatio;
}


void StiHifyRatiosHistContainer::SaveAllAs(std::string prefix)
{
   TCanvas canvas("canvas", "canvas", 1000, 800);
   canvas.UseCurrentStyle();
   canvas.SetGridx(true);
   canvas.SetGridy(true);

   HistMapIter iHist = mHs.begin();

   for ( ; iHist!=mHs.end(); ++iHist) {
      // For shorthand
      string   objName = iHist->first;
      TObject *obj      = iHist->second;

      if (!obj) {
         Error("SaveAllAs", "No object found for key %s. Skipping...", objName.c_str());
         continue;
      }

      char* opts = (char*) obj->GetOption();

      if (strstr(opts, "logX")) canvas.SetLogx(true);
      else canvas.SetLogx(false);

      if (strstr(opts, "logY")) canvas.SetLogy(true);
      else canvas.SetLogy(false);

      if (strstr(opts, "logZ")) canvas.SetLogz(true);
      else canvas.SetLogz(false);

      obj->Draw();

      string sFileName = prefix + "/c_" + objName + ".png";
      canvas.SaveAs(sFileName.c_str());
   }
}
