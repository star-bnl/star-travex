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


void StiHifyHistContainer::FillHists(const StiHifyEvent &event, StiNodeHitStatus hitStatus, const std::set<std::string> *volumeList)
{
   for (auto iTStiKTrack=event.GetTStiKalmanTracks().begin(); iTStiKTrack!=event.GetTStiKalmanTracks().end(); ++iTStiKTrack)
   {
      const TStiKalmanTrack &kalmTrack = *iTStiKTrack;

      for (auto iTStiKTrackNode=kalmTrack.GetNodes().begin(); iTStiKTrackNode!=kalmTrack.GetNodes().end(); ++iTStiKTrackNode)
      {
         const TStiKalmanTrackNode &trkNode = *iTStiKTrackNode;

         switch (hitStatus)
         {
         case StiNodeHitStatus::Any:
            FillHists(trkNode, volumeList);
            break;
         case StiNodeHitStatus::Accepted:
            FillHistsHitsAccepted(trkNode, volumeList);
            break;
         case StiNodeHitStatus::Rejected:
            FillHistsHitsRejected(trkNode, volumeList);
            break;
         default:
            Error("FillHists", "Internal type of Sti hit assigned to this node is not specified. Histograms won't be filled");
            break;
         }
      }
   }
}


void StiHifyHistContainer::FillDerivedHists()
{
   this->cd();
}


void StiHifyHistContainer::FillHists(const TStiKalmanTrackNode &trkNode, const std::set<std::string> *volumeList)
{
   if (volumeList && volumeList->size() && !trkNode.MatchedVolName(*volumeList) )
      return;

   if (!trkNode.IsInsideVolume())
      return;

   if (trkNode.GetHit()) {
      hDistClosest2AcceptedHit->Fill( fabs(trkNode.CalcDistanceToHit() - trkNode.CalcDistanceToClosestHit()) );
   }

   hPullClosestHit1D->Fill(trkNode.CalcDistanceToClosestHit() < 0 ? -1 : (trkNode.CalcDistanceToClosestHit()/trkNode.GetTrackProjErr().Mag()) );

   TVector3 pull = trkNode.CalcPullClosestHit();
   hPullClosestHit2D->Fill(pull.Z(), pull.Y());

   for (auto iCandHit=trkNode.GetAdjacentHits().begin(); iCandHit!=trkNode.GetAdjacentHits().end(); ++iCandHit)
   {
      TVector3 pull = trkNode.CalcPullToHit(**iCandHit);
      hPullCandidateHits2D->Fill(pull.Z(), pull.Y());
   }

   hActiveLayerCounts->Fill(trkNode.GetPositionLocal().Z(), trkNode.GetPositionLocal().Y());
}


void StiHifyHistContainer::FillHistsHitsAccepted(const TStiKalmanTrackNode &trkNode, const std::set<std::string> *volumeList)
{
   if (!trkNode.GetHit())
      return;

   FillHists(trkNode, volumeList);
}


void StiHifyHistContainer::FillHistsHitsRejected(const TStiKalmanTrackNode &trkNode, const std::set<std::string> *volumeList)
{
   if (trkNode.GetHit())
      return;

   FillHists(trkNode, volumeList);
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
