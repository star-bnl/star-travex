#include <fstream>
#include <string>

#include "TChain.h"
#include "TError.h"
#include "TGeoNavigator.h"
#include "TGeoManager.h"
#include "TRandom.h"
#include "TROOT.h"

#include "StiHify/StiHifyEvent.h"
#include "StiScan/StiScanPrgOptions.h"
#include "StiHify/StiHifyRootFile.h"



void loop_over_tree(StiScanPrgOptions &prgOpts);


int main(int argc, char **argv)
{
   const std::string hftTreeName = "t";

   StiScanPrgOptions prgOpts(argc, argv, hftTreeName);
   prgOpts.ProcessOptions();

   loop_over_tree(prgOpts);

   return EXIT_SUCCESS;
}


void loop_over_tree(StiScanPrgOptions &prgOpts)
{
   TChain *treeChain = prgOpts.GetHftChain();

   // Create a new output file
   std::string outFileName = prgOpts.GetHftreeFile();

   std::string suffix("stihify.root");
   std::size_t suffix_pos = outFileName.find(suffix);

   if (suffix_pos != std::string::npos)
   {
      outFileName.replace( suffix_pos, suffix.length(), "stihify.hist.root");
   } else {
      outFileName += "_stiscan.root";
   }

   StiHifyRootFile outRootFile(prgOpts, outFileName.c_str(), "recreate");

   int nTreeEvents = treeChain->GetEntries();
   int nProcEvents = 0;

   Info("loop_over_tree", "Found tree/chain with N entries: %d", nTreeEvents);

   StiHifyEvent *stiHifyEvent = new StiHifyEvent();
   treeChain->SetBranchAddress("e.", &stiHifyEvent);
   treeChain->SetBranchStatus("e.*", false);
   treeChain->SetBranchStatus("e.TStiEvent.fTStiKalmanTracks*", true);
   treeChain->SetBranchStatus("e.TStiEvent.fTStiHits*", true);

   TRandom myRandom;

   Info("loop_over_tree", "Loop over tree/chain...");

   for (int iEvent = 1; iEvent <= nTreeEvents; iEvent++, nProcEvents++)
   {
      if ( nTreeEvents >= 10 && iEvent % int(nTreeEvents*0.1) == 0 )
         Info("loop_over_tree", "Analyzing event %d", iEvent);

      if (myRandom.Rndm() > prgOpts.GetSparsity()) continue;

      treeChain->GetEntry(iEvent-1);

      outRootFile.FillHists(*stiHifyEvent, &prgOpts.GetVolumeList());
   }

   delete stiHifyEvent;

   outRootFile.FillDerivedHists();

   if (prgOpts.SaveGraphics()) {
      gROOT->Macro(prgOpts.GetStyleMacro().c_str());
      outRootFile.SaveAllAs(prgOpts.GetOutPrefix());
   }

   outRootFile.Write();
   outRootFile.Close();
}
