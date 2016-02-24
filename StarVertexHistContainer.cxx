#include "StarVertexHistContainer.h"


StarVertexHistContainer::StarVertexHistContainer(const std::string name, TDirectory* motherDir, const std::string option) :
   tvx::HistContainer(name, motherDir, option)
{

   fHs["hNumTracksToVertex"].reset( new TH1I("hNumTracksToVertex", "Number of Tracks Assigned to Vertex", 100, 0, 100) );
   fHs["hVertexRank"].reset( new TH1F("hVertexRank", "Vertex Rank", 100, -100, 100) );
   fHs["hVertexX"].reset( new TH1I("hVertexX", "Vertex X Position", 60, -10, 10) );
   fHs["hVertexY"].reset( new TH1I("hVertexY", "Vertex Y Position", 60, -10, 10) );
   fHs["hVertexZ"].reset( new TH1I("hVertexZ", "Vertex Z Position", 60, -30, 30) );

   TH1* h = new TH1I("hVertexErrorMag", "Vertex Position Error Magnitude; sqrt(#sigma_{x}^{2} + #sigma_{y}^{2} + #sigma_{z}^{2}), cm", 50, 0, 1);
   fHs["hVertexErrorMag"].reset(h);
}


void StarVertexHistContainer::FillHists(const StMuPrimaryVertex &vertex)
{
   fHs["hNumTracksToVertex"]->Fill(vertex.noTracks());
   fHs["hVertexRank"]->Fill(vertex.ranking());
   fHs["hVertexX"]->Fill(vertex.position().x());
   fHs["hVertexY"]->Fill(vertex.position().y());
   fHs["hVertexZ"]->Fill(vertex.position().z());
   fHs["hVertexErrorMag"]->Fill(vertex.posError().mag());
}
