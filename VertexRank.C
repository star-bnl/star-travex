// Vertex QA
//Amilkar Quintero
// July 2015
//   to run > root.exe lMuDst.C 'VertexRank.C+(9999999,"/path/to/files/*MuDst.root")'
// /star/data26/reco/pp500_production_2013/ReversedFullField/P15ic_KFvertex_BL/2013/

#ifndef __CINT__
#include <string>
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
#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "StMuDSTMaker/COMMON/StMuPrimaryVertex.h"
#include "StMuDSTMaker/COMMON/StMuMcVertex.h"
#include "StMuDSTMaker/COMMON/StMuDstMaker.h"
#include "TDatime.h"
#include "TMath.h"
#include "StEvent/StBTofHeader.h"
#endif

static bool gDebugFlag = false;

Bool_t Ask();


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
   primaryvtx->Branch("event", &primVtx.event, "event/I");
   primaryvtx->Branch("index", &primVtx.index, "index/I");
   primaryvtx->Branch("rank", &primVtx.rank, "rank/I");
   primaryvtx->Branch("mult", &primVtx.mult, "mult/I");
   primaryvtx->Branch("refMult", &primVtx.refMult, "refMult/I");
   primaryvtx->Branch("maxmult", &primVtx.maxmult, "maxmult/I");
   primaryvtx->Branch("primX", &primVtx.primX, "primX/F");
   primaryvtx->Branch("primY", &primVtx.primY, "primY/F");
   primaryvtx->Branch("primZ", &primVtx.primZ, "primZ/F");
   primaryvtx->Branch("zVpd", &primVtx.zVpd, "zVpd/F");
   primaryvtx->Branch("McX", &primVtx.McX, "McX/F");
   primaryvtx->Branch("McY", &primVtx.McY, "McY/F");
   primaryvtx->Branch("McZ", &primVtx.McZ, "McZ/F");
   primaryvtx->Branch("chi2", &primVtx.chi2, "chi2/F");
   primaryvtx->Branch("beam", &primVtx.beam, "beam/I");
   primaryvtx->Branch("postx", &primVtx.postx, "postx/I");
   primaryvtx->Branch("prompt", &primVtx.prompt, "prompt/I");
   primaryvtx->Branch("cross", &primVtx.cross, "cross/I");
   primaryvtx->Branch("tof", &primVtx.tof, "tof/I");
   primaryvtx->Branch("notof", &primVtx.notof, "notof/I");
   primaryvtx->Branch("EEMC", &primVtx.EEMC, "EEMC/I");
   primaryvtx->Branch("noEEMC", &primVtx.noEEMC, "noEEMC/I");
   primaryvtx->Branch("BEMC", &primVtx.BEMC, "BEMC/I");
   primaryvtx->Branch("noBEMC", &primVtx.noBEMC, "noBEMC/I");

   TH1I hNumVertices("hNumVertices", "Number of Vertices", 500, 0, 500);
   TH1I hNumTracksPerVertex("hNumTracksPerVertex", "Number of Tracks per Vertex", 100, 0, 100);
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
   const Char_t *ActiveBranches[] = {
      "MuEvent",
      "PrimaryVertices",
      //"PrimaryTracks",
      //"CovPrimTrack",
      //"GlobalTracks",
      "BTofHit",
      "BTofHeader",
      "StStMuMcVertex"
   };

   Int_t Nb = sizeof(ActiveBranches) / sizeof(Char_t *);

   for (Int_t i = 0; i < Nb; i++) maker->SetStatus(ActiveBranches[i], 1); // Set Active braches

   StMuDebug::setLevel(0);
   TChain *tree = maker->chain();
   Long64_t nentries = tree->GetEntries();
   nevent = TMath::Min(nevent, nentries);
   cout << nentries << " events in chain " << nevent << " will be read." << endl;
   //  if (nentries < 100) return;
   tree->SetCacheSize(-1);        //by setting the read cache to -1 we set it to the AutoFlush value when writing
   tree->SetCacheLearnEntries(1); //one entry is sufficient to learn
   tree->SetCacheEntryRange(0, nevent);

   TDatime now;                                          //Set time in Root
   now.Print();

   Int_t noreco = 0;

   for (Long64_t ev = 0; ev < nevent; ev++) {         //START Events
      if (maker->Make()) break;

      StMuDst *mu = maker->muDst();   // get a pointer to the StMuDst class, the class that points to all the data
      StMuEvent *muEvent = mu->event(); // get a pointer to the class holding event-wise information

      if (gDebugFlag) cout << "Read event #" << ev << "\tRun\t" << muEvent->runId() << "\tId: " << muEvent->eventId() << endl;

      TClonesArray *PrimaryVertices   = mu->primaryVertices();
      Int_t NoPrimaryVertices = PrimaryVertices->GetEntriesFast();  // cout << "\tPrimaryVertices " << NoPrimaryVertices;
      //TClonesArray *PrimaryTracks    = mu->array(muPrimary);
      //Int_t NoPrimaryTracks = PrimaryTracks->GetEntriesFast();  // cout << "\tPrimaryTracks " << NoPrimaryTracks;
      //TClonesArray *CovPrimTrack     = mu->covPrimTrack(); // cout << "\tCovPrimTrack " << CovPrimTrack->GetEntriesFast();
      //TClonesArray *GlobalTracks    = mu->array(muGlobal);
      //Int_t NoGlobalTracks = GlobalTracks->GetEntriesFast();  // cout << "\tPrimaryTracks " << NoPrimaryTracks;

      TClonesArray *MuMcVertices   = mu->mcArray(0);
      Int_t NoMuMcVertices = MuMcVertices->GetEntriesFast();

      ///////VPD////////////
      Float_t VpdZ = 999;
      StBTofHeader *BTofHeader = mu->btofHeader();

      if (BTofHeader) {
         VpdZ = BTofHeader->vpdVz();

         if (TMath::Abs(VpdZ) < 200) primVtx.zVpd = VpdZ;
      }
      else primVtx.zVpd = 999;

      if (gDebugFlag) cout << Form("Vpd value:              %8.3f", VpdZ) << endl;

      //////Max multiplicity/////////
      /////Usually the correct vertex/////
      Int_t MaxMult = 0;

      for (Int_t l = 0; l < NoPrimaryVertices; l++) {
         StMuPrimaryVertex *Vtx = (StMuPrimaryVertex *) PrimaryVertices->UncheckedAt(l);
         Float_t Multiplicity = Vtx->noTracks();

         hNumTracksPerVertex.Fill(Multiplicity);

         if (MaxMult < Multiplicity) {                               //Amilkar: check if the multiplicity is higher than previous
            MaxMult = Multiplicity;                                   //Amilkar: asign the new maximum value
         }
      }

      ////////No reconstructed///////////
      if (NoPrimaryVertices == 0) {
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

         if (gDebugFlag) cout << "No reconstructed vertex" << endl;
      }

      for (Int_t l = 0; l < NoPrimaryVertices; l++) {        //START Vertices
         StMuPrimaryVertex *Vtx = (StMuPrimaryVertex *) PrimaryVertices->UncheckedAt(l);

         primVtx.event = ev;

         primVtx.mult = Vtx->noTracks();
         primVtx.refMult = Vtx->refMult();

         primVtx.primX = Vtx->position().x();
         primVtx.primY = Vtx->position().y();
         primVtx.primZ = Vtx->position().z();
         primVtx.index = l;
         primVtx.rank = Vtx->ranking();

         //////Mc info/////////
         primVtx.McX    = 999;
         primVtx.McY    = 999;
         primVtx.McZ    = 999;
         Int_t idd = Vtx->idTruth();
         StMuMcVertex *mcVertex = 0;

         if (idd > 0 && idd < NoMuMcVertices) mcVertex = (StMuMcVertex *) MuMcVertices->UncheckedAt(idd - 1);

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

         if (gDebugFlag) {
            //cout << Form("Vx[%3i]", l) << *Vtx << endl;
            cout << Form("[%i]", l) << Form(" %8.3f  %8.3f  %8.3f ", Vtx->position().x(), Vtx->position().y(), Vtx->position().z()) << Form("  Rank:%1.0f", Vtx->ranking()) << "    Mult: " << primVtx.mult;

            if (primVtx.maxmult == 1 && l != 0) cout << "\t WRONG RANK" << endl;
            else cout << endl;
         }

      }// END VERTICES

      hNumVertices.Fill(NoPrimaryVertices);

      if ( !gROOT->IsBatch() ) {
         if (Ask()) return;
      }
      else {gDebugFlag = false;}

      //if ((ev % 100) == 0) cout << "Number of events: " << ev << endl;
   }     //END EVENTS

   cout << "Number of events: " <<  nevent << "  No reconstructed events: " << noreco << endl;
   fOut.Write();

   TDatime now1;
   now1.Print();
}


/** Get input from user to control amount of print out interactively. */
Bool_t Ask()
{
   static Bool_t fAsk = kTRUE;
   char symbol;

   if (fAsk) {
      std::cout << "ask (enter - next, r - don't ask, q - quit) >";

      do {
         std::cin.get(symbol);

         if (symbol == 'r') {
            gDebugFlag = false;
            fAsk = kFALSE;
         }
         else if (symbol == 'q') return kTRUE;
      }
      while (symbol != '\n');

      std::cout << std::endl;
   }

   return kFALSE;
}
