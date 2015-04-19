#include <cmath>

#include "TCanvas.h"
#include "TClonesArray.h"
#include "TH2S.h"
#include "TProfile2D.h"
#include "TVector3.h"

#include "StiHify/StiHifyHistContainer.h"
#include "StiRootIO/TStiKalmanTrackNode.h"


StiHifyHistContainer::StiHifyHistContainer(StiScanPrgOptions& prgOpts) : TDirectoryFile(),
   mHs()
{
   BookHists();
}


StiHifyHistContainer::StiHifyHistContainer(StiScanPrgOptions& prgOpts, const char* name, TDirectory* motherDir, bool doProjection, Option_t* option) :
   TDirectoryFile(name, name, option, motherDir),
   mHs()
{
   BookHists();
}


StiHifyHistContainer::~StiHifyHistContainer()
{
   while (!mHs.empty()) {
      delete mHs.begin()->second;
      mHs.erase(mHs.begin());
   }
}


void StiHifyHistContainer::BookHists()
{
   this->cd();





}


void StiHifyHistContainer::FillHists(const StiHifyEvent &event, const std::set<std::string> *volumeList)
{
   std::vector<TStiKalmanTrack>::const_iterator iTStiKTrack = event.GetTStiKalmanTracks().begin();

   for ( ; iTStiKTrack != event.GetTStiKalmanTracks().end(); ++iTStiKTrack)
   {
      const TStiKalmanTrack &kalmTrack = *iTStiKTrack;
      FillHists(kalmTrack, volumeList);
   }
}


void StiHifyHistContainer::FillDerivedHists()
{
   this->cd();








}


void StiHifyHistContainer::FillHists(const TStiKalmanTrack &kalmTrack, const std::set<std::string> *volumeList)
{




}


void StiHifyHistContainer::SaveAllAs(std::string prefix)
{
   TCanvas canvas("canvas", "canvas", 1400, 600);
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
