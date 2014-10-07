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
   hELossVsPhiVsRVsZ(nullptr),
   hRelRadLengthVsPhiVsRVsZ(nullptr),
   hNStepsVsPhiVsR_buf(nullptr),
   hNStepsVsPhiVsR(nullptr)
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
   hELossVsPhiVsRVsZ(nullptr),
   hRelRadLengthVsPhiVsRVsZ(nullptr),
   hNStepsVsPhiVsR_buf(nullptr),
   hNStepsVsPhiVsR(nullptr)
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

   // Histograms for selected track nodes/volumes only
   hNStepsVsPhiVsR_buf = new Profile2D("hNStepsVsPhiVsR_buf", " ; #phi, rad; r, cm; Num. of Steps", 120, -M_PI, M_PI, nRBins, mNodeRMin, mNodeRMax);

   hNStepsVsPhiVsR = new Profile2D("hNStepsVsPhiVsR", " ; #phi, rad; r, cm; Num. of Steps", 120, -M_PI, M_PI, nRBins, mNodeRMin, mNodeRMax);
   mHs["hNStepsVsPhiVsR"] = hNStepsVsPhiVsR;
   hNStepsVsPhiVsR->SetOption("colz");

   mHs["hELossVsEtaVsPhi_trk"] = h = new TProfile2D("hELossVsEtaVsPhi_trk", " ; Track #eta; Track #phi, rad; Energy Losses in Select Volumes, keV", 50, -2, 2, 120, -M_PI, M_PI);
   h->SetOption("colz");

   mHs["hELossVsEtaVsPhi"] = h = new TProfile2D("hELossVsEtaVsPhi", " ; #eta; #phi, rad; Energy Losses in Select Volumes, keV", 50, -2, 2, 120, -M_PI, M_PI);
   h->SetOption("colz");

   mHs["hELossVsZVsPhi"]   = h = new TProfile2D("hELossVsZVsPhi", " ; z, cm; #phi, rad; Energy Losses in Select Volumes, keV", nZBins, mNodeZMin, mNodeZMax, 120, -M_PI, M_PI);
   h->SetOption("colz");

   mHs["hELossVsZVsR"]     = h = new TProfile2D("hELossVsZVsR", " ; z, cm; r, cm; Energy Losses in Select Volumes, keV", nZBins, mNodeZMin, mNodeZMax, nRBins, mNodeRMin, mNodeRMax);
   h->SetOption("colz");

   mHs["hELossVsPhiVsR"]   = h = new TProfile2D("hELossVsPhiVsR", " ; #phi, rad; r, cm; Energy Losses in Select Volumes, keV", 120, -M_PI, M_PI, nRBins, mNodeRMin, mNodeRMax);
   h->SetOption("colz");

   mHs["hDensityVsPhiVsR"] = h = new TProfile2D("hDensityVsPhiVsR", " ; #phi, rad; r, cm; Material Density, g/cm^{3}", 120, -M_PI, M_PI, nRBins, mNodeRMin, mNodeRMax);
   h->SetOption("colz");
   mHs["hELossVsPhiVsRVsZ"] = hELossVsPhiVsRVsZ =
      new Profile3D("hELossVsPhiVsRVsZ", "Energy Losses in Select Volumes, keV ; #phi, rad; r, cm; z, cm", 120, -M_PI, M_PI, nRBins, mNodeRMin, mNodeRMax, nZBins, mNodeZMin, mNodeZMax);

   mHs["hRelRadLengthVsPhiVsR"] = h = new TProfile2D("hRelRadLengthVsPhiVsR", " ; #phi, rad; r, cm; Rel. Radiation Length, %", 120, -M_PI, M_PI, nRBins, mNodeRMin, mNodeRMax);
   h->SetOption("colz");

   h->SetOption("colz");
   mHs["hRelRadLengthVsPhiVsRVsZ"] = hRelRadLengthVsPhiVsRVsZ =
      new Profile3D("hRelRadLengthVsPhiVsRVsZ", "Rel. Radiation Length, %; #phi, rad; r, cm; z, cm", 120, -M_PI, M_PI, nRBins, mNodeRMin, mNodeRMax, nZBins, mNodeZMin, mNodeZMax);
}


