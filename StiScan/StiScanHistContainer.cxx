#include <cmath>

#include "TH2S.h"
#include "TProfile2D.h"
#include "TVector3.h"

#include "StiScan/StiScanHistContainer.h"
#include "StiRootIO/TStiKalmanTrackNode.h"


StiScanHistContainer::StiScanHistContainer(StiScanPrgOptions& prgOpts, const char* name, TDirectory* motherDir, bool doProjection, Option_t* option) :
   tvx::HistContainer(name, motherDir, option),
   fPrgOptions(prgOpts),
   mNodeZMin(-250), mNodeZMax(250),
   mNodeRMin(0), mNodeRMax(30),
   mDoProjection(doProjection),
   hNStepsVsPhiVsRVsZ(nullptr),
   hELossVsPhiVsRVsZ(nullptr),
   hELossVsXVsYVsZ(nullptr),
   hDensityVsPhiVsRVsZ(nullptr),
   hRelRadLengthVsPhiVsRVsZ(nullptr)
{
   InitRange();
   BookHists();
}


/** The default limits will be used if user provided values for min >= max. */
void StiScanHistContainer::InitRange()
{
   if (fPrgOptions.GetHistZMin() < fPrgOptions.GetHistZMax() ) {
      mNodeZMin = fPrgOptions.GetHistZMin();
      mNodeZMax = fPrgOptions.GetHistZMax();
   }

   if (fPrgOptions.GetHistRMin() < fPrgOptions.GetHistRMax() ) {
      mNodeRMin = fPrgOptions.GetHistRMin();
      mNodeRMax = fPrgOptions.GetHistRMax();
   }
}


void StiScanHistContainer::BookHists()
{
   const double minZBinWidth = 1;   // desired bin width in cm
   const double minRBinWidth = 0.2; // desired bin width in cm

   int nZBins = ceil( (mNodeZMax - mNodeZMin) / minZBinWidth );
   int nRBins = ceil( (mNodeRMax - mNodeRMin) / minRBinWidth );

   nRBins = nRBins > 1000 ? 1000 : nRBins;

   int nXYBins = nRBins*floor(mNodeRMax/(mNodeRMax - mNodeRMin));

   nZBins = nZBins > 500 ? 500 : nZBins;

   this->cd();

   TH1* h;

   h = new TH2S("hTrackCountVsEtaVsPhi", " ; #eta; #phi, rad; Num. of Tracks", 50, -2, 2, 120, -M_PI, M_PI);
   h->SetOption("colz");
   Add(h);

   h = new TH2S("hTrackCountVsZVsPhi", " ; z, cm; #phi, rad; Num. of Tracks", nZBins, mNodeZMin, mNodeZMax, 120, -M_PI, M_PI);
   h->SetOption("colz");
   Add(h);

   h = new TProfile2D("hELossVsEtaVsPhi_trk", " ; Track #eta; Track #phi, rad; Energy Losses in Select Volumes, keV", 50, -2, 2, 120, -M_PI, M_PI);
   h->SetOption("colz");
   Add(h);

   h = new TProfile2D("hELossVsEtaVsPhi", " ; #eta; #phi, rad; Energy Losses in Select Volumes, keV", 50, -2, 2, 120, -M_PI, M_PI);
   h->SetOption("colz");
   Add(h);

   h = new TProfile2D("hELossVsZVsPhi", " ; z, cm; #phi, rad; Energy Losses in Select Volumes, keV", nZBins, mNodeZMin, mNodeZMax, 120, -M_PI, M_PI);
   h->SetOption("colz");
   Add(h);

   h = new TProfile2D("hELossVsZVsR", " ; z, cm; r, cm; Energy Losses in Select Volumes, keV", nZBins, mNodeZMin, mNodeZMax, nRBins, mNodeRMin, mNodeRMax);
   h->SetOption("colz");
   Add(h);

   h = new Profile2D("hELossVsPhiVsR", " ; #phi, rad; r, cm; Energy Losses in Select Volumes, keV", 120, -M_PI, M_PI, nRBins, mNodeRMin, mNodeRMax, "colz");
   Add(h);

   hNStepsVsPhiVsRVsZ = new Profile3D("hNStepsVsPhiVsRVsZ", "Num. of Steps per Track ; #phi, rad; r, cm; z, cm", 120, -M_PI, M_PI, nRBins, mNodeRMin, mNodeRMax, nZBins, mNodeZMin, mNodeZMax);
   Add(hNStepsVsPhiVsRVsZ);

   hELossVsPhiVsRVsZ = new Profile3D("hELossVsPhiVsRVsZ", "Energy Losses in Select Volumes, keV ; #phi, rad; r, cm; z, cm", 120, -M_PI, M_PI, nRBins, mNodeRMin, mNodeRMax, nZBins, mNodeZMin, mNodeZMax);
   Add(hELossVsPhiVsRVsZ);

   hELossVsXVsYVsZ = new Profile3D("hELossVsXVsYVsZ", "Energy Losses in Select Volumes, keV ; x, cm; y, cm; z, cm", nXYBins, -mNodeRMax, mNodeRMax, nXYBins, -mNodeRMax, mNodeRMax, nZBins, mNodeZMin, mNodeZMax);
   Add(hELossVsXVsYVsZ);

   hDensityVsPhiVsRVsZ = new Profile3D("hDensityVsPhiVsRVsZ", "Material Density, g/cm^{3}; #phi, rad; r, cm; z, cm", 120, -M_PI, M_PI, nRBins, mNodeRMin, mNodeRMax, nZBins, mNodeZMin, mNodeZMax);
   Add(hDensityVsPhiVsRVsZ);

   hRelRadLengthVsPhiVsRVsZ = new Profile3D("hRelRadLengthVsPhiVsRVsZ", "Rel. Radiation Length, %; #phi, rad; r, cm; z, cm", 120, -M_PI, M_PI, nRBins, mNodeRMin, mNodeRMax, nZBins, mNodeZMin, mNodeZMax);
   Add(hRelRadLengthVsPhiVsRVsZ);
}


