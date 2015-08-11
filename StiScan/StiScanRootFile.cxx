#include <cmath>
#include <boost/filesystem.hpp>

#include "StiScanRootFile.h"

#include "TRandom.h"

#include "StiScan/StiScanHistContainer.h"
#include "StiScan/StiScanHistsByVolume.h"
#include "StiScan/StiScanPrgOptions.h"
#include "StiScan/StiScanRatiosHistContainer.h"


StiScanRootFile::StiScanRootFile(StiScanPrgOptions& prgOpts, Option_t *option, const char *ftitle, Int_t compress) :
   StiRootFile(prgOpts, option, ftitle, compress),
   fPrgOptions(prgOpts)
{
   // Find ranges (\todo if requested by the user)
   if (fPrgOptions.DoAutoHistRange()) {
      Info("StiScanRootFile", "Find auto range. Loop over tree/chain...");
      FindAutoRange();
   }

   BookHists();
}


void StiScanRootFile::BookHists()
{
   mDirs["sti_vol"] = new StiScanHistsByVolume(fPrgOptions, "sti_vol", this);
   mDirs["sti_trk"] = new StiScanHistContainer(fPrgOptions, "sti_trk", this);
   mDirs["gea"]     = new StiScanHistContainer(fPrgOptions, "gea", this); // Will create integral projections from 2D histograms
}


void StiScanRootFile::FindAutoRange() const
{
   const std::set<std::string> *volumeList = &fPrgOptions.GetVolumeList();

   TChain *stiChain = fPrgOptions.GetStiTChain();

   StiScanEvent *stiEvent = new StiScanEvent();
   stiChain->SetBranchAddress("e.", &stiEvent);
   stiChain->SetBranchStatus("e.TStiEvent.*", false);
   stiChain->SetBranchStatus("e.TStiEvent.fTStiKalmanTracks*", true);

   double nodeRMax = 0;
   double nodeZMin = DBL_MAX;
   double nodeZMax = -DBL_MAX;

   TRandom myRandom;

   int nTreeEvents = stiChain->GetEntries();

   Info("FindAutoRange", "Found tree/chain with N entries: %d", nTreeEvents);

   for (int iEvent = 1; iEvent <= nTreeEvents; iEvent++)
   {
      if ( nTreeEvents >= 10 && iEvent % int(nTreeEvents*0.1) == 0 )
         Info("FindAutoRange", "Analyzing event %d", iEvent);

      if (myRandom.Rndm() > fPrgOptions.GetSparsity()) continue;

      stiChain->GetEntry(iEvent-1);

      for (const auto& kalmTrack : stiEvent->GetTStiKalmanTracks())
      {
         for (const auto& kalmNode : kalmTrack.GetNodes())
         {
            if (volumeList && volumeList->size() && !kalmNode.MatchedVolName(*volumeList) )
               continue;

            if (kalmNode.GetNodeMaterialDensity() <= 0)
               continue;

            double node_z = kalmNode.GetPosition().Z();

            if (node_z < nodeZMin) nodeZMin = node_z;
            if (node_z > nodeZMax) nodeZMax = node_z;

            // Find maximum radius
            double node_r = (float) kalmNode.GetPosition().Perp();

            if (node_r > nodeRMax) nodeRMax = node_r;
         }
      }
   }

   // In case no entries were found in the tree set some reasonable values
   if (nodeRMax == 0 && nodeZMin == DBL_MAX && nodeZMax == -DBL_MAX) {
      nodeRMax = 1;
      nodeZMin = 0;
      nodeZMax = 1;
   }

   Info("FindAutoRange", "Updated nodeZMin, nodeZMax, nodeRMax: %g, %g, %g", nodeZMin, nodeZMax, nodeRMax);

   fPrgOptions.SetHistZRange(nodeZMin, nodeZMax);
   fPrgOptions.SetHistRMax(nodeRMax);

   delete stiEvent;
}


void StiScanRootFile::FillHists(const StiScanEvent &stiEvent, const std::set<std::string> *volumeList)
{
   StiScanHistContainer* container;

   container = static_cast<StiScanHistContainer*> (mDirs["sti_vol"]);
   container->FillHists(stiEvent, volumeList);

   container = static_cast<StiScanHistContainer*> (mDirs["sti_trk"]);
   container->FillHists(stiEvent, volumeList);
}


void StiScanRootFile::FillHists(const TGeaEvent &geaEvent, const std::set<std::string> *volumeList)
{
   StiScanHistContainer* container = static_cast<StiScanHistContainer*> (mDirs["gea"]);
   container->FillHists(geaEvent, volumeList);
}


void StiScanRootFile::FillDerivedHists()
{
   StiRootFile::FillDerivedHists();

   StiScanRatiosHistContainer *ratios;
   mDirs["sti_gea_ratio"] = ratios = new StiScanRatiosHistContainer("sti_gea_ratio", this);

   TH1* gea_eloss = static_cast<StiScanHistContainer*>(mDirs["gea"])->GetHists().find("hELossVsPhiVsRVsZ_pyx")->second;
   TH1* sti_eloss = static_cast<StiScanHistContainer*>(mDirs["sti_trk"])->GetHists().find("hELossVsPhiVsRVsZ_pyx")->second;
   ratios->CreateRatioHist(sti_eloss, gea_eloss);

   gea_eloss = static_cast<StiScanHistContainer*>(mDirs["gea"])->GetHists().find("hELossVsPhiVsRVsZ_pyx_px")->second;
   sti_eloss = static_cast<StiScanHistContainer*>(mDirs["sti_trk"])->GetHists().find("hELossVsPhiVsRVsZ_pyx_px")->second;
   ratios->CreateRatioHist(sti_eloss, gea_eloss);

   gea_eloss = static_cast<StiScanHistContainer*>(mDirs["gea"])->GetHists().find("hELossVsXVsYVsZ_pyx")->second;
   sti_eloss = static_cast<StiScanHistContainer*>(mDirs["sti_trk"])->GetHists().find("hELossVsXVsYVsZ_pyx")->second;
   ratios->CreateRatioHist(sti_eloss, gea_eloss);
}
