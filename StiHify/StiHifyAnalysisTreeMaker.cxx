#include "StiHify/StiHifyAnalysisTreeMaker.h"


StiHifyAnalysisTreeMaker::StiHifyAnalysisTreeMaker(const StiHifyPrgOptions& prgOpts) :fPrgOptions(prgOpts)
{
  createTree();
  clearTreeStructs(); // prep the structures for running
}


void StiHifyAnalysisTreeMaker::createTree()
{
  // This creates the tree object, than populates 3 different branches, each with the same leaves
  // Each of the branches corresponds to either Any hit, a hit accepted by Sti, or a hit rejected by Sti
  aTree = new TTree("pullAnaTree","Tree for Pull Error Analysis");
  aTree->Branch("AnyHit",&errorInfo.errorMag,"errorMag/F:pull/F:residual/F:eta/F:pT/F:phi/F:z/F");
  aTree->Branch("AcceptedHit",&errorInfoAcc.errorMag,"errorMag/F:pull/F:residual/F:eta/F:pT/F:phi/F:z/F");
  aTree->Branch("RejectedHit",&errorInfoRej.errorMag,"errorMag/F:pull/F:residual/F:eta/F:pT/F:phi/F:z/F");
}


void StiHifyAnalysisTreeMaker::FillTree(const StiHifyEvent &event, StiNodeHitStatus hitStatus, bool onlyNodesWithCandidates) 
{
  for (const auto& kalmTrack : event.GetTStiKalmanTracks()) // Loop over tracks in event
  {
    for (const auto& trkNode : kalmTrack.GetNodes()) // Loop over nodes in track
    {
      // Ignore nodes with 0 candidate hits when requested by user
      if ( onlyNodesWithCandidates && !trkNode.GetCandidateProxyHits().size() )
        continue;

      switch (hitStatus) // Decide what type of nodes you want to look at in FillTree iteration
      {
        case StiNodeHitStatus::Any:
          FillTree(trkNode, errorInfo);
          break;
        case StiNodeHitStatus::Accepted:
          if(trkNode.GetHit()) // Check if accepted
            FillTree(trkNode, errorInfoAcc);
          break;
        case StiNodeHitStatus::Rejected:
          if(!trkNode.GetHit()) //Check if not accepted
            FillTree(trkNode, errorInfoRej);
          break;
        default:
          Error("FillHists", "Internal type of Sti hit assigned to this node is not specified. "
              "Histograms won't be filled");
          break;
      }
    }
  }
}

void StiHifyAnalysisTreeMaker::FillTree(const TStiKalmanTrackNode &trkNode, errorInfo_t &eI )
{
  if ( trkNode.GetVolumeName().empty() )
    return;

  if (!fPrgOptions.MatchedVolName(trkNode.GetVolumeName()) ) // Limit to only volumes in volumeList
    return;

  // Set variables needed by tree in the structure. All others are -999.
  eI.errorMag = trkNode.GetProjError().Mag();
  eI.residual = trkNode.CalcDistanceToClosestHit();
  eI.pull     = eI.residual/eI.errorMag;
  eI.eta      = trkNode.GetTrackP().PseudoRapidity();
  eI.pT       = trkNode.GetTrackP().Perp();
  eI.phi      = trkNode.GetTrackP().Phi();
  eI.z        = trkNode.GetTrackP().z();

  aTree->Fill(); // push structures to tree
  clearTreeStructs(); // reset structure for next node

}

void StiHifyAnalysisTreeMaker::writeTree()
{
  aTree->Write();
}

void StiHifyAnalysisTreeMaker::clearTreeStructs()
{
  // Set all values to default for next node
  errorInfoRej.errorMag = -999;
  errorInfoRej.residual = -999;
  errorInfoRej.pull     = -999;
  errorInfoRej.eta      = -999;
  errorInfoRej.pT       = -999;
  errorInfoRej.phi      = -999;
  errorInfoRej.z        = -999;
  errorInfo.errorMag =-999;
  errorInfo.residual = -999;
  errorInfo.pull     = -999;
  errorInfo.eta      = -999;
  errorInfo.pT       = -999;
  errorInfo.phi      = -999;
  errorInfo.z        = -999;
  errorInfoAcc.errorMag =-999;
  errorInfoAcc.residual = -999;
  errorInfoAcc.pull     = -999;
  errorInfoAcc.eta      = -999;
  errorInfoAcc.pT       = -999;
  errorInfoAcc.phi      = -999;
  errorInfoAcc.z        = -999;
}
