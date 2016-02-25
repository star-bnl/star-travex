// Vertex QA
//Amilkar Quintero
// July 2015
//   to run > root.exe lMuDst.C 'VertexRank.C+(9999999,"/path/to/files/*MuDst.root")'
// /star/data26/reco/pp500_production_2013/ReversedFullField/P15ic_KFvertex_BL/2013/

#include <cfloat>

#ifndef __CINT__
#include <string>
#include <vector>
#include "Riostream.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TH1.h"
#include "TH2.h"
#include "TNtuple.h"
#include "TTree.h"
#include "TChain.h"
#include "TFile.h"
#include "TString.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StMuDSTMaker/COMMON/StMuPrimaryVertex.h"
#include "StMuDSTMaker/COMMON/StMuMcVertex.h"
#include "StMuDSTMaker/COMMON/StMuDstMaker.h"
#include "TDatime.h"
#include "TMath.h"
#include "StEvent/StBTofHeader.h"
#endif

#include "utils.h"
#include "VertexRank.h"
#include "StarEventHistContainer.h"
#include "StarVertexHistContainer.h"



void VertexRank(Long64_t nevent, const std::string& fileName, const std::string& outFile)
{
   TString OutFile(outFile);
   OutFile += ".root";
   TFile fOut(OutFile, "recreate");        //Create the file to save the data

   VertexData primVtx;

   TTree *primaryvtx = new TTree("primaryvtx", "The Primary Vertices");
   primaryvtx->Branch("event",   &primVtx.event, "event/I");
   primaryvtx->Branch("index",   &primVtx.index, "index/I");
   primaryvtx->Branch("rank",    &primVtx.rank, "rank/I");
   primaryvtx->Branch("mult",    &primVtx.mult, "mult/I");
   primaryvtx->Branch("refMult", &primVtx.refMult, "refMult/I");
   primaryvtx->Branch("maxmult", &primVtx.maxmult, "maxmult/I");
   primaryvtx->Branch("primX",   &primVtx.primX, "primX/F");
   primaryvtx->Branch("primY",   &primVtx.primY, "primY/F");
   primaryvtx->Branch("primZ",   &primVtx.primZ, "primZ/F");
   primaryvtx->Branch("positionError", &primVtx.positionError);
   primaryvtx->Branch("zVpd",    &primVtx.zVpd, "zVpd/F");
   primaryvtx->Branch("McX",     &primVtx.McX, "McX/F");
   primaryvtx->Branch("McY",     &primVtx.McY, "McY/F");
   primaryvtx->Branch("McZ",     &primVtx.McZ, "McZ/F");
   primaryvtx->Branch("chi2",    &primVtx.chi2, "chi2/F");
   primaryvtx->Branch("beam",    &primVtx.beam, "beam/I");
   primaryvtx->Branch("postx",   &primVtx.postx, "postx/I");
   primaryvtx->Branch("prompt",  &primVtx.prompt, "prompt/I");
   primaryvtx->Branch("cross",   &primVtx.cross, "cross/I");
   primaryvtx->Branch("tof",     &primVtx.tof, "tof/I");
   primaryvtx->Branch("notof",   &primVtx.notof, "notof/I");
   primaryvtx->Branch("EEMC",    &primVtx.EEMC, "EEMC/I");
   primaryvtx->Branch("noEEMC",  &primVtx.noEEMC, "noEEMC/I");
   primaryvtx->Branch("BEMC",    &primVtx.BEMC, "BEMC/I");
   primaryvtx->Branch("noBEMC",  &primVtx.noBEMC, "noBEMC/I");

   TH1F h1("h1", "Rank Max", 100, -3, 3);
   TH1F h2("h2", "Max Mult", 100, -3, 3);

   StarEventHistContainer  starEventHistContainer("event", &fOut);
   StarVertexHistContainer starVertexHistContainer("vertex", &fOut);
   StarVertexHistContainer starMaxRankVertexHistContainer("vertex_maxrank", &fOut);


   StMuDebug::setLevel(0);
   StMuDstMaker *maker = new StMuDstMaker(0, 0, "", fileName.c_str(), "st:MuDst.root", 1e9); // set up maker in read mode
   //                       0,0                        this mean read mode
   //                           dir                    read all files in this directory
   //                               file               bla.lis real all file in this list, if (file!="") dir is ignored
   //                                    filter        apply filter to filenames, multiple filters are separated by ':'
   //                                          10      maximum number of file to read
   maker->SetStatus("*", 0);

   std::vector<std::string> activeBranchNames = {
      "MuEvent",
      "PrimaryVertices",
      "PrimaryTracks",
      "GlobalTracks",
      "BTofHit",
      "BTofHeader",
      "StStMuMcVertex"
   };

   for (const auto& branchName : activeBranchNames) maker->SetStatus(branchName.c_str(), 1); // Set Active braches

   StMuDebug::setLevel(0);
   TChain *tree = maker->chain();
   Long64_t nentries = tree->GetEntries();
   nevent = TMath::Min(nevent, nentries);
   std::cout << nentries << " events in chain " << nevent << " will be read." << std::endl;
   tree->SetCacheSize(-1);        //by setting the read cache to -1 we set it to the AutoFlush value when writing
   tree->SetCacheLearnEntries(1); //one entry is sufficient to learn
   tree->SetCacheEntryRange(0, nevent);

   // Log current time
   TDatime now;
   now.Print();

   // Keep track of the number of events with 0 reconstructed verticies
   int noreco = 0;

   // Main loop over events
   for (Long64_t ev = 0; ev < nevent; ev++) {
      if (maker->Make()) break;

      StMuDst *muDst = maker->muDst();   // get a pointer to the StMuDst class, the class that points to all the data
      StMuEvent *muEvent = muDst->event(); // get a pointer to the class holding event-wise information

      if (vtxeval::gDebugFlag) std::cout << "Read event #" << ev << "\tRun\t" << muEvent->runId() << "\tId: " << muEvent->eventId() << std::endl;

      TClonesArray *primaryVertices   = muDst->primaryVertices();
      int numPrimaryVertices = primaryVertices->GetEntriesFast();

      TClonesArray *MuMcVertices   = muDst->mcArray(0);
      int NoMuMcVertices = MuMcVertices->GetEntriesFast();

      ///////VPD////////////
      Float_t VpdZ = 999;
      StBTofHeader *BTofHeader = muDst->btofHeader();

      if (BTofHeader) {
         VpdZ = BTofHeader->vpdVz();

         if (TMath::Abs(VpdZ) < 200) primVtx.zVpd = VpdZ;
      }
      else primVtx.zVpd = 999;

      if (vtxeval::gDebugFlag) std::cout << Form("Vpd value:              %8.3f", VpdZ) << std::endl;

      //////Max multiplicity/////////
      /////Usually the correct vertex/////
      int MaxMult = 0;
      StMuPrimaryVertex *maxRankVertex = 0;
      float vertexMaxRank = -1e10;

      for (int l = 0; l < numPrimaryVertices; l++) {
         StMuPrimaryVertex *Vtx = (StMuPrimaryVertex *) primaryVertices->UncheckedAt(l);
         Float_t numTracksToVertex = Vtx->noTracks();

         starVertexHistContainer.FillHists(*Vtx);

         if (MaxMult < numTracksToVertex) {                               //Amilkar: check if the numTracksToVertex is higher than previous
            MaxMult = numTracksToVertex;                                   //Amilkar: asign the new maximum value
         }

         // Find the highest rank vertex
         if (Vtx->ranking() > vertexMaxRank )
         {
            vertexMaxRank = Vtx->ranking();
            maxRankVertex = Vtx;
         }
      }

      if (maxRankVertex) {
         // Fill vertex hist container for max rank vertex
         starMaxRankVertexHistContainer.FillHists(*maxRankVertex);
      }

      ////////No reconstructed///////////
      if (numPrimaryVertices == 0) {
         noreco++;
         primVtx.event = ev;
         primVtx.zVpd = 999;
         primVtx.mult = 999;
         primVtx.refMult = 999;

         primVtx.primX = 999;
         primVtx.primY = 999;
         primVtx.primZ = 999;
         primVtx.index = 999;
         primVtx.rank = 999;
         primVtx.maxmult = 999;

         if (vtxeval::gDebugFlag) std::cout << "No reconstructed vertex" << std::endl;
      }


      // Loop over primary verticies in the event
      for (int l = 0; l < numPrimaryVertices; l++) {
         StMuPrimaryVertex *Vtx = (StMuPrimaryVertex *) primaryVertices->UncheckedAt(l);

         if (!Vtx) continue;

         primVtx.event   = ev;
         primVtx.mult    = Vtx->noTracks();
         primVtx.refMult = Vtx->refMult();
         primVtx.primX   = Vtx->position().x();
         primVtx.primY   = Vtx->position().y();
         primVtx.primZ   = Vtx->position().z();
         primVtx.positionError  = Vtx->posError();
         primVtx.index   = l;
         primVtx.rank    = Vtx->ranking();

         //////Mc info/////////
         primVtx.McX    = 999;
         primVtx.McY    = 999;
         primVtx.McZ    = 999;
         int idd = Vtx->idTruth();
         StMuMcVertex *mcVertex = 0;

         if (idd > 0 && idd < NoMuMcVertices)
            mcVertex = (StMuMcVertex *) MuMcVertices->UncheckedAt(idd - 1);

         if (mcVertex) {
            primVtx.McX    = mcVertex->XyzV().x();
            primVtx.McY    = mcVertex->XyzV().y();
            primVtx.McZ    = mcVertex->XyzV().z();
         }

         primVtx.beam   =  Vtx->isBeamConstrained() ? 1 : 0;
         primVtx.postx  =  Vtx->nPostXtracks();
         primVtx.prompt =  Vtx->nPromptTracks();
         primVtx.cross  =  Vtx->nCrossCentralMembrane();
         primVtx.tof    = (Vtx->nCTBMatch()     + Vtx->nBTOFMatch());
         primVtx.notof  = (Vtx->nCTBNotMatch()  + Vtx->nBTOFNotMatch());
         primVtx.BEMC   =  Vtx->nBEMCMatch();
         primVtx.noBEMC =  Vtx->nBEMCNotMatch();
         primVtx.EEMC   =  Vtx->nEEMCMatch();
         primVtx.noEEMC =  Vtx->nEEMCNotMatch();
         primVtx.chi2   =  Vtx->chiSquared();

         if (l == 0 && abs(primVtx.primZ - VpdZ) < 3)
            h1.Fill(primVtx.primZ - VpdZ);

         if (MaxMult == primVtx.mult) {
            primVtx.maxmult = 1;

            if (abs(primVtx.primZ - VpdZ) < 3)
               h2.Fill(primVtx.primZ - VpdZ);
         }
         else primVtx.maxmult = 0;

         primaryvtx->Fill();

         if (vtxeval::gDebugFlag) {
            std::cout << Form("[%i]", l) << Form(" %8.3f  %8.3f  %8.3f ", Vtx->position().x(), Vtx->position().y(), Vtx->position().z())
                      << Form("  Rank:%1.0f", Vtx->ranking()) << "    Mult: " << primVtx.mult;

            if (primVtx.maxmult == 1 && l != 0) std::cout << "\t WRONG RANK" << std::endl;
            else std::cout << std::endl;
         }

      }// END VERTICES

      starEventHistContainer.FillHists(*muDst);

      if ( !gROOT->IsBatch() ) {
         if (vtxeval::ask_user()) return;
      }
      else {vtxeval::gDebugFlag = false;}
   }     //END EVENTS

   std::cout << "Number of events: " <<  nevent << ", with 0 reconstructed verticies: " << noreco << std::endl;

   starEventHistContainer.SaveAllAs(std::string(outFile) + "_event");
   starVertexHistContainer.SaveAllAs(std::string(outFile) + "_vertex");
   starMaxRankVertexHistContainer.SaveAllAs(std::string(outFile) + "_vertex_max_rank");

   fOut.Write();

   TDatime now1;
   now1.Print();
}
