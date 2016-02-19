#include <cmath>
#include <boost/algorithm/string/replace.hpp>

#include "TVector3.h"

#include "StiHify/StiHifyAnalysisTreeMaker.h"
#include "StiRootIO/TStiHitProxy.h"

StiHifyAnalysisTreeMaker::StiHifyAnalysisTreeMaker()
{
  createTree();
  clearTreeStructs(); // prep the structures for running
}


void StiHifyAnalysisTreeMaker::createTree()
{
  aTree = new TTree("pullAnaTree","Tree for Pull Error Analysis");
  aTree->Branch("AnyHit",&errorInfo.errorMag,"errorMag/F:pull/F:residual/F:eta/F:pT/F:phi/F:z/F");
  aTree->Branch("AcceptedHit",&errorInfoAcc.errorMag,"errorMag/F:pull/F:residual/F:eta/F:pT/F:phi/F:z/F");
  aTree->Branch("RejectedHit",&errorInfoRej.errorMag,"errorMag/F:pull/F:residual/F:eta/F:pT/F:phi/F:z/F");
}


void StiHifyAnalysisTreeMaker::FillTree(const StiHifyEvent &event, StiNodeHitStatus hitStatus, const std::set<std::string> *volumeList)
{
  for (const auto& kalmTrack : event.GetTStiKalmanTracks())
  {
    for (const auto& trkNode : kalmTrack.GetNodes())
    {
      switch (hitStatus)
      {
        case StiNodeHitStatus::Any:
          FillTree(trkNode, volumeList, errorInfo);
          break;
        case StiNodeHitStatus::Accepted:
          if(trkNode.GetHit())
            FillTree(trkNode, volumeList, errorInfoAcc);
          break;
        case StiNodeHitStatus::Rejected:
          if(!trkNode.GetHit())
            FillTree(trkNode, volumeList, errorInfoRej);
          break;
        default:
          Error("FillHists", "Internal type of Sti hit assigned to this node is not specified. "
              "Histograms won't be filled");
          break;
      }
    }
  }
}

void StiHifyAnalysisTreeMaker::FillTree(const TStiKalmanTrackNode &trkNode, const std::set<std::string> *volumeList, errorInfo_t &eI )
{
  if (volumeList && volumeList->size() && !trkNode.MatchedVolName(*volumeList) )
    return;

  if (trkNode.GetVolumeName().empty() || !trkNode.IsInsideVolume())
    return;

  // Set variables needed by tree
  eI.errorMag = trkNode.GetProjError().Mag();
  eI.residual = trkNode.CalcDistanceToClosestHit();
  eI.pull     = eI.residual/eI.errorMag;
  eI.eta      = trkNode.GetTrackP().PseudoRapidity();
  eI.pT       = trkNode.GetTrackP().Perp();
  eI.phi      = trkNode.GetTrackP().Phi();
  eI.z        = trkNode.GetTrackP().z();

  aTree->Fill();
  clearTreeStructs();

}

void StiHifyAnalysisTreeMaker::writeTree()
{
  aTree->Write();
}

void StiHifyAnalysisTreeMaker::clearTreeStructs()
{
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
