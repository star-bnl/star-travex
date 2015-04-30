#include <cmath>

#include "TVector3.h"

#include "StiHify/StiHifyHistContainer.h"
#include "StiRootIO/TStiHitProxy.h"


StiHifyHistContainer::StiHifyHistContainer(const char* name, TDirectory* motherDir, Option_t* option) :
   StiHistContainer(name, motherDir, option),
   hDistClosest2AcceptedHit(nullptr),
   hPullClosestHit1D(nullptr),
   hPullClosestHit2D(nullptr),
   hPullCandidateHits2D(nullptr),
   hChi2CandidateHits(nullptr),
   hCountCandidateHits(nullptr)
{
   BookHists();
}


void StiHifyHistContainer::BookHists()
{
   this->cd();

   mHs["hDistClosest2AcceptedHit"] = hDistClosest2AcceptedHit
      = new TH1I("hDistClosest2AcceptedHit", " ; Distance; Num. of Track Nodes; ", 100, 0, 0.5);
   hDistClosest2AcceptedHit->SetOption("XY hist");

   mHs["hPullClosestHit1D"] = hPullClosestHit1D
      = new TH1I("hPullClosestHit1D", " ; Distance; Num. of Track Nodes; ", 100, 0, 6);

   mHs["hPullClosestHit2D"] = hPullClosestHit2D
      = new TH2I("hPullClosestHit2D", " ; local Z; local Y; Num. of Track Nodes", 50, -10, 10, 50, -10, 10);
   hPullClosestHit2D->SetOption("colz");

   mHs["hPullCandidateHits2D"] = hPullCandidateHits2D
      = new TH2I("hPullCandidateHits2D", " ; local Z; local Y; Num. of Track Nodes", 50, -6, 6, 50, -6, 6);
   hPullCandidateHits2D->SetOption("colz");

   mHs["hChi2CandidateHits"] = hChi2CandidateHits
      = new TH1I("hChi2CandidateHits", " ; #chi^2; Num. of Track Nodes", 50, 0, 50);

   mHs["hCountCandidateHits"] = hCountCandidateHits
      = new TH1I("hCountCandidateHits", " ; Num. of Candidate Hits; Counts", 20, 0, 20);

   mHs["hActiveLayerCounts"] = hActiveLayerCounts
      = new TH2F("hActiveLayerCounts", " ; local Z; local Y; Num. of Track Nodes", 25, -25, 25, 12, -2, 10);
   hActiveLayerCounts->SetOption("colz");
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

   const std::set<TStiHitProxy>& hitCandidates = trkNode.GetAdjacentProxyHits();

   hCountCandidateHits->Fill(hitCandidates.size());

   for (auto iHitCandidate=hitCandidates.begin(); iHitCandidate!=hitCandidates.end(); ++iHitCandidate)
   {
      const TStiHitProxy& hitCandidate = *iHitCandidate;

      TVector3 pull = trkNode.CalcPullToHit( *hitCandidate.GetTStiHit() );
      hPullCandidateHits2D->Fill(pull.Z(), pull.Y());
      hChi2CandidateHits->Fill(hitCandidate.GetChi2());
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
