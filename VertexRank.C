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




void VertexRank(Long64_t nevent = 999999, const char *file = "./*.MuDst.root", const  char *outFile = "test")
{
   TString OutFile(outFile);
   OutFile += ".root";
   TFile fOut(OutFile, "recreate");        //Create the file to save the data

   struct primVtxPoint_t {
      Int_t event, index, rank, mult, refMult, maxmult;
      Float_t primX, primY, primZ, zVpd;
      Float_t McX, McY, McZ;
      Float_t chi2;
      Int_t beam, postx, prompt, cross, tof, notof, EEMC, noEEMC, BEMC, noBEMC;
   };

   primVtxPoint_t primVtx;

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

   TH1I hNumVertices("hNumVertices", "Number of Vertices", 500, 0, 500);
   TH1I hNumTracksPerVertex("hNumTracksPerVertex", "Number of Tracks per Vertex", 100, 0, 100);
   TH1I hVertexX("hVertexX", "Vertex X Position", 60, -10, 10);
   TH1I hVertexY("hVertexY", "Vertex Y Position", 60, -10, 10);
   TH1I hVertexZ("hVertexZ", "Vertex Z Position", 60, -30, 30);
   TH1F hMinDistBetweenVertices("hMinDistBetweenVertices", "Min Distance Between Vertices", 100, 0, 20);
   TH1F h1("h1", "Rank Max", 100, -3, 3);
   TH1F h2("h2", "Max Mult", 100, -3, 3);


   StMuDebug::setLevel(0);
   StMuDstMaker *maker = new StMuDstMaker(0, 0, "", file, "st:MuDst.root", 1e9); // set up maker in read mode
   //                       0,0                        this mean read mode
   //                           dir                    read all files in this directory
   //                               file               bla.lis real all file in this list, if (file!="") dir is ignored
   //                                    filter        apply filter to filenames, multiple filters are separated by ':'
   //                                          10      maximum number of file to read
   maker->SetStatus("*", 0);

   std::vector<std::string> activeBranchNames = {
      "MuEvent",
      "PrimaryVertices",
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

      StMuDst *mu = maker->muDst();   // get a pointer to the StMuDst class, the class that points to all the data
      StMuEvent *muEvent = mu->event(); // get a pointer to the class holding event-wise information

      if (vtxeval::gDebugFlag) std::cout << "Read event #" << ev << "\tRun\t" << muEvent->runId() << "\tId: " << muEvent->eventId() << std::endl;

      TClonesArray *primaryVertices   = mu->primaryVertices();
      int numPrimaryVertices = primaryVertices->GetEntriesFast();

      TClonesArray *MuMcVertices   = mu->mcArray(0);
      int NoMuMcVertices = MuMcVertices->GetEntriesFast();

      ///////VPD////////////
      Float_t VpdZ = 999;
      StBTofHeader *BTofHeader = mu->btofHeader();

      if (BTofHeader) {
         VpdZ = BTofHeader->vpdVz();

         if (TMath::Abs(VpdZ) < 200) primVtx.zVpd = VpdZ;
      }
      else primVtx.zVpd = 999;

      if (vtxeval::gDebugFlag) std::cout << Form("Vpd value:              %8.3f", VpdZ) << std::endl;

      //////Max multiplicity/////////
      /////Usually the correct vertex/////
      int MaxMult = 0;

      for (int l = 0; l < numPrimaryVertices; l++) {
         StMuPrimaryVertex *Vtx = (StMuPrimaryVertex *) primaryVertices->UncheckedAt(l);
         Float_t numTrackPerVertex = Vtx->noTracks();

         hNumTracksPerVertex.Fill(numTrackPerVertex);
         hVertexX.Fill(Vtx->position().x());
         hVertexY.Fill(Vtx->position().y());
         hVertexZ.Fill(Vtx->position().z());

         if (MaxMult < numTrackPerVertex) {                               //Amilkar: check if the numTrackPerVertex is higher than previous
            MaxMult = numTrackPerVertex;                                   //Amilkar: asign the new maximum value
         }
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

      double minDistance = DBL_MAX;

      // Loop over primary verticies in the event
      for (int l = 0; l < numPrimaryVertices; l++) {
         StMuPrimaryVertex *Vtx = (StMuPrimaryVertex *) primaryVertices->UncheckedAt(l);

         if (!Vtx) continue;

         for (int iVertex2 = l+1; iVertex2 < numPrimaryVertices; iVertex2++)
         {
            StMuPrimaryVertex *vertex2 = (StMuPrimaryVertex*) primaryVertices->UncheckedAt(iVertex2);

            if (!vertex2) continue;

            double dist = std::sqrt((Vtx->position().x() - vertex2->position().x()) * (Vtx->position().x() - vertex2->position().x()) +
                                    (Vtx->position().y() - vertex2->position().y()) * (Vtx->position().y() - vertex2->position().y()) +
                                    (Vtx->position().z() - vertex2->position().z()) * (Vtx->position().z() - vertex2->position().z()) );

            if (minDistance < 1) continue;

            minDistance =  (dist < minDistance) ? dist : minDistance;
         }

         primVtx.event   = ev;
         primVtx.mult    = Vtx->noTracks();
         primVtx.refMult = Vtx->refMult();
         primVtx.primX   = Vtx->position().x();
         primVtx.primY   = Vtx->position().y();
         primVtx.primZ   = Vtx->position().z();
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

      hNumVertices.Fill(numPrimaryVertices);
      hMinDistBetweenVertices.Fill(minDistance);

      if ( !gROOT->IsBatch() ) {
         if (vtxeval::ask_user()) return;
      }
      else {vtxeval::gDebugFlag = false;}
   }     //END EVENTS

   std::cout << "Number of events: " <<  nevent << ", with 0 reconstructed verticies: " << noreco << std::endl;

   fOut.Write();

   TDatime now1;
   now1.Print();
}
