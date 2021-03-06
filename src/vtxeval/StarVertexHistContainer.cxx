#include "TH1I.h"
#include "TH2I.h"

#include "StarVertexHistContainer.h"

#include "StMuDSTMaker/COMMON/StMuPrimaryVertex.h"
#include "StMuDSTMaker/COMMON/StMuMcVertex.h"


StarVertexHistContainer::StarVertexHistContainer(const std::string name, TDirectory* motherDir, const std::string option) :
   tvx::HistContainer(name, motherDir, option)
{
   Add( new TH1I("hNumTracksToVertex", "Number of Tracks Assigned to Vertex", 50, 0, 50) );
   Add( new TH1F("hVertexRank", "Vertex Rank", 100, -100, 100) );

   Add( new TH1I("hVertexX", "; Reco Vertex X Position, cm; Counts; ", 50, -1, 1) );
   Add( new TH1I("hVertexY", "; Reco Vertex Y Position, cm; Counts; ", 50, -1, 1) );
   Add( new TH1I("hVertexZ", "; Reco Vertex Z Position, cm; Counts; ", 50, -30, 30) );

   Add( new TH2I("hVertexXvY", "; Reco Vertex X Position, cm; Reco Vertex Y Position, cm; Counts", 50, -1, 1, 50, -1, 1) );
   h("hVertexXvY")->SetOption("colz");
   Add( new TH2I("hVertexZvX", "; Reco Vertex Z Position, cm; Reco Vertex X Position, cm; Counts", 50, -30, 30, 50, -1, 1) );
   h("hVertexZvX")->SetOption("colz");
   Add( new TH2I("hVertexZvY", "; Reco Vertex Z Position, cm; Reco Vertex Y Position, cm; Counts", 50, -30, 30, 50, -1, 1) );
   h("hVertexZvY")->SetOption("colz");

   Add( new TH1I("hMcVertexX", "; MC Vertex X Position, cm; Counts; ", 50, -1, 1) );
   Add( new TH1I("hMcVertexY", "; MC Vertex Y Position, cm; Counts; ", 50, -1, 1) );
   Add( new TH1I("hMcVertexZ", "; MC Vertex Z Position, cm; Counts; ", 50, -30, 30) );

   Add( new TH2I("hMcVertexXvY", "; MC Vertex X Position, cm; Reco Vertex Y Position, cm; Counts", 50, -1, 1, 50, -1, 1) );
   h("hMcVertexXvY")->SetOption("colz");
   Add( new TH2I("hMcVertexZvX", "; MC Vertex Z Position, cm; Reco Vertex X Position, cm; Counts", 50, -30, 30, 50, -1, 1) );
   h("hMcVertexZvX")->SetOption("colz");
   Add( new TH2I("hMcVertexZvY", "; MC Vertex Z Position, cm; Reco Vertex Y Position, cm; Counts", 50, -30, 30, 50, -1, 1) );
   h("hMcVertexZvY")->SetOption("colz");

   Add( new TH1I("hMcRecoVertexDeltaX", "; MC - Reco Vertex X, cm; Counts; ", 50, -0.3, 0.3) );
   Add( new TH1I("hMcRecoVertexDeltaY", "; MC - Reco Vertex Y, cm; Counts; ", 50, -0.3, 0.3) );
   Add( new TH1I("hMcRecoVertexDeltaZ", "; MC - Reco Vertex Z, cm; Counts; ", 50, -0.3, 0.3) );

   Add( new TH1I("hMcRecoVertexDelta", "; MC - Reco Vertex Dist, cm; Counts; ", 50, 0, 0.5) );

   TH1* h = new TH1I("hVertexErrorMag", "Vertex Position Error Magnitude; sqrt(#sigma_{x}^{2} + #sigma_{y}^{2} + #sigma_{z}^{2}), cm", 50, 0, 1);
   Add(h);

   Add( new TH1I("hVertexErrorX", "; Reco Vertex X Position Error, cm; Counts; ", 50, 0, 0.35) );
   Add( new TH1I("hVertexErrorY", "; Reco Vertex Y Position Error, cm; Counts; ", 50, 0, 0.35) );
   Add( new TH1I("hVertexErrorZ", "; Reco Vertex Z Position Error, cm; Counts; ", 50, 0, 0.50) );

   // The following histograms require info from simulated vertex
   Add( new TH1I("hVertexPullX", "; Vertex Pull Distribution in X; Counts; ", 50, -5, 5) );
   Add( new TH1I("hVertexPullY", "; Vertex Pull Distribution in Y; Counts; ", 50, -5, 5) );
   Add( new TH1I("hVertexPullZ", "; Vertex Pull Distribution in Z; Counts; ", 50, -5, 5) );
}


void StarVertexHistContainer::FillHists(const StMuPrimaryVertex &vertex, const StMuMcVertex* mcVertex)
{
   h("hNumTracksToVertex")->Fill(vertex.noTracks());
   h("hVertexRank")->Fill(vertex.ranking());

   h("hVertexX")->Fill(vertex.position().x());
   h("hVertexY")->Fill(vertex.position().y());
   h("hVertexZ")->Fill(vertex.position().z());

   h("hVertexXvY")->Fill(vertex.position().x(), vertex.position().y());
   h("hVertexZvX")->Fill(vertex.position().z(), vertex.position().x());
   h("hVertexZvY")->Fill(vertex.position().z(), vertex.position().y());

   h("hVertexErrorMag")->Fill(vertex.posError().mag());
   h("hVertexErrorX")->Fill(vertex.posError().x());
   h("hVertexErrorY")->Fill(vertex.posError().y());
   h("hVertexErrorZ")->Fill(vertex.posError().z());

   // Proceed with histogram requiring info about simulated vertex
   if (!mcVertex) return;

   h("hMcVertexX")->Fill(mcVertex->XyzV().x());
   h("hMcVertexY")->Fill(mcVertex->XyzV().y());
   h("hMcVertexZ")->Fill(mcVertex->XyzV().z());

   h("hMcVertexXvY")->Fill(mcVertex->XyzV().x(), mcVertex->XyzV().y());
   h("hMcVertexZvX")->Fill(mcVertex->XyzV().z(), mcVertex->XyzV().x());
   h("hMcVertexZvY")->Fill(mcVertex->XyzV().z(), mcVertex->XyzV().y());

   h("hMcRecoVertexDeltaX")->Fill(mcVertex->XyzV().x() - vertex.position().x());
   h("hMcRecoVertexDeltaY")->Fill(mcVertex->XyzV().y() - vertex.position().y());
   h("hMcRecoVertexDeltaZ")->Fill(mcVertex->XyzV().z() - vertex.position().z());

   h("hMcRecoVertexDelta")->Fill( (mcVertex->XyzV() - vertex.position()).mag() );

   h("hVertexPullX")->Fill( (vertex.position().x() - mcVertex->XyzV().x())/vertex.posError().x() );
   h("hVertexPullY")->Fill( (vertex.position().y() - mcVertex->XyzV().y())/vertex.posError().y() );
   h("hVertexPullZ")->Fill( (vertex.position().z() - mcVertex->XyzV().z())/vertex.posError().z() );
}
