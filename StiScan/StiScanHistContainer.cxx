#include <cmath>

#include "TCanvas.h"
#include "TH2S.h"
#include "TProfile2D.h"
#include "TVector3.h"

#include "StiScan/StiScanHistContainer.h"
#include "StiScan/TStiKalmanTrackNode.h"


StiScanHistContainer::StiScanHistContainer(StiScanPrgOptions& prgOpts) : TDirectoryFile(),
   fPrgOptions(prgOpts), mHs(), mNodeZMin(-250), mNodeZMax(250),
   mNodeRMin(0), mNodeRMax(30),
   mDoProjection(false),
   hNStepsVsPhiVsRVsZ(nullptr),
   hELossVsPhiVsRVsZ(nullptr),
   hELossVsXVsYVsZ(nullptr),
   hDensityVsPhiVsRVsZ(nullptr),
   hRelRadLengthVsPhiVsRVsZ(nullptr)
{
   InitRange();
   BookHists();
}


StiScanHistContainer::StiScanHistContainer(StiScanPrgOptions& prgOpts, const char* name, TDirectory* motherDir, bool doProjection, Option_t* option) :
   TDirectoryFile(name, name, option, motherDir),
   fPrgOptions(prgOpts),
   mHs(), mNodeZMin(-250), mNodeZMax(250),
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


StiScanHistContainer::~StiScanHistContainer()
{
   while (!mHs.empty()) {
      delete mHs.begin()->second;
      mHs.erase(mHs.begin());
   }
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

   nRBins = nRBins > 150 ? 150 : nRBins;
   nRBins = nRBins <  50 ?  50 : nRBins;

   nZBins = nZBins > 500 ? 500 : nZBins;

   this->cd();

   TH1* h;

   mHs["hTrackCountVsEtaVsPhi"] = h = new TH2S("hTrackCountVsEtaVsPhi", " ; #eta; #phi, rad; Num. of Tracks", 50, -2, 2, 120, -M_PI, M_PI);
   h->SetOption("colz");

   mHs["hTrackCountVsZVsPhi"]   = h = new TH2S("hTrackCountVsZVsPhi", " ; z, cm; #phi, rad; Num. of Tracks", nZBins, mNodeZMin, mNodeZMax, 120, -M_PI, M_PI);
   h->SetOption("colz");

   mHs["hELossVsEtaVsPhi_trk"] = h = new TProfile2D("hELossVsEtaVsPhi_trk", " ; Track #eta; Track #phi, rad; Energy Losses in Select Volumes, keV", 50, -2, 2, 120, -M_PI, M_PI);
   h->SetOption("colz");

   mHs["hELossVsEtaVsPhi"] = h = new TProfile2D("hELossVsEtaVsPhi", " ; #eta; #phi, rad; Energy Losses in Select Volumes, keV", 50, -2, 2, 120, -M_PI, M_PI);
   h->SetOption("colz");

   mHs["hELossVsZVsPhi"]   = h = new TProfile2D("hELossVsZVsPhi", " ; z, cm; #phi, rad; Energy Losses in Select Volumes, keV", nZBins, mNodeZMin, mNodeZMax, 120, -M_PI, M_PI);
   h->SetOption("colz");

   mHs["hELossVsZVsR"]     = h = new TProfile2D("hELossVsZVsR", " ; z, cm; r, cm; Energy Losses in Select Volumes, keV", nZBins, mNodeZMin, mNodeZMax, nRBins, mNodeRMin, mNodeRMax);
   h->SetOption("colz");

   mHs["hELossVsPhiVsR"]   = h = new Profile2D("hELossVsPhiVsR", " ; #phi, rad; r, cm; Energy Losses in Select Volumes, keV", 120, -M_PI, M_PI, nRBins, mNodeRMin, mNodeRMax, "colz");

   mHs["hNStepsVsPhiVsRVsZ"] = hNStepsVsPhiVsRVsZ =
      new Profile3D("hNStepsVsPhiVsRVsZ", "Num. of Steps per Track ; #phi, rad; r, cm; z, cm", 120, -M_PI, M_PI, nRBins, mNodeRMin, mNodeRMax, nZBins, mNodeZMin, mNodeZMax);

   mHs["hELossVsPhiVsRVsZ"] = hELossVsPhiVsRVsZ =
      new Profile3D("hELossVsPhiVsRVsZ", "Energy Losses in Select Volumes, keV ; #phi, rad; r, cm; z, cm", 120, -M_PI, M_PI, nRBins, mNodeRMin, mNodeRMax, nZBins, mNodeZMin, mNodeZMax);

   mHs["hELossVsXVsYVsZ"] = hELossVsXVsYVsZ =
      new Profile3D("hELossVsXVsYVsZ", "Energy Losses in Select Volumes, keV ; x, cm; y, cm; z, cm", nRBins, -mNodeRMax, mNodeRMax, nRBins, -mNodeRMax, mNodeRMax, nZBins, mNodeZMin, mNodeZMax);

   mHs["hDensityVsPhiVsRVsZ"] = hDensityVsPhiVsRVsZ =
      new Profile3D("hDensityVsPhiVsRVsZ", "Material Density, g/cm^{3}; #phi, rad; r, cm; z, cm", 120, -M_PI, M_PI, nRBins, mNodeRMin, mNodeRMax, nZBins, mNodeZMin, mNodeZMax);

   mHs["hRelRadLengthVsPhiVsRVsZ"] = hRelRadLengthVsPhiVsRVsZ =
      new Profile3D("hRelRadLengthVsPhiVsRVsZ", "Rel. Radiation Length, %; #phi, rad; r, cm; z, cm", 120, -M_PI, M_PI, nRBins, mNodeRMin, mNodeRMax, nZBins, mNodeZMin, mNodeZMax);
}


