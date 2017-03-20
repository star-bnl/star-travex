#include <algorithm>
#include <cfloat>
#include <iostream>
#include <string>

#include "TChain.h"
#include "TClonesArray.h"
#include "TFile.h"
#include "TString.h"

#include "St_db_Maker/St_db_Maker.h"
#include "StEmcRawMaker/StBemcTables.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuDstMaker.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"

#include "StGenericVertexMaker/StGenericVertexFinder.h"
#include "StGenericVertexMaker/StiPPVertex/StPPVertexFinder.h"

#include "travex/ProgramOptions.h"


bool switched_file(const StMuDstMaker& maker);
void process_muDst(tvx::ProgramOptions& prgOpts);



int main(int argc, char **argv)
{
   tvx::ProgramOptions prgOpts(argc, argv);

   prgOpts.ProcessOptions();
   prgOpts.Print();

   process_muDst(prgOpts);

   return EXIT_SUCCESS;
}



void process_muDst(tvx::ProgramOptions& prgOpts)
{
   StMuDstMaker *muDstMaker = new StMuDstMaker(0, 0, "", prgOpts.PathToInputFile().c_str(), "st:MuDst.root", 1e9); // set up maker in read mode
   //                                     0, 0                        this mean read mode
   //                                           dir                    read all files in this directory
   //                                               file               bla.lis real all file in this list, if (file!="") dir is ignored
   //                                                    filter        apply filter to filenames, multiple filters are separated by ':'
   //                                                           10      maximum number of file to read

   // Specify (active) branches to read but first disable all branches
   muDstMaker->SetStatus("*", 0);

   std::vector<std::string> activeBranchNames = {
      "MuEvent",
      "GlobalTracks",
      "CovGlobTrack",
      "EmcTow",
      "BTofHit",
      "BTofHeader",
      "StStMuMcVertex",
      "StStMuMcTrack"
   };

   // Enable selected branches
   for (const auto& branchName : activeBranchNames)
      muDstMaker->SetStatus(branchName.c_str(), 1);

   TChain *muDstChain = muDstMaker->chain();

   unsigned int nEntries      = muDstChain->GetEntries();
   unsigned int nEventsUser   = prgOpts.GetMaxEventsUser();
   unsigned int nEventsToRead = nEventsUser > 0 ? std::min(nEventsUser, nEntries) : nEntries;

   std::cout << nEntries << " events in chain, " << nEventsToRead << " will be read." << std::endl;

   // Create new branch
   TClonesArray  *verticesRefitted = new TClonesArray("StMuPrimaryVertex", 1000);

   TFile* outFile = new TFile(prgOpts.GetOutFileName("vtxreco", ".MuDst.root").c_str(), "RECREATE");
   TTree* muDstTreeOut = muDstMaker->chain()->CloneTree(0);

   muDstTreeOut->Branch("PrimaryVertices", &verticesRefitted, 65536, 99);

   using VertexFit_t = StGenericVertexFinder::VertexFit_t;

   //StGenericVertexFinder* vertexFinder = new StPPVertexFinder(VertexFit_t::NoBeamline);
   StGenericVertexFinder* vertexFinder = new StPPVertexFinder(VertexFit_t::Beamline1D);
   //StGenericVertexFinder* vertexFinder = new StPPVertexFinder(VertexFit_t::Beamline3D);

   vertexFinder->SetDebugLevel(0);
   vertexFinder->Init();

   St_db_Maker* st_db_maker = new St_db_Maker("db", "StarDb", "MySQL:StarDb", "$STAR/StarDb");
   st_db_maker->Init();

   int currRunNumber = 0;

   // Main loop over events
   for (unsigned int iEvent = 0; iEvent < nEventsToRead; iEvent++)
   {
      if ( muDstMaker->Make() ) break;

      // Access all event data
      StMuDst *muDst = muDstMaker->muDst();

      // Access event header-wise information
      StMuEvent *muEvent = muDst->event();

      // Perform new run initialization such as database access and info retrieval
      if (currRunNumber != muEvent->runNumber())
      {
         currRunNumber = muEvent->runNumber();

         st_db_maker->InitRun(currRunNumber);
         vertexFinder->InitRun(currRunNumber, st_db_maker);
      }

      vertexFinder->Fit(*muDst);
      vertexFinder->result(*verticesRefitted); // container refilled

      muDstTreeOut->Fill();

      vertexFinder->Clear();
   }

   muDstTreeOut->Write();
   outFile->Close();

   delete st_db_maker;
   delete vertexFinder;
   delete outFile;
}


bool switched_file(const StMuDstMaker& maker)
{
   static std::string prev_file_name("");
   static std::string curr_file_name("");

   curr_file_name = maker.GetFileName();

   if (curr_file_name != prev_file_name) {
      std::cout << "Processing new file: " << curr_file_name << std::endl;
      prev_file_name = curr_file_name;

      return true;
   }

   return false;
}
