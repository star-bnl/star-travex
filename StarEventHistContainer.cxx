#include <algorithm>

#include <TEfficiency.h>

#include "StarEventHistContainer.h"

#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuMcTrack.h"
#include "StMuDSTMaker/COMMON/StMuMcVertex.h"
#include "StMuDSTMaker/COMMON/StMuPrimaryVertex.h"

#include "utils.h"


StarEventHistContainer::StarEventHistContainer(const std::string name, TDirectory* motherDir, const std::string option) :
   tvx::HistContainer(name, motherDir, option)
{
   Add(new TH1I("hNumVertices", "; Number of Vertices; Counts; ", 10, 0, 10));
   Add(new TH1I("hNumTracksPerEvent", "; Number of Tracks per Event; Counts", 100, 0, 100));
   Add(new TH1I("hNumTracksPerEventPrimary", "; Number of Primary Tracks per Event; Counts", 100, 0, 100));

   // Vertex efficiency histograms
   Add(new TH1I("McRecMulT", "; Reconstructable multiplicity for trigger Mc Vertex; Num. of Reco Vertices; ", 50, 0, 500) );
   Add(new TH1I("McRecMulAny", "; MC Vertex Multiplicity; Num. of Reco Vertices matching MC (idTruth); ", 50, 0, 500) );
   Add(new TH1I("McRecMulGood", "; MC Vertex Multiplicity; Num. of Max Rank Reco Vertices matching MC (idTruth); ", 50, 0, 500) );
}


void StarEventHistContainer::FillHists(const StMuDst &event)
{
   h("hNumVertices")->Fill( event.primaryVertices()->GetEntriesFast() );
   h("hNumTracksPerEvent")->Fill( event.globalTracks()->GetEntriesFast() );
   h("hNumTracksPerEventPrimary")->Fill( event.primaryTracks()->GetEntriesFast() );
}


void StarEventHistContainer::FillEfficyHists(const StMuDst &event, const StMuMcVertex &mcVertex,
   const StMuPrimaryVertex *recoVertex, const StMuPrimaryVertex *recoVertexMaxRank)
{
   vtxeval::VectorMcTracks mcTracks = vtxeval::getMcTracksMatchingMcVertex(event, &mcVertex);

   // This is the "reconstructible" track multiplicity
   auto isReconstructible = [](const StMuMcTrack* mcTrack) { return mcTrack->No_tpc_hit() >= 15; };
   int nRecoTracks = std::count_if(mcTracks.begin(), mcTracks.end(), isReconstructible);

   // This is our denominator histogram for efficiencies
   h("McRecMulT")->Fill(nRecoTracks);

   if (recoVertex) {
      h("McRecMulAny")->Fill(nRecoTracks);

      if (recoVertex == recoVertexMaxRank)
         h("McRecMulGood")->Fill(nRecoTracks);
   }
}


void StarEventHistContainer::FillDerivedHists()
{
   // Calculate efficiency for any reco vertex matching simulated vertex
   TEfficiency *eff_any = new TEfficiency(*h("McRecMulAny"), *h("McRecMulT"));

   std::string eff_title = std::string(";") + h("McRecMulAny")->GetXaxis()->GetTitle() + "; Efficiency";

   eff_any->SetTitle(eff_title.c_str() );
   eff_any->SetMarkerColor(kBlack);
   eff_any->SetMarkerStyle(kFullCross);

   TH1 *eff_hist = static_cast<TH1*>( h("McRecMulAny")->Clone() );
   eff_hist->Reset("ICES");
   eff_hist->SetName( (std::string(eff_hist->GetName()) + "_eff").c_str() );
   eff_hist->SetYTitle("Efficiency");
   eff_hist->GetListOfFunctions()->Add(eff_any);

   Add(eff_hist);


   // Calculate efficiency for max rank reco vertex matching simulated vertex
   TEfficiency *eff_good = new TEfficiency(*h("McRecMulGood"), *h("McRecMulT"));

   eff_title = std::string(";") + h("McRecMulGood")->GetXaxis()->GetTitle() + "; Efficiency";

   eff_good->SetTitle(eff_title.c_str() );
   eff_good->SetMarkerColor(kRed);
   eff_good->SetMarkerStyle(kFullCross);

   eff_hist = static_cast<TH1*>( h("McRecMulGood")->Clone() );
   eff_hist->Reset("ICES");
   eff_hist->SetName( (std::string(eff_hist->GetName()) + "_eff").c_str() );
   eff_hist->SetYTitle("Efficiency");
   eff_hist->GetListOfFunctions()->Add(eff_good);

   Add(eff_hist);
}