void StiScanHistContainer::FillHists(const StiScanEvent &eventT, const std::set<std::string> *volumeList)
{
   std::vector<TStiKalmanTrack>::const_iterator iTStiKTrack = eventT.GetTStiKalmanTracks().begin();

   for ( ; iTStiKTrack != eventT.GetTStiKalmanTracks().end(); ++iTStiKTrack)
   {
      const TStiKalmanTrack &kalmTrack = *iTStiKTrack;
      FillHists(kalmTrack, volumeList);
   }
}


void StiScanHistContainer::FillHists(const EventG &eventG, const std::set<std::string> *volumeList)
{
   TIter iGeantTrack(eventG.tracks);

   while (TrackG* trackG = (TrackG*) iGeantTrack())
   {
      FillHists(*trackG, volumeList);
   }
}


void StiScanHistContainer::FillDerivedHists()
{
   this->cd();

   TProfile2D* profile2D;

   mHs["hNStepsVsPhiVsRVsZ_pyx"] = profile2D = hNStepsVsPhiVsRVsZ->Project3DProfile("yx");
   profile2D->SetOption("colz");
   mHs["hNStepsVsPhiVsRVsZ_pyx_px"] = mDoProjection ? profile2D->ProjectionX() : profile2D->ProfileX();

   mHs["hNStepsVsPhiVsRVsZ_pyz"] = profile2D = hNStepsVsPhiVsRVsZ->Project3DProfile("yz");
   profile2D->SetOption("colz");
   mHs["hNStepsVsPhiVsRVsZ_pyz_px"] = mDoProjection ? profile2D->ProjectionX() : profile2D->ProfileX();

   mHs["hNStepsVsPhiVsRVsZ_pxz"] = profile2D = hNStepsVsPhiVsRVsZ->Project3DProfile("xz");
   profile2D->SetOption("colz");
   mHs["hNStepsVsPhiVsRVsZ_pxz_px"] = mDoProjection ? profile2D->ProjectionX() : profile2D->ProfileX();


   mHs["hELossVsPhiVsRVsZ_pyx"] = profile2D = hELossVsPhiVsRVsZ->Project3DProfile("yx");
   profile2D->SetOption("colz logZ");
   mHs["hELossVsPhiVsRVsZ_pyx_px"] = mDoProjection ? profile2D->ProjectionX() : profile2D->ProfileX();
   mHs["hELossVsPhiVsRVsZ_pyx_px"]->SetOption("logY");

   mHs["hELossVsPhiVsRVsZ_pyz"] = profile2D = hELossVsPhiVsRVsZ->Project3DProfile("yz");
   profile2D->SetOption("colz logZ");
   mHs["hELossVsPhiVsRVsZ_pyz_px"] = mDoProjection ? profile2D->ProjectionX() : profile2D->ProfileX();

   mHs["hELossVsPhiVsRVsZ_pxz"] = profile2D = hELossVsPhiVsRVsZ->Project3DProfile("xz");
   profile2D->SetOption("colz logZ");
   mHs["hELossVsPhiVsRVsZ_pxz_px"] = mDoProjection ? profile2D->ProjectionX() : profile2D->ProfileX();


   mHs["hELossVsXVsYVsZ_pyx"] = profile2D = hELossVsXVsYVsZ->Project3DProfile("yx");
   profile2D->SetOption("colz logZ");


   // For 1D density profiles we always calculate the average density
   mHs["hDensityVsPhiVsRVsZ_pyx"] = profile2D = hDensityVsPhiVsRVsZ->Project3DProfile("yx");
   profile2D->SetOption("colz");
   mHs["hDensityVsPhiVsRVsZ_pyx_px"] = profile2D->ProfileX();
   mHs["hDensityVsPhiVsRVsZ_pyx_px"]->SetOption("logY");

   mHs["hDensityVsPhiVsRVsZ_pyz"] = profile2D = hDensityVsPhiVsRVsZ->Project3DProfile("yz");
   profile2D->SetOption("colz");
   mHs["hDensityVsPhiVsRVsZ_pyz_px"] = profile2D->ProfileX();

   mHs["hDensityVsPhiVsRVsZ_pxz"] = profile2D = hDensityVsPhiVsRVsZ->Project3DProfile("xz");
   profile2D->SetOption("colz");
   mHs["hDensityVsPhiVsRVsZ_pxz_px"] = profile2D->ProfileX();


   mHs["hRelRadLengthVsPhiVsRVsZ_pyx"] = profile2D = hRelRadLengthVsPhiVsRVsZ->Project3DProfile("yx");
   profile2D->SetOption("colz");
   mHs["hRelRadLengthVsPhiVsRVsZ_pyx_px"] = mDoProjection ? profile2D->ProjectionX() : profile2D->ProfileX();
   mHs["hRelRadLengthVsPhiVsRVsZ_pyx_px"]->SetOption("logY");

   mHs["hRelRadLengthVsPhiVsRVsZ_pyz"] = profile2D = hRelRadLengthVsPhiVsRVsZ->Project3DProfile("yz");
   profile2D->SetOption("colz");
   mHs["hRelRadLengthVsPhiVsRVsZ_pyz_px"] = mDoProjection ? profile2D->ProjectionX() : profile2D->ProfileX();

   mHs["hRelRadLengthVsPhiVsRVsZ_pxz"] = profile2D = hRelRadLengthVsPhiVsRVsZ->Project3DProfile("xz");
   profile2D->SetOption("colz");
   mHs["hRelRadLengthVsPhiVsRVsZ_pxz_px"] = mDoProjection ? profile2D->ProjectionX() : profile2D->ProfileX();
}


