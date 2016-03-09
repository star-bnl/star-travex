#include "TH1I.h"
#include "TH2I.h"

#include "StarVertexHistContainer.h"


StarVertexHistContainer::StarVertexHistContainer(const std::string name, TDirectory* motherDir, const std::string option) :
   tvx::HistContainer(name, motherDir, option)
{
   fHs["hNumTracksToVertex"].reset( new TH1I("hNumTracksToVertex", "Number of Tracks Assigned to Vertex", 50, 0, 50) );
   fHs["hVertexRank"].reset( new TH1F("hVertexRank", "Vertex Rank", 100, -100, 100) );
   fHs["hVertexX"].reset( new TH1I("hVertexX", "; Reco Vertex X Position, cm; Counts; ", 50, -1, 1) );
   fHs["hVertexY"].reset( new TH1I("hVertexY", "; Reco Vertex Y Position, cm; Counts; ", 50, -1, 1) );
   fHs["hVertexZ"].reset( new TH1I("hVertexZ", "; Reco Vertex Z Position, cm; Counts; ", 50, -30, 30) );

   fHs["hVertexXvY"].reset( new TH2I("hVertexXvY", "; Reco Vertex X Position, cm; Reco Vertex Y Position, cm; Counts", 50, -1, 1, 50, -1, 1) );
   fHs["hVertexXvY"]->SetOption("colz");
   fHs["hVertexZvX"].reset( new TH2I("hVertexZvX", "; Reco Vertex Z Position, cm; Reco Vertex X Position, cm; Counts", 50, -30, 30, 50, -1, 1) );
   fHs["hVertexZvX"]->SetOption("colz");
   fHs["hVertexZvY"].reset( new TH2I("hVertexZvY", "; Reco Vertex Z Position, cm; Reco Vertex Y Position, cm; Counts", 50, -30, 30, 50, -1, 1) );
   fHs["hVertexZvY"]->SetOption("colz");

   fHs["hMcVertexX"].reset( new TH1I("hMcVertexX", "; MC Vertex X Position, cm; Counts; ", 50, -1, 1) );
   fHs["hMcVertexY"].reset( new TH1I("hMcVertexY", "; MC Vertex Y Position, cm; Counts; ", 50, -1, 1) );
   fHs["hMcVertexZ"].reset( new TH1I("hMcVertexZ", "; MC Vertex Z Position, cm; Counts; ", 50, -30, 30) );

   fHs["hMcVertexXvY"].reset( new TH2I("hMcVertexXvY", "; MC Vertex X Position, cm; Reco Vertex Y Position, cm; Counts", 50, -1, 1, 50, -1, 1) );
   fHs["hMcVertexXvY"]->SetOption("colz");
   fHs["hMcVertexZvX"].reset( new TH2I("hMcVertexZvX", "; MC Vertex Z Position, cm; Reco Vertex X Position, cm; Counts", 50, -30, 30, 50, -1, 1) );
   fHs["hMcVertexZvX"]->SetOption("colz");
   fHs["hMcVertexZvY"].reset( new TH2I("hMcVertexZvY", "; MC Vertex Z Position, cm; Reco Vertex Y Position, cm; Counts", 50, -30, 30, 50, -1, 1) );
   fHs["hMcVertexZvY"]->SetOption("colz");

   fHs["hMcRecoVertexDeltaX"].reset( new TH1I("hMcRecoVertexDeltaX", "; MC - Reco Vertex X, cm; Counts; ", 50, -0.3, 0.3) );
   fHs["hMcRecoVertexDeltaY"].reset( new TH1I("hMcRecoVertexDeltaY", "; MC - Reco Vertex Y, cm; Counts; ", 50, -0.3, 0.3) );
   fHs["hMcRecoVertexDeltaZ"].reset( new TH1I("hMcRecoVertexDeltaZ", "; MC - Reco Vertex Z, cm; Counts; ", 50, -0.3, 0.3) );

   fHs["hMcRecoVertexDelta"].reset( new TH1I("hMcRecoVertexDelta", "; MC - Reco Vertex Dist, cm; Counts; ", 50, 0, 0.5) );

   TH1* h = new TH1I("hVertexErrorMag", "Vertex Position Error Magnitude; sqrt(#sigma_{x}^{2} + #sigma_{y}^{2} + #sigma_{z}^{2}), cm", 50, 0, 1);
   fHs["hVertexErrorMag"].reset(h);
   fHs["hVertexErrorX"].reset( new TH1I("hVertexErrorX", "; Reco Vertex X Position Error, cm; Counts; ", 50, 0, 0.35) );
   fHs["hVertexErrorY"].reset( new TH1I("hVertexErrorY", "; Reco Vertex Y Position Error, cm; Counts; ", 50, 0, 0.35) );
   fHs["hVertexErrorZ"].reset( new TH1I("hVertexErrorZ", "; Reco Vertex Z Position Error, cm; Counts; ", 50, 0, 0.50) );

   // The following histograms require info from simulated vertex
   fHs["hVertexPullX"].reset( new TH1I("hVertexPullX", "; Vertex Pull Distribution in X; Counts; ", 50, -5, 5) );
   fHs["hVertexPullY"].reset( new TH1I("hVertexPullY", "; Vertex Pull Distribution in Y; Counts; ", 50, -5, 5) );
   fHs["hVertexPullZ"].reset( new TH1I("hVertexPullZ", "; Vertex Pull Distribution in Z; Counts; ", 50, -5, 5) );
}


