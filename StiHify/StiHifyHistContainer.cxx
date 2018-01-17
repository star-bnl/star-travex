#include <cmath>
#include <boost/algorithm/string/replace.hpp>

#include "TVector3.h"

#include "StiHify/StiHifyHistContainer.h"
#include "StiRootIO/TStiHitProxy.h"


StiHifyHistContainer::StiHifyHistContainer(const StiHifyPrgOptions& prgOpts, const char* name, TDirectory* motherDir, Option_t* option) :
   tvx::HistContainer(name, motherDir, option),
   fPrgOptions(prgOpts),
   hDiffProjToFitPositionWRTHit(nullptr),
   hDiffProjToFitError(nullptr),
   hDist2AcceptedHit(nullptr),
   hDist2ClosestHit(nullptr),
   hPullClosestHit1D(nullptr),
   hPullClosestHit2D(nullptr),
   hPullCandidateHits2D(nullptr),
   hChi2CandidateHits(nullptr),
   hCountCandidateHits(nullptr),
   hActiveLayerCounts(nullptr),
   hProjErrorMag(nullptr)
{
   const double suggestBinWidth = 1;   // desired bin width in cm

   const double z_max = fPrgOptions.GetHistZMax();
   const double z_min = fPrgOptions.GetHistZMin();
   const double y_max = fPrgOptions.GetHistYMax();
   const double y_min = fPrgOptions.GetHistYMin();

   int n_z_bins = ceil( (z_max - z_min) / suggestBinWidth );
   int n_y_bins = ceil( (y_max - y_min) / suggestBinWidth );

   n_z_bins = ( n_z_bins <= 10 ? 10 : (n_z_bins >  20 ? 20 : n_z_bins) );
   n_y_bins = ( n_y_bins <= 10 ? 10 : (n_y_bins >  20 ? 20 : n_y_bins) );

   this->cd();

   hDiffProjToFitPositionWRTHit = new TH1I("hDiffProjToFitPositionWRTHit", " ; Diff. (Projection - Final) Position w.r.t. Hit, cm; Num. of Track Nodes; ", 50, -0.5, 0.5);
   hDiffProjToFitPositionWRTHit->SetOption("XY hist");
   Add(hDiffProjToFitPositionWRTHit);

   hDiffProjToFitError = new TH2I("hDiffProjToFitError", " ; Diff. (Projection - Final) Error_z, cm; Diff. Error_y, cm; Num. of Track Nodes; ", 50, 0, 0.25, 50, 0, 0.25);
   hDiffProjToFitError->SetOption("colz");
   Add(hDiffProjToFitError);

   hDist2AcceptedHit = new TH1I("hDist2AcceptedHit", " ; Closest to Accepted Hits: Distance R, cm; Num. of Track Nodes; ", 100, 0, 1);
   hDist2AcceptedHit->SetOption("XY hist");
   Add(hDist2AcceptedHit);

   hDist2ClosestHit = new TH1I("hDist2ClosestHit", " ; Closest to Accepted Hits: Distance R, cm; Num. of Track Nodes; ", 100, 0, 1);
   hDist2ClosestHit->SetOption("XY hist");
   Add(hDist2ClosestHit);

   hPullClosestHit1D = new TH1I("hPullClosestHit1D", " ; Track Proj. to Closest Hit Pull Dist.: Distance R, #sigma-units; Num. of Track Nodes; ", 100, 0, 10);
   Add(hPullClosestHit1D);

   hPullClosestHit2D = new TH2I("hPullClosestHit2D", " ; Track Proj. to Closest Hit Pull Dist.: Local Z, #sigma-units; Local Y, #sigma-units; Num. of Track Nodes", 50, -6, 6, 50, -6, 6);
   hPullClosestHit2D->SetOption("colz");
   Add(hPullClosestHit2D);

   hPullCandidateHits2D = new TH2I("hPullCandidateHits2D", " ; Track Proj. to Candidate Hit Pull Dist.: Local Z, #sigma-units; Local Y, #sigma-units; Num. of Track Nodes", 50, -6, 6, 50, -6, 6);
   hPullCandidateHits2D->SetOption("colz");
   Add(hPullCandidateHits2D);

   hChi2CandidateHits = new TH1I("hChi2CandidateHits", " ; Track Proj. to Candidate Hit: #chi^{2}; Num. of Track Nodes", 100, 0, 20);
   Add(hChi2CandidateHits);

   hCountCandidateHits = new TH1I("hCountCandidateHits", " ; Num. of Candidate Hits; Num. of Track Nodes", 20, 0, 20);
   Add(hCountCandidateHits);

   hActiveLayerCounts = new TH2F("hActiveLayerCounts", " ; Track Local Z, cm; Local Y, cm; Num. of Track Nodes", n_z_bins, z_min, z_max, n_y_bins, y_min, y_max);
   hActiveLayerCounts->SetOption("colz");
   Add(hActiveLayerCounts);

   hProjErrorMag = new TH1I("hProjErrorMag", "Projection Error; Projection Error Mag",200,0.,1.0);
   Add(hProjErrorMag);
}


