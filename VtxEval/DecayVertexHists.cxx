#include "TH1I.h"

#include "VtxEval/DecayVertexHists.h"

#include "StMuDSTMaker/COMMON/StMuPrimaryVertex.h"
#include "StMuDSTMaker/COMMON/StMuMcVertex.h"


DecayVertexHists::DecayVertexHists(const std::string name, TDirectory* motherDir, const std::string option) :
   tvx::HistContainer(name, motherDir, option)
{
   Add( new TH1I("hInvMass", "; Invariant Mass, GeV; Counts; ", 200, 0.4, 1.2) );
   Add( new TH1I("hDecayLength", "; Decay Length, cm; Counts; ", 50, 0, 50) );
   Add( new TH1I("hDLSignificance", "; Decay Length Significance, cm; Counts; ", 50, 0, 50) );
}


void DecayVertexHists::FillHists(const StMuPrimaryVertex &vertex, const TDecayVertex& decayVtx)
{
   h("hInvMass")->Fill( decayVtx.im_p );
   h("hDecayLength")->Fill( decayVtx.dl_p );
   double expected_decay_vertex_err2 = 0.15*0.15;
   h("hDLSignificance")->Fill( decayVtx.dl_p / sqrt(vertex.posError().mag()*vertex.posError().mag() + expected_decay_vertex_err2) );
}