void StiScanHistContainer::FillHists(const EventT &eventT, const std::set<std::string> *volumeList)
{
   std::vector<TStiKalmanTrack>::const_iterator iTStiKTrack = eventT.fTStiKalmanTracks.begin();

   for ( ; iTStiKTrack != eventT.fTStiKalmanTracks.end(); ++iTStiKTrack)
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

   TH1* h;
   TProfile2D* prof2D;
   TProfile2D* profile2D;

   // Create a projection from hNStepsVsPhiVsR
   prof2D = (TProfile2D*) mHs["hNStepsVsPhiVsR"];

   mHs["hNStepsVsPhiVsR_px"]  = h = prof2D->ProjectionX();
   h->SetTitle(" ; #phi, rad; Num. of Steps (from ProjectionX)");

   mHs["hELossVsPhiVsRVsZ_pyx"] = profile2D = hELossVsPhiVsRVsZ->Project3DProfile("yx");
   profile2D->SetOption("colz");
   mHs["hELossVsPhiVsRVsZ_pyx_px"] = mDoProjection ? profile2D->ProjectionX() : profile2D->ProfileX();

   mHs["hELossVsPhiVsRVsZ_pyz"] = profile2D = hELossVsPhiVsRVsZ->Project3DProfile("yz");
   profile2D->SetOption("colz");
   mHs["hELossVsPhiVsRVsZ_pyz_px"] = mDoProjection ? profile2D->ProjectionX() : profile2D->ProfileX();

   mHs["hELossVsPhiVsRVsZ_pxz"] = profile2D = hELossVsPhiVsRVsZ->Project3DProfile("xz");
   profile2D->SetOption("colz");
   mHs["hELossVsPhiVsRVsZ_pxz_px"] = mDoProjection ? profile2D->ProjectionX() : profile2D->ProfileX();

   mHs["hRelRadLengthVsPhiVsRVsZ_pyx"] = profile2D = hRelRadLengthVsPhiVsRVsZ->Project3DProfile("yx");
   profile2D->SetOption("colz");
   mHs["hRelRadLengthVsPhiVsRVsZ_pyx_px"] = mDoProjection ? profile2D->ProjectionX() : profile2D->ProfileX();

   mHs["hRelRadLengthVsPhiVsRVsZ_pyz"] = profile2D = hRelRadLengthVsPhiVsRVsZ->Project3DProfile("yz");
   profile2D->SetOption("colz");
   mHs["hRelRadLengthVsPhiVsRVsZ_pyz_px"] = mDoProjection ? profile2D->ProjectionX() : profile2D->ProfileX();

   mHs["hRelRadLengthVsPhiVsRVsZ_pxz"] = profile2D = hRelRadLengthVsPhiVsRVsZ->Project3DProfile("xz");
   profile2D->SetOption("colz");
   mHs["hRelRadLengthVsPhiVsRVsZ_pxz_px"] = mDoProjection ? profile2D->ProjectionX() : profile2D->ProfileX();

   Profile2D* hProf = (Profile2D*) mHs["hELossVsPhiVsR"];
   hProf->Multiply(*hNStepsVsPhiVsR);

   // Create a projection from hELossVsPhiVsR
   prof2D = (TProfile2D*) mHs["hELossVsPhiVsR"];

   mHs["hELossVsPhiVsR_px"]  = h = prof2D->ProjectionX();
   h->SetTitle(" ; #phi, rad; Energy Losses in Select Volumes, keV");
   h->SetOption("XY");

   // Create a projection from hRelRadLengthVsPhiVsR
   prof2D = (TProfile2D*) mHs["hRelRadLengthVsPhiVsR"];

   mHs["hRelRadLengthVsPhiVsR_px"] = h = prof2D->ProjectionX();
   h->SetTitle(" ; #phi, rad; Rel. Radiation Length, %");
   h->SetOption("XY");

   prof2D = (TProfile2D*) mHs["hDensityVsPhiVsR"];

   mHs["hDensityVsPhiVsR_px"] = h = prof2D->ProjectionX();
   h->SetTitle(" ; #phi, rad; \"Total\" (added) Material Density, g/cm^{3}");
}


