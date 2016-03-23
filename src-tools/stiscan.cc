#include <fstream>
#include <string>
#include <unordered_map>

#include "TChain.h"
#include "TError.h"
#include "TGeoNavigator.h"
#include "TGeoManager.h"
#include "TRandom.h"

#include "GeaRootIO/TGeaEvent.h"
#include "StiScan/StiScanEvent.h"
#include "StiScan/StiScanPrgOptions.h"
#include "StiScan/StiScanRootFile.h"

typedef std::unordered_map<size_t, std::string> Hash2StringMap;

TGeoManager *gGeoManager = nullptr;


void loop_over_tree(StiScanPrgOptions &prgOpts);
void create_volume_hash_map(TGeoNavigator &geoNav, Hash2StringMap &hash2PathMap);


int main(int argc, char **argv)
{
   const std::string stiTreeName = "t";
   const std::string geantStepTreeName = "stepping";

   StiScanPrgOptions prgOpts(argc, argv, stiTreeName, geantStepTreeName);
   prgOpts.ProcessOptions();

   // Initialize gGeoManager with geometry from a ROOT file
   TGeoManager::Import(prgOpts.PathToGeometryFile().c_str());

   if (!gGeoManager)
      Fatal(__PRETTY_FUNCTION__, "Cannot load TGeo geometry from %s", prgOpts.PathToGeometryFile().c_str());

   gGeoManager->cd("HALL_1/CAVE_1");
   TGeoNavigator* geoNav = gGeoManager->GetCurrentNavigator();
   assert(geoNav);

   Hash2StringMap hash2PathMap;
   create_volume_hash_map(*geoNav, hash2PathMap);

   TGeaEvent::fgHash2PathMap = &hash2PathMap;

   loop_over_tree(prgOpts);

   return EXIT_SUCCESS;
}


void loop_over_tree(StiScanPrgOptions &prgOpts)
{
   TChain *stiChain       = prgOpts.GetStiTChain();
   TChain *geantStepChain = prgOpts.GetGeantStepChain();

   StiScanRootFile outRootFile(prgOpts, "recreate");

   int nTreeEvents = stiChain->GetEntries();
   int nProcEvents = 0;

   Info("loop_over_tree", "Found tree/chain with N entries: %d", nTreeEvents);

   nTreeEvents = (prgOpts.GetMaxEventsUser() < nTreeEvents) ? prgOpts.GetMaxEventsUser() : nTreeEvents;

   Info("loop_over_tree", "Will process %d events", nTreeEvents);

   StiScanEvent *stiScanEvent = new StiScanEvent();
   stiChain->SetBranchAddress("e.", &stiScanEvent);
   stiChain->SetBranchStatus("e.TStiEvent*", false);
   stiChain->SetBranchStatus("e.TStiEvent.fTStiKalmanTracks*", true);


   // Prepare resources for geant event
   TGeaEvent *geantEvent = new TGeaEvent();

   if (prgOpts.DoGeantStepTree())
      geantStepChain->SetBranchAddress("TGeaEvent", &geantEvent);

   TRandom myRandom;

   Info("loop_over_tree", "Looping over tree/chain...");

   for (int iEvent = 1; iEvent <= nTreeEvents; iEvent++, nProcEvents++)
   {
      if ( nTreeEvents >= 10 && iEvent % int(nTreeEvents*0.1) == 0 )
         Info("loop_over_tree", "Analyzing event %d", iEvent);

      if (myRandom.Rndm() > prgOpts.GetSparsity()) continue;

      stiChain->GetEntry(iEvent-1);

      outRootFile.FillHists(*stiScanEvent, &prgOpts.GetVolumeList());

      if (prgOpts.DoGeantStepTree()) {
         geantStepChain->GetEntry(iEvent-1);
         outRootFile.FillHists(*geantEvent, &prgOpts.GetVolumeList());
      }
   }

   delete stiScanEvent;
   delete geantEvent;

   outRootFile.FillDerivedHists();
   outRootFile.Write();
   outRootFile.Close();
}


void create_volume_hash_map(TGeoNavigator &geoNav, Hash2StringMap &hash2PathMap)
{
   TGeoNode* currNode = geoNav.GetCurrentNode();

   if (!currNode) {
      Warning("create_volume_hash_map", "Invalid TGeoNode provided as input. Skipping...");
      return;
   }

   // Keep track of current level with respect to the original node
   static int level = 0;

   std::string currentPath( geoNav.GetPath() );

   int nDaughters = currNode->GetVolume()->GetNdaughters();

   if (nDaughters) {

      TGeoNode* motherNode = currNode;
      for (int iDaughter = 0; iDaughter < nDaughters; iDaughter++)
      {
         TGeoNode *daughter = motherNode->GetVolume()->GetNode(iDaughter);

         geoNav.CdDown(daughter);
         level++;

         create_volume_hash_map(geoNav, hash2PathMap);
      }

   } else { // We are here if this node is a leaf, i.e. no daughters

      // Add this volume to the hash map
      std::string hashedPath(currentPath);

      // Remove a "TpcRefSys_1/" substring as it not relevant for geometry trees used in simulation
      size_t first_pos = hashedPath.find("TpcRefSys_1/");
      if (first_pos != std::string::npos) {
         hashedPath.replace(first_pos, std::string("TpcRefSys_1/").length(), "");
      }

      std::hash<std::string> hash_fn;
      std::size_t hash_value = hash_fn(hashedPath);

      std::pair<size_t, std::string> hash2Path(hash_value, hashedPath);

      hash2PathMap.insert(hash2Path);

      geoNav.cd(currentPath.c_str());
   }

   if (level > 0) {
      geoNav.CdUp();
      level--;
   }
}