void StiScanHistContainer::FillHists(const StiScanEvent &eventT, const std::set<std::string> *volumeList)
{
   for (const auto& kalmTrack : eventT.GetTStiKalmanTracks())
   {
      FillHists(kalmTrack, volumeList);
   }
}


void StiScanHistContainer::FillHists(const TGeaEvent &eventG, const std::set<std::string> *volumeList)
{
   TIter iGeaTrack(eventG.tracks);

   while (TGeaTrack* trackG = static_cast<TGeaTrack*>(iGeaTrack()) )
   {
      FillHists(*trackG, volumeList);
   }
}


void StiScanHistContainer::FillDerivedHists()
{
   this->cd();

   TProfile2D* profile2D;

   profile2D = hNStepsVsPhiVsRVsZ->Project3DProfile("yx");
   profile2D->SetOption("colz");
   Add(profile2D);
   Add(mDoProjection ? profile2D->ProjectionX() : profile2D->ProfileX());

   profile2D = hNStepsVsPhiVsRVsZ->Project3DProfile("yz");
   profile2D->SetOption("colz");
   Add(profile2D);
   Add(mDoProjection ? profile2D->ProjectionX() : profile2D->ProfileX());

   profile2D = hNStepsVsPhiVsRVsZ->Project3DProfile("xz");
   profile2D->SetOption("colz");
   Add(profile2D);
   Add(mDoProjection ? profile2D->ProjectionX() : profile2D->ProfileX());


   profile2D = hELossVsPhiVsRVsZ->Project3DProfile("yx");
   profile2D->SetOption("colz XZ");
   Add(profile2D);
   TH1* h = mDoProjection ? profile2D->ProjectionX() : profile2D->ProfileX();
   h->SetOption("XY");
   Add(h);

   profile2D = hELossVsPhiVsRVsZ->Project3DProfile("yz");
   profile2D->SetOption("colz XZ");
   Add(profile2D);
   Add(mDoProjection ? profile2D->ProjectionX() : profile2D->ProfileX());

   profile2D = hELossVsPhiVsRVsZ->Project3DProfile("xz");
   profile2D->SetOption("colz XZ");
   Add(profile2D);
   Add(mDoProjection ? profile2D->ProjectionX() : profile2D->ProfileX());


   profile2D = hELossVsXVsYVsZ->Project3DProfile("yx");
   profile2D->SetOption("colz XZ");
   Add(profile2D);


   // For 1D density profiles we always calculate the average density
   profile2D = hDensityVsPhiVsRVsZ->Project3DProfile("yx");
   profile2D->SetOption("colz");
   Add(profile2D);
   h = profile2D->ProfileX();
   h->SetOption("XY");
   Add(h);

   profile2D = hDensityVsPhiVsRVsZ->Project3DProfile("yz");
   profile2D->SetOption("colz");
   Add(profile2D);
   Add(profile2D->ProfileX());

   profile2D = hDensityVsPhiVsRVsZ->Project3DProfile("xz");
   profile2D->SetOption("colz");
   Add(profile2D);
   Add(profile2D->ProfileX());


   profile2D = hRelRadLengthVsPhiVsRVsZ->Project3DProfile("yx");
   profile2D->SetOption("colz");
   Add(profile2D);
   h = mDoProjection ? profile2D->ProjectionX() : profile2D->ProfileX();
   h->SetOption("XY");
   Add(h);

   profile2D = hRelRadLengthVsPhiVsRVsZ->Project3DProfile("yz");
   profile2D->SetOption("colz");
   Add(profile2D);
   Add(mDoProjection ? profile2D->ProjectionX() : profile2D->ProfileX());

   profile2D = hRelRadLengthVsPhiVsRVsZ->Project3DProfile("xz");
   profile2D->SetOption("colz");
   Add(profile2D);
   Add(mDoProjection ? profile2D->ProjectionX() : profile2D->ProfileX());
}