void StiScanHistContainer::FillHists(const TStiKalmanTrack &kalmTrack, const std::set<std::string> *volumeList)
{
   // Take the first node with the smallest radius
   const TStiKalmanTrackNode& dcaNode = kalmTrack.GetDcaNode();

   mHs["hTrackCountVsEtaVsPhi"]->Fill(dcaNode.GetTrackP().Eta(), dcaNode.GetTrackP().Phi());
   mHs["hTrackCountVsZVsPhi"]->Fill(dcaNode.GetPosition().Z(), dcaNode.GetTrackP().Phi());

   std::set<TStiKalmanTrackNode>::const_iterator iTStiKTrackNode = kalmTrack.GetNodes().begin();

   for ( ; iTStiKTrackNode != kalmTrack.GetNodes().end(); ++iTStiKTrackNode)
   {
      const TStiKalmanTrackNode &kalmNode = *iTStiKTrackNode;

      if (volumeList && volumeList->size() && !kalmNode.MatchedVolName(*volumeList) ) continue;

      if (kalmNode.GetNodeMaterialDensity() <= 0) continue;

      hNStepsVsPhiVsRVsZ->FillAsCumulative(kalmNode.GetPosition().Phi(), kalmNode.GetPosition().Perp(), kalmNode.GetPosition().Z(), 1);
      ((TProfile2D*) mHs["hELossVsEtaVsPhi_trk"])->Fill(kalmNode.GetTrackP().Eta(), kalmNode.GetTrackP().Phi(), kalmNode.GetEnergyLosses());

      ((TProfile2D*) mHs["hELossVsEtaVsPhi"])->Fill(kalmNode.GetPosition().Eta(), kalmNode.GetPosition().Phi(),  kalmNode.GetEnergyLosses());
      ((TProfile2D*) mHs["hELossVsZVsPhi"])  ->Fill(kalmNode.GetPosition().Z(),   kalmNode.GetPosition().Phi(),  kalmNode.GetEnergyLosses());
      ((TProfile2D*) mHs["hELossVsZVsR"])    ->Fill(kalmNode.GetPosition().Z(),   kalmNode.GetPosition().Perp(), kalmNode.GetEnergyLosses());
      ((TProfile2D*) mHs["hELossVsPhiVsR"])  ->Fill(kalmNode.GetPosition().Phi(), kalmNode.GetPosition().Perp(), kalmNode.GetEnergyLosses());
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


void StiScanHistContainer::FillHists(const TrackG &trackG, const std::set<std::string> *volumeList)
{
   TIter iGeantStep(&trackG.steps);

   while (StepG* stepG = (StepG*) iGeantStep())
   {
      double dEStep = stepG->dEstep * 1e6; // convert GeV to keV

      TVector3 step_pos(stepG->x, stepG->y, stepG->z);

      if (volumeList && volumeList->size() && !stepG->MatchedVolName(*volumeList) ) continue;

      hNStepsVsPhiVsRVsZ->FillAsCumulative(step_pos.Phi(), step_pos.Perp(), step_pos.Z(), 1);

      ((TProfile2D*) mHs["hELossVsZVsPhi"])->Fill(step_pos.Z(),   step_pos.Phi(),  dEStep);
      ((TProfile2D*) mHs["hELossVsZVsR"])  ->Fill(step_pos.Z(),   step_pos.Perp(), dEStep);
      ((TProfile2D*) mHs["hELossVsPhiVsR"])->Fill(step_pos.Phi(), step_pos.Perp(), dEStep);
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


void StiScanHistContainer::SaveAllAs(std::string prefix)
{
   TCanvas canvas("canvas", "canvas", 1400, 600);
   canvas.UseCurrentStyle();
   canvas.SetGridx(true);
   canvas.SetGridy(true);

   HistMapIter iHist = mHs.begin();

   for ( ; iHist!=mHs.end(); ++iHist) {
      // For shorthand
      string   objName = iHist->first;
      TObject *obj      = iHist->second;

      if (!obj) {
         Error("SaveAllAs", "No object found for key %s. Skipping...", objName.c_str());
         continue;
      }

      char* opts = (char*) obj->GetOption();

      if (strstr(opts, "logX")) canvas.SetLogx(true);
      else canvas.SetLogx(false);

      if (strstr(opts, "logY")) canvas.SetLogy(true);
      else canvas.SetLogy(false);

      if (strstr(opts, "logZ")) canvas.SetLogz(true);
      else canvas.SetLogz(false);

      obj->Draw();

      string sFileName = prefix + "/c_" + objName + ".png";
      canvas.SaveAs(sFileName.c_str());
   }
}
