#include <algorithm>

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