void StiHifyHistContainer::FillHists(const StiHifyEvent &event, StiNodeHitStatus hitStatus, bool onlyNodesWithCandidates)
{
   for (const auto& kalmTrack : event.GetTStiKalmanTracks())
   {
      for (const auto& trkNode : kalmTrack.GetNodes())
      {
         // Ignore nodes with 0 candidate hits when requested by user
         if ( onlyNodesWithCandidates && !trkNode.GetCandidateProxyHits().size() )
            continue;

         switch (hitStatus)
         {
         case StiNodeHitStatus::Any:
            FillHists(trkNode);
            break;
         case StiNodeHitStatus::Accepted:
            FillHistsHitsAccepted(trkNode);
            break;
         case StiNodeHitStatus::Rejected:
            FillHistsHitsRejected(trkNode);
            break;
         default:
            Error("FillHists", "Internal type of Sti hit assigned to this node "
                  "is not specified or implemented. Histograms won't be filled");
            break;
         }
      }
   }
}


/**
 * Creates X and Y projections from filled 2D histograms.
 */
void StiHifyHistContainer::FillDerivedHists()
{
   this->cd();

   Add( hPullCandidateHits2D->ProjectionX() );
   Add( hPullCandidateHits2D->ProjectionY() );

   Add( hActiveLayerCounts->ProjectionX() );
   Add( hActiveLayerCounts->ProjectionY() );
}


void StiHifyHistContainer::FillHists(const TStiKalmanTrackNode &trkNode)
{
   if ( trkNode.GetVolumeName().empty() )
      return;

   if ( !fPrgOptions.MatchedVolName(trkNode.GetVolumeName()) )
      return;

   // Start filling histograms
   hDiffProjToFitPositionWRTHit->Fill( trkNode.CalcDiffProjToFitPositionWRTHit() );
   hDiffProjToFitError->Fill( trkNode.CalcDiffProjToFitError().Z(), trkNode.CalcDiffProjToFitError().Y() );
   hDist2AcceptedHit->Fill( trkNode.CalcDistanceToHit() );
   hDist2ClosestHit->Fill( trkNode.CalcDistanceToClosestHit() );

   hPullClosestHit1D->Fill(trkNode.CalcDistanceToClosestHit() < 0 ? -1 : (trkNode.CalcDistanceToClosestHit()/trkNode.GetProjError().Mag()) );

   // Add by ZWM
   hProjErrorMag->Fill( trkNode.GetProjError().Mag() );

   const std::set<TStiHitProxy>& hitCandidates = trkNode.GetCandidateProxyHits();

   hCountCandidateHits->Fill(hitCandidates.size());

   // Consider the first candidate hit only. This histogram is used in hit
   // efficiency calculation
   bool foundClosestCandidate = false;

   for (const auto& hitCandidate : hitCandidates)
   {
      TVector3 pull = trkNode.CalcPullToHit( *hitCandidate.GetTStiHit() );

      hPullCandidateHits2D->Fill(pull.Z(), pull.Y());
      hChi2CandidateHits->Fill(hitCandidate.GetChi2());

      // Choose the first (i.e. the closest) candidate hit
      if (hitCandidate.GetDistanceToNode() >= 0 && !foundClosestCandidate)
      {
         hPullClosestHit2D->Fill(pull.Z(), pull.Y());
         foundClosestCandidate = true;
      }
   }

   hActiveLayerCounts->Fill(trkNode.GetPositionLocal().Z(), trkNode.GetPositionLocal().Y());

   // Fill individual histograms for each volume matching the regex only when
   // requested by the user
   if ( fPrgOptions.SplitHistsByVolume() )
   {
      std::string histName("hActiveLayerCounts_" + boost::replace_all_copy<string>(trkNode.GetVolumeName(), "/", "__"));

      TH1* hActiveLayerCounts_det = h(histName);

      if (!hActiveLayerCounts_det) {
         this->cd();
         hActiveLayerCounts_det = static_cast<TH1*>(hActiveLayerCounts->Clone());
         hActiveLayerCounts_det->SetName(histName.c_str());
         hActiveLayerCounts_det->SetOption("colz");
         Add(hActiveLayerCounts_det);
      }

      hActiveLayerCounts_det->Fill( trkNode.GetPositionLocal().Z(), trkNode.GetPositionLocal().Y() );
   }
}


void StiHifyHistContainer::FillHistsHitsAccepted(const TStiKalmanTrackNode &trkNode)
{
   if (!trkNode.GetHit())
      return;

   FillHists(trkNode);
}


void StiHifyHistContainer::FillHistsHitsRejected(const TStiKalmanTrackNode &trkNode)
{
   if (trkNode.GetHit())
      return;

   FillHists(trkNode);
}