void StiScanHistContainer::FillHists(const TStiKalmanTrack &kalmTrack, const std::set<std::string> *volumeList)
{
   // Take the first node with the smallest radius
   const TStiKalmanTrackNode& dcaNode = kalmTrack.GetDcaNode();

   mHs["hTrackCountVsEtaVsPhi"]->Fill(dcaNode.GetTrackP().Eta(), dcaNode.GetTrackP().Phi());
   mHs["hTrackCountVsZVsPhi"]->Fill(dcaNode.GetPosition().Z(), dcaNode.GetTrackP().Phi());

   hNStepsVsPhiVsR_buf->Reset("ICES"); // reset the histogram

   std::set<TStiKalmanTrackNode>::const_iterator iTStiKTrackNode = kalmTrack.GetNodes().begin();

   for ( ; iTStiKTrackNode != kalmTrack.GetNodes().end(); ++iTStiKTrackNode)
   {
      const TStiKalmanTrackNode &kalmNode = *iTStiKTrackNode;

      if (volumeList && volumeList->size() && !kalmNode.MatchedVolName(*volumeList) ) continue;

      if (kalmNode.GetNodeMaterialDensity() <= 0) continue;

      hNStepsVsPhiVsR_buf->Fill(kalmNode.GetPosition().Phi(), kalmNode.GetPosition().Perp(), 1);
      ((TProfile2D*) mHs["hELossVsEtaVsPhi_trk"])->Fill(kalmNode.GetTrackP().Eta(), kalmNode.GetTrackP().Phi(), kalmNode.GetEnergyLosses());

      ((TProfile2D*) mHs["hELossVsEtaVsPhi"])->Fill(kalmNode.GetPosition().Eta(), kalmNode.GetPosition().Phi(),  kalmNode.GetEnergyLosses());
      ((TProfile2D*) mHs["hELossVsZVsPhi"])  ->Fill(kalmNode.GetPosition().Z(),   kalmNode.GetPosition().Phi(),  kalmNode.GetEnergyLosses());
      ((TProfile2D*) mHs["hELossVsZVsR"])    ->Fill(kalmNode.GetPosition().Z(),   kalmNode.GetPosition().Perp(), kalmNode.GetEnergyLosses());
      ((TProfile2D*) mHs["hELossVsPhiVsR"])  ->Fill(kalmNode.GetPosition().Phi(), kalmNode.GetPosition().Perp(), kalmNode.GetEnergyLosses());
      hELossVsPhiVsRVsZ->FillAsCumulative(kalmNode.GetPosition().Phi(), kalmNode.GetPosition().Perp(), kalmNode.GetPosition().Z(), kalmNode.GetEnergyLosses());
      ((TProfile2D*) mHs["hDensityVsPhiVsR"])->Fill(kalmNode.GetPosition().Phi(), kalmNode.GetPosition().Perp(), kalmNode.GetNodeMaterialDensity());
      ((TProfile2D*) mHs["hRelRadLengthVsPhiVsR"])->Fill(kalmNode.GetPosition().Phi(), kalmNode.GetPosition().Perp(), kalmNode.GetNodeRelRadLength());
      hRelRadLengthVsPhiVsRVsZ->FillAsCumulative(kalmNode.GetPosition().Phi(), kalmNode.GetPosition().Perp(), kalmNode.GetPosition().Z(), kalmNode.GetNodeRelRadLength());
   }

   hELossVsPhiVsRVsZ->ResetBinCumulMode();
   hRelRadLengthVsPhiVsRVsZ->ResetBinCumulMode();

   hNStepsVsPhiVsR_buf->ResetBinEntries(1); // Force number of entries per bin to 1
   hNStepsVsPhiVsR->Add(hNStepsVsPhiVsR_buf); // Calculate the average number of steps by adding the temp profile
}


void StiScanHistContainer::FillHists(const TrackG &trackG, const std::set<std::string> *volumeList)
{
   hNStepsVsPhiVsR_buf->Reset("ICES"); // reset the histogram

   TIter iGeantStep(&trackG.steps);

   while (StepG* stepG = (StepG*) iGeantStep())
   {
      double dEStep = stepG->dEstep * 1e6; // convert GeV to keV

      TVector3 step_pos(stepG->x, stepG->y, stepG->z);

      if (volumeList && volumeList->size() && !stepG->MatchedVolName(*volumeList) ) continue;

      hNStepsVsPhiVsR_buf->Fill(step_pos.Phi(), step_pos.Perp(), 1);

      ((TProfile2D*) mHs["hELossVsZVsPhi"])->Fill(step_pos.Z(),   step_pos.Phi(),  dEStep);
      ((TProfile2D*) mHs["hELossVsZVsR"])  ->Fill(step_pos.Z(),   step_pos.Perp(), dEStep);
      ((TProfile2D*) mHs["hELossVsPhiVsR"])->Fill(step_pos.Phi(), step_pos.Perp(), dEStep);
      hELossVsPhiVsRVsZ->FillAsCumulative(step_pos.Phi(), step_pos.Perp(), step_pos.Z(), dEStep);
      ((TProfile2D*) mHs["hDensityVsPhiVsR"])->Fill(step_pos.Phi(), step_pos.Perp(), stepG->dens);
      ((TProfile2D*) mHs["hRelRadLengthVsPhiVsR"])->Fill(step_pos.Phi(), step_pos.Perp(), stepG->relRadLength);
      hRelRadLengthVsPhiVsRVsZ->FillAsCumulative(step_pos.Phi(), step_pos.Perp(), step_pos.Z(), stepG->relRadLength);
   }

   hELossVsPhiVsRVsZ->ResetBinCumulMode();
   hRelRadLengthVsPhiVsRVsZ->ResetBinCumulMode();

   hNStepsVsPhiVsR_buf->ResetBinEntries(1); // Force number of entries per bin to 1
   hNStepsVsPhiVsR->Add(hNStepsVsPhiVsR_buf); // Calculate the average number of steps by adding the temp profile
}


void StiScanHistContainer::SaveAllAs(std::string prefix)
{
   TCanvas canvas("canvas", "canvas", 1400, 600);
   canvas.UseCurrentStyle();
   canvas.SetGridx(true);
   canvas.SetGridy(true);
   canvas.SetLogz(true);

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
      char* l = strstr(opts, "XY");

      if (l) canvas.SetLogy(true);
      else   canvas.SetLogy(false);

      obj->Draw();

      string sFileName = prefix + "/c_" + objName + ".png";
      canvas.SaveAs(sFileName.c_str());
   }
}