void StarVertexHistContainer::FillHists(const StMuPrimaryVertex &vertex, const StMuMcVertex* mcVertex)
{
   fHs["hNumTracksToVertex"]->Fill(vertex.noTracks());
   fHs["hVertexRank"]->Fill(vertex.ranking());
   fHs["hVertexX"]->Fill(vertex.position().x());
   fHs["hVertexY"]->Fill(vertex.position().y());
   fHs["hVertexZ"]->Fill(vertex.position().z());

   fHs["hVertexXvY"]->Fill(vertex.position().x(), vertex.position().y());
   fHs["hVertexZvX"]->Fill(vertex.position().z(), vertex.position().x());
   fHs["hVertexZvY"]->Fill(vertex.position().z(), vertex.position().y());

   fHs["hVertexErrorMag"]->Fill(vertex.posError().mag());
   fHs["hVertexErrorX"]->Fill(vertex.posError().x());
   fHs["hVertexErrorY"]->Fill(vertex.posError().y());
   fHs["hVertexErrorZ"]->Fill(vertex.posError().z());

   // Proceed with histogram requiring info about simulated vertex
   if (!mcVertex) return;

   fHs["hMcVertexX"]->Fill(mcVertex->XyzV().x());
   fHs["hMcVertexY"]->Fill(mcVertex->XyzV().y());
   fHs["hMcVertexZ"]->Fill(mcVertex->XyzV().z());

   fHs["hMcVertexXvY"]->Fill(mcVertex->XyzV().x(), mcVertex->XyzV().y());
   fHs["hMcVertexZvX"]->Fill(mcVertex->XyzV().z(), mcVertex->XyzV().x());
   fHs["hMcVertexZvY"]->Fill(mcVertex->XyzV().z(), mcVertex->XyzV().y());

   fHs["hMcRecoVertexDeltaX"]->Fill(mcVertex->XyzV().x() - vertex.position().x());
   fHs["hMcRecoVertexDeltaY"]->Fill(mcVertex->XyzV().y() - vertex.position().y());
   fHs["hMcRecoVertexDeltaZ"]->Fill(mcVertex->XyzV().z() - vertex.position().z());

   fHs["hMcRecoVertexDelta"]->Fill( (mcVertex->XyzV() - vertex.position()).mag() );

   fHs["hVertexPullX"]->Fill( (vertex.position().x() - mcVertex->XyzV().x())/vertex.posError().x() );
   fHs["hVertexPullY"]->Fill( (vertex.position().y() - mcVertex->XyzV().y())/vertex.posError().y() );
   fHs["hVertexPullZ"]->Fill( (vertex.position().z() - mcVertex->XyzV().z())/vertex.posError().z() );
}
