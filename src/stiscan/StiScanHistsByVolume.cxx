#include <cmath>

#include "TProfile2D.h"

#include "stiscan/StiScanHistsByVolume.h"
#include "stirootio/TStiKalmanTrackNode.h"


StiScanHistsByVolume::StiScanHistsByVolume(StiScanPrgOptions& prgOpts, const char* name, TDirectory* motherDir, bool doProjection, Option_t* option) :
   StiScanHistContainer(prgOpts, name, motherDir, doProjection, option)
{
}


void StiScanHistsByVolume::FillHists(const TStiKalmanTrack &kalmTrack, const std::set<std::string> *volumeList)
{
   // Take the first node with the smallest radius
   const TStiKalmanTrackNode& dcaNode = kalmTrack.GetDcaNode();

   h("hTrackCountVsEtaVsPhi")->Fill(dcaNode.GetTrackP().Eta(), dcaNode.GetTrackP().Phi());
   h("hTrackCountVsZVsPhi")->Fill(dcaNode.GetPosition().Z(), dcaNode.GetTrackP().Phi());

   for (const auto& kalmNode : kalmTrack.GetNodes())
   {
      if (volumeList && volumeList->size() && !kalmNode.MatchedVolName(*volumeList) ) continue;

      hNStepsVsPhiVsRVsZ->FillAsCumulative(kalmNode.GetNodeCenterRefAngle(), kalmNode.GetNodeRadius(), kalmNode.GetPosition().Z(), 1);
      dynamic_cast<TProfile2D&>( *h("hELossVsEtaVsPhi_trk")).Fill(kalmNode.GetTrackP().Eta(), kalmNode.GetTrackP().Phi(), kalmNode.GetEnergyLosses());

      dynamic_cast<TProfile2D&>( *h("hELossVsEtaVsPhi")).Fill(kalmNode.GetPosition().Eta(), kalmNode.GetNodeCenterRefAngle(), kalmNode.GetEnergyLosses());
      dynamic_cast<TProfile2D&>( *h("hELossVsZVsPhi")).Fill(kalmNode.GetPosition().Z(), kalmNode.GetNodeCenterRefAngle(), kalmNode.GetEnergyLosses());
      dynamic_cast<TProfile2D&>( *h("hELossVsZVsR")).Fill(kalmNode.GetPosition().Z(), kalmNode.GetNodeRadius(), kalmNode.GetEnergyLosses());
      dynamic_cast<TProfile2D&>( *h("hELossVsPhiVsR")).Fill(kalmNode.GetNodeCenterRefAngle(), kalmNode.GetNodeRadius(), kalmNode.GetEnergyLosses());
      hELossVsPhiVsRVsZ->FillAsCumulative(kalmNode.GetNodeCenterRefAngle(), kalmNode.GetNodeRadius(), kalmNode.GetPosition().Z(), kalmNode.GetEnergyLosses());
      hELossVsXVsYVsZ->FillAsCumulative(kalmNode.GetNodeCenterRefAngle(), kalmNode.GetNodeRadius(), kalmNode.GetPosition().Z(), kalmNode.GetEnergyLosses());
      hDensityVsPhiVsRVsZ->Fill(kalmNode.GetNodeCenterRefAngle(), kalmNode.GetNodeRadius(), kalmNode.GetPosition().Z(), kalmNode.GetNodeMaterialDensity());
      hRelRadLengthVsPhiVsRVsZ->FillAsCumulative(kalmNode.GetNodeCenterRefAngle(), kalmNode.GetNodeRadius(), kalmNode.GetPosition().Z(), kalmNode.GetNodeRelRadLength());
   }

   hNStepsVsPhiVsRVsZ->ResetBinCumulMode();
   hELossVsPhiVsRVsZ->ResetBinCumulMode();
   hELossVsXVsYVsZ->ResetBinCumulMode();
   hRelRadLengthVsPhiVsRVsZ->ResetBinCumulMode();
}
