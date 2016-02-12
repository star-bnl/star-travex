#include <fstream>
#include <string>

#include "TChain.h"
#include "TError.h"
#include "TGeoNavigator.h"
#include "TGeoManager.h"
#include "TRandom.h"

#include "src-tools/config.h"

#include "StiHify/StiHifyEvent.h"
#include "StiHify/StiHifyPrgOptions.h"
#include "StiHify/StiHifyRootFile.h"



void loop_over_tree(StiHifyPrgOptions &prgOpts);


int main(int argc, char **argv)
{
   const std::string stiTreeName = "t";

   StiHifyPrgOptions prgOpts(argc, argv, stiTreeName);
   prgOpts.ProcessOptions();

   loop_over_tree(prgOpts);

   return EXIT_SUCCESS;
}


void loop_over_tree(StiHifyPrgOptions &prgOpts)
{
   TChain *treeChain = prgOpts.GetStiTChain();

   StiHifyRootFile outRootFile(prgOpts, "recreate");

   int nTreeEvents = treeChain->GetEntries();
   int nProcEvents = 0;

   Info("loop_over_tree", "Found tree/chain with N entries: %d", nTreeEvents);

   StiHifyEvent *stiHifyEvent = new StiHifyEvent();
   treeChain->SetBranchAddress("e.", &stiHifyEvent);
   treeChain->SetBranchStatus("e.*", false);
   treeChain->SetBranchStatus("e.TStiEvent.fTStiKalmanTracks*", true);
   treeChain->SetBranchStatus("e.TStiEvent.fTStiHits*", true);

   TRandom myRandom;

   Info("loop_over_tree", "Looping over tree/chain...");

   for (int iEvent = 1; iEvent <= nTreeEvents; iEvent++, nProcEvents++)
   {
      if ( nTreeEvents >= 10 && iEvent % int(nTreeEvents*0.1) == 0 )
         Info("loop_over_tree", "Analyzing event %d", iEvent);

      if (myRandom.Rndm() > prgOpts.GetSparsity()) continue;

      treeChain->GetEntry(iEvent-1);

      // If requested by the user modify the event read from the input tree such
      // that the closest within certain window hit for each track node is
      // defined as the accepted one
      if (prgOpts.AcceptCandidateHit())
         stiHifyEvent->AssignClosestCandidateHit();

      outRootFile.FillHists(*stiHifyEvent);
   }

   delete stiHifyEvent;

   outRootFile.FillDerivedHists();
   outRootFile.Write();
   outRootFile.Close();
}
