#include <cmath>
#include <boost/filesystem.hpp>

#include "StiScanRootFile.h"

#include "TCanvas.h"
#include "TH2S.h"
#include "TProfile2D.h"
#include "TSystem.h"

#include "StiScan/StiScanHistContainer.h"
#include "StiScan/StiScanHistsByVolume.h"
#include "StiScan/StiScanPrgOptions.h"


ClassImp(StiScanRootFile)


StiScanRootFile::StiScanRootFile(StiScanPrgOptions& prgOpts) : TFile(), mDirs(),
   fPrgOptions(prgOpts)
{
   BookHists();
}


StiScanRootFile::StiScanRootFile(StiScanPrgOptions& prgOpts, const char *fname, Option_t *option, const char *ftitle, Int_t compress) :
   TFile(fname, option, ftitle, compress), mDirs(),
   fPrgOptions(prgOpts)
{
   Info("StiScanRootFile", "Created ROOT file: %s", GetName());

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
   mDirs["gea"]     = new StiScanHistContainer(fPrgOptions, "gea", this, true); // Will create integral projections from 2D histograms
}


void StiScanRootFile::FindAutoRange() const
{
   const std::set<std::string> *volumeList = &fPrgOptions.GetVolumeList();

   TChain *hftChain = fPrgOptions.GetHftChain();
   EventT *eventT = new EventT();

   hftChain->SetBranchAddress("e.", &eventT);
   hftChain->SetBranchStatus("e.*", false);
   hftChain->SetBranchStatus("e.fTStiKalmanTracks*", true);

   double nodeRMax = 0;
   double nodeZMin = DBL_MAX;
   double nodeZMax = -DBL_MAX;

   TRandom myRandom;

   int nTreeEvents = hftChain->GetEntries();

   Info("FindAutoRange", "Found tree/chain with N entries: %d", nTreeEvents);

   for (int iEvent = 1; iEvent <= nTreeEvents; iEvent++)
   {
      if ( nTreeEvents >= 10 && iEvent % int(nTreeEvents*0.1) == 0 )
         Info("FindAutoRange", "Analyzing event %d", iEvent);

      if (myRandom.Rndm() > fPrgOptions.GetSparsity()) continue;

      hftChain->GetEntry(iEvent-1);

      auto iTStiKTrack = eventT->fTStiKalmanTracks.begin();

      for ( ; iTStiKTrack != eventT->fTStiKalmanTracks.end(); ++iTStiKTrack)
      {
         const TStiKalmanTrack &kalmTrack = *iTStiKTrack;

         auto iTStiKTrackNode = kalmTrack.GetNodes().begin();

         for ( ; iTStiKTrackNode != kalmTrack.GetNodes().end(); ++iTStiKTrackNode)
         {
            const TStiKalmanTrackNode &kalmNode = *iTStiKTrackNode;

            if (volumeList && volumeList->size() && !kalmNode.MatchedVolName(*volumeList) ) continue;

            if (kalmNode.GetNodeMaterialDensity() <= 0) continue;

            double node_z = kalmNode.GetPosition().Z();

            if (node_z < nodeZMin) nodeZMin = node_z;
            if (node_z > nodeZMax) nodeZMax = node_z;

            // Find maximum radius
            double node_r = (float) kalmNode.GetPosition().Perp();

            if (node_r > nodeRMax) nodeRMax = node_r;
         }
      }
   }

   Info("FindAutoRange", "Updated nodeZMin, nodeZMax, nodeRMax: %f, %f, %f", nodeZMin, nodeZMax, nodeRMax);

   fPrgOptions.SetHistZRange(nodeZMin, nodeZMax);
   fPrgOptions.SetHistRMax(nodeRMax);

   delete eventT;
}


void StiScanRootFile::FillHists(const EventT &eventT, const std::set<std::string> *volumeList)
{
   StiScanHistContainer* container;

   container = static_cast<StiScanHistContainer*> (mDirs["sti_vol"]);
   container->FillHists(eventT, volumeList);

   container = static_cast<StiScanHistContainer*> (mDirs["sti_trk"]);
   container->FillHists(eventT, volumeList);
}


void StiScanRootFile::FillHists(const EventG &eventG, const std::set<std::string> *volumeList)
{
   StiScanHistContainer* container = static_cast<StiScanHistContainer*> (mDirs["gea"]);
   container->FillHists(eventG, volumeList);
}


void StiScanRootFile::FillDerivedHists()
{
   for (TDirMapConstIter iDir=mDirs.begin() ; iDir!=mDirs.end(); ++iDir)
   {
      std::string  dirName = iDir->first;
      StiScanHistContainer *container = (StiScanHistContainer*) iDir->second;

      if (!container) {
         Error("FillDerivedHists", "No container/directory found for key %s. Skipping...", dirName.c_str());
         continue;
      }

      container->FillDerivedHists();
   }
}


Int_t StiScanRootFile::Write(const char* name, Int_t opt, Int_t bufsiz)
{
   Info("Write", "%s", GetName());

   return TFile::Write(name, opt, bufsiz);
}


Int_t StiScanRootFile::Write(const char* name, Int_t opt, Int_t bufsiz) const
{
   return TFile::Write(name, opt, bufsiz);
}


void StiScanRootFile::Close(Option_t *option)
{
   TFile::Close(option);
}


void StiScanRootFile::SaveAllAs(std::string prefix)
{
   namespace fs = boost::filesystem;

   if (fs::create_directories(prefix))
      Info("SaveAllAs", "Created dir: %s", prefix.c_str());
   else
      Warning("SaveAllAs", "Perhaps dir already exists: %s", prefix.c_str());

   for (TDirMapConstIter iDir=mDirs.begin() ; iDir!=mDirs.end(); ++iDir)
   {
      std::string  dirName = iDir->first;
      StiScanHistContainer *container = static_cast<StiScanHistContainer*> (iDir->second);

      if (!container) {
         Error("SaveAllAs", "No container/directory found for key %s. Skipping...", dirName.c_str());
         continue;
      }

      std::string path = prefix + "/" + dirName;

      if (gSystem->mkdir(path.c_str()) < 0)
         Warning("SaveAllAs", "Perhaps dir already exists: %s", path.c_str());
      else
         Info("SaveAllAs", "Created dir: %s", path.c_str());

      container->SaveAllAs(path);
   }
}