void StiScanHistContainer::FillHists(const TStiKalmanTrack &kalmTrack, const std::set<std::string> *volumeList)
{
   // Take the first node with the smallest radius
   const TStiKalmanTrackNode& dcaNode = kalmTrack.GetDcaNode();

   h("hTrackCountVsEtaVsPhi")->Fill(dcaNode.GetTrackP().Eta(), dcaNode.GetTrackP().Phi());
   h("hTrackCountVsZVsPhi")->Fill(dcaNode.GetPosition().Z(), dcaNode.GetTrackP().Phi());

   for (const auto& kalmNode : kalmTrack.GetNodes())
   {
      if (volumeList && volumeList->size() && !kalmNode.MatchedVolName(*volumeList) ) continue;

      if (kalmNode.GetNodeMaterialDensity() <= 0) continue;

      hNStepsVsPhiVsRVsZ->FillAsCumulative(kalmNode.GetPosition().Phi(), kalmNode.GetPosition().Perp(), kalmNode.GetPosition().Z(), 1);
      dynamic_cast<TProfile2D&>( *h("hELossVsEtaVsPhi_trk")).Fill(kalmNode.GetTrackP().Eta(), kalmNode.GetTrackP().Phi(), kalmNode.GetEnergyLosses());

      dynamic_cast<TProfile2D&>( *h("hELossVsEtaVsPhi")).Fill(kalmNode.GetPosition().Eta(), kalmNode.GetPosition().Phi(),  kalmNode.GetEnergyLosses());
      dynamic_cast<TProfile2D&>( *h("hELossVsZVsPhi")  ).Fill(kalmNode.GetPosition().Z(),   kalmNode.GetPosition().Phi(),  kalmNode.GetEnergyLosses());
      dynamic_cast<TProfile2D&>( *h("hELossVsZVsR")    ).Fill(kalmNode.GetPosition().Z(),   kalmNode.GetPosition().Perp(), kalmNode.GetEnergyLosses());
      dynamic_cast<TProfile2D&>( *h("hELossVsPhiVsR")  ).Fill(kalmNode.GetPosition().Phi(), kalmNode.GetPosition().Perp(), kalmNode.GetEnergyLosses());
      // Record the total energy deposited by this track
      hELossVsPhiVsRVsZ->FillAsCumulative(kalmNode.GetPosition().Phi(), kalmNode.GetPosition().Perp(), kalmNode.GetPosition().Z(), kalmNode.GetEnergyLosses());
      hELossVsXVsYVsZ->FillAsCumulative(kalmNode.GetPosition().X(), kalmNode.GetPosition().Y(), kalmNode.GetPosition().Z(), kalmNode.GetEnergyLosses());
      hDensityVsPhiVsRVsZ->Fill(kalmNode.GetPosition().Phi(), kalmNode.GetPosition().Perp(), kalmNode.GetPosition().Z(), kalmNode.GetNodeMaterialDensity());
      hRelRadLengthVsPhiVsRVsZ->FillAsCumulative(kalmNode.GetPosition().Phi(), kalmNode.GetPosition().Perp(), kalmNode.GetPosition().Z(), kalmNode.GetNodeRelRadLength());
   }

   hNStepsVsPhiVsRVsZ->ResetBinCumulMode();
   hELossVsPhiVsRVsZ->ResetBinCumulMode();
   hELossVsXVsYVsZ->ResetBinCumulMode();
   hRelRadLengthVsPhiVsRVsZ->ResetBinCumulMode();
}


void StiScanHistContainer::FillHists(const TGeaTrack &trackG, const std::set<std::string> *volumeList)
{
   TIter iGeaStep(&trackG.steps);

   while (TGeaStep* stepG = static_cast<TGeaStep*>(iGeaStep()) )
   {
      double dEStep = stepG->dEstep * 1e6; // convert GeV to keV

      TVector3 step_pos(stepG->x, stepG->y, stepG->z);

      if (volumeList && volumeList->size() && !stepG->MatchedVolName(*volumeList) ) continue;

      hNStepsVsPhiVsRVsZ->FillAsCumulative(step_pos.Phi(), step_pos.Perp(), step_pos.Z(), 1);

      dynamic_cast<TProfile2D&>( *h("hELossVsZVsPhi") ).Fill(step_pos.Z(),   step_pos.Phi(),  dEStep);
      dynamic_cast<TProfile2D&>( *h("hELossVsZVsR")   ).Fill(step_pos.Z(),   step_pos.Perp(), dEStep);
      dynamic_cast<TProfile2D&>( *h("hELossVsPhiVsR") ).Fill(step_pos.Phi(), step_pos.Perp(), dEStep);
      hELossVsPhiVsRVsZ->FillAsCumulative(step_pos.Phi(), step_pos.Perp(), step_pos.Z(), dEStep);
      hELossVsXVsYVsZ->FillAsCumulative(step_pos.X(), step_pos.Y(), step_pos.Z(), dEStep);
      hDensityVsPhiVsRVsZ->Fill(step_pos.Phi(), step_pos.Perp(), step_pos.Z(), stepG->dens);
      hRelRadLengthVsPhiVsRVsZ->FillAsCumulative(step_pos.Phi(), step_pos.Perp(), step_pos.Z(), stepG->relRadLength);
   }

   hNStepsVsPhiVsRVsZ->ResetBinCumulMode();
   hELossVsPhiVsRVsZ->ResetBinCumulMode();
   hELossVsXVsYVsZ->ResetBinCumulMode();
   hRelRadLengthVsPhiVsRVsZ->ResetBinCumulMode();
}
