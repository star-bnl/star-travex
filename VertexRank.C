// Vertex QA
//Amilkar Quintero 
// July 2015
//   to run > root.exe lMuDst.C 'VertexRank.C+(9999999,"/path/to/files/*MuDst.root")'
// /star/data26/reco/pp500_production_2013/ReversedFullField/P15ic_KFvertex_BL/2013/

#if !defined(__CINT__) || defined(__MAKECINT__)
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
#include "StMuDSTMaker/COMMON/StMuDstMaker.h"
#include "TDatime.h"
#include "TMath.h"
#include "StBTofHeader.h"
#endif

#include "Ask.h"


//--->START MAIN PROGRAM
//________________________________________________________________________________
void VertexRank(Long64_t nevent = 999999,const char* file="./*.MuDst.root",const  char* outFile="test") {
  
  gROOT->cd();
  
  TString OutFile(outFile);
  OutFile += ".root";
  TFile *fOut = TFile::Open(OutFile,"recreate");         //Create the file to save the data
  fOut->cd();
  
  //////////////////////////////////////////////////////->Define the variables 
  //static Double_t pionM = 0.13956995;  
  struct primVtxPoint_t{ Float_t event,primX,primY,primZ,MultP,refMultP,l,rank,maxmult,zVpd;};
  primVtxPoint_t primVtx;

  TNtuple *primaryvtx = new TNtuple("primaryvtx","The Primary Vertices","event:primX:primY:primZ:MultP:refMultP:l:rank:maxmult:zVpd");

  TH1F *h1 =  new TH1F("h1","Rank Max",200,-3,3);
  TH1F *h2 =  new TH1F("h2","Max Mult",200,-3,3);
  ///////////////////////////////////////////////////////<-
  
  
  // ----------------------------------------------
  StMuDebug::setLevel(0);  
  StMuDstMaker* maker = new StMuDstMaker(0,0,"",file,"st:MuDst.root",1e9);   // set up maker in read mode
                //                       0,0                        this mean read mode
                //                           dir                    read all files in this directory
                //                               file               bla.lis real all file in this list, if (file!="") dir is ignored
                //                                    filter        apply filter to filenames, multiple filters are separated by ':'
                //                                          10      maximum number of file to read
  maker->SetStatus("*",0);
  const Char_t *ActiveBranches[] = {
    "MuEvent",
    "PrimaryVertices",
    //"PrimaryTracks",
    //"CovPrimTrack",
    //"GlobalTracks",
    "BTofHit",
    "BTofHeader"
  }; 

  Int_t Nb = sizeof(ActiveBranches)/sizeof(Char_t *);
  for (Int_t i = 0; i < Nb; i++) maker->SetStatus(ActiveBranches[i],1); // Set Active braches
  StMuDebug::setLevel(0);  
  TChain *tree = maker->chain();
  Long64_t nentries = tree->GetEntries();
  nevent = TMath::Min(nevent,nentries);
  cout << nentries << " events in chain " << nevent << " will be read." << endl;
  //  if (nentries < 100) return;
  tree->SetCacheSize(-1);        //by setting the read cache to -1 we set it to the AutoFlush value when writing
  tree->SetCacheLearnEntries(1); //one entry is sufficient to learn
  tree->SetCacheEntryRange(0,nevent);

  TDatime now;                                          //Set time in Root
  now.Print();

  Int_t noreco = 0;

  for (Long64_t ev = 0; ev < nevent; ev++) {         //START Events
    if (maker->Make()) break;
    StMuDst* mu = maker->muDst();   // get a pointer to the StMuDst class, the class that points to all the data
    StMuEvent* muEvent = mu->event(); // get a pointer to the class holding event-wise information
    if (_debugAsk) cout << "Read event #" << ev << "\tRun\t" << muEvent->runId() << "\tId: " << muEvent->eventId() << endl; 
    TClonesArray *PrimaryVertices   = mu->primaryVertices(); 
    Int_t NoPrimaryVertices = PrimaryVertices->GetEntriesFast();  // cout << "\tPrimaryVertices " << NoPrimaryVertices;
    //TClonesArray *PrimaryTracks    = mu->array(muPrimary);  
    //Int_t NoPrimaryTracks = PrimaryTracks->GetEntriesFast();  // cout << "\tPrimaryTracks " << NoPrimaryTracks;
    //TClonesArray *CovPrimTrack     = mu->covPrimTrack(); // cout << "\tCovPrimTrack " << CovPrimTrack->GetEntriesFast();
    //TClonesArray *GlobalTracks    = mu->array(muGlobal);  
    //Int_t NoGlobalTracks = GlobalTracks->GetEntriesFast();  // cout << "\tPrimaryTracks " << NoPrimaryTracks;
  
    ///////VPD////////////
    Float_t VpdZ =999;
    StBTofHeader *BTofHeader = mu->btofHeader();
    if (BTofHeader){
      VpdZ = BTofHeader->vpdVz();
      if (TMath::Abs(VpdZ) < 200) primVtx.zVpd = VpdZ;
      } else primVtx.zVpd = 999;
    if (_debugAsk) cout << Form("Vpd value:              %8.3f",VpdZ) << endl;
    /////////////////////

    //////Max multiplicity/////////
    /////Usually the correct vertex/////
    Int_t MaxMult = 0;
    for (Int_t l = 0; l < NoPrimaryVertices; l++) {
      StMuPrimaryVertex *Vtx = (StMuPrimaryVertex *) PrimaryVertices->UncheckedAt(l);
      Float_t Multiplicity = Vtx->noTracks();
      if(MaxMult < Multiplicity) {                                //Amilkar: check if the multiplicity is higher than previous
	MaxMult = Multiplicity;                                   //Amilkar: asign the new maximum value
      }                                                           
    }
    //////////////////////

    ////////No reconstructed///////////
    if (NoPrimaryVertices == 0){
      noreco++;
      primVtx.event = ev;
      primVtx.zVpd = 999;
      primVtx.MultP = 999;
      primVtx.refMultP = 999;
      
      primVtx.primX = 999;
      primVtx.primY = 999;
      primVtx.primZ = 999;
      primVtx.l = 999;
      primVtx.rank = 999;
      primVtx.maxmult = 999;
      primaryvtx->Fill(&primVtx.event);
      if (_debugAsk) cout << "No reconstructed vertex" << endl; 
    }
    ////////////////////////////////

    for (Int_t l = 0; l < NoPrimaryVertices; l++) {        //START Vertices
      StMuPrimaryVertex *Vtx = (StMuPrimaryVertex *) PrimaryVertices->UncheckedAt(l);
            
      primVtx.event = ev;
      
      primVtx.MultP = Vtx->noTracks();
      primVtx.refMultP = Vtx->refMult();
      
      primVtx.primX = Vtx->position().x();
      primVtx.primY = Vtx->position().y();
      primVtx.primZ = Vtx->position().z();
      primVtx.l = l;
      primVtx.rank = Vtx->ranking();

      if (l==0 && abs(primVtx.primZ-VpdZ)<3) h1->Fill(primVtx.primZ-VpdZ);
      if(MaxMult==primVtx.MultP) {primVtx.maxmult = 1; if (abs(primVtx.primZ-VpdZ)<3) h2->Fill(primVtx.primZ-VpdZ);}
      else primVtx.maxmult = 0;
      
      primaryvtx->Fill(&primVtx.event);

      if (_debugAsk) {
	//cout << Form("Vx[%3i]", l) << *Vtx << endl;
	cout << Form("[%i]",l) <<Form(" %8.3f  %8.3f  %8.3f ",Vtx->position().x(),Vtx->position().y(),Vtx->position().z()) << Form("  Rank:%1.0f",Vtx->ranking()) << "    MultP: " << primVtx.MultP ;
	if (primVtx.maxmult == 1 && l!=0) cout << "\t WRONG RANK"<< endl;
	else cout << endl;
      }
 
    }// END VERTICES

    if (! gROOT->IsBatch()) {
      if (Ask()) return;
    } else {_debugAsk = 0;}
    //if ((ev % 100) == 0) cout << "Number of events: " << ev << endl;
  }     //END EVENTS

  cout << "Number of events: " <<  nevent << "  No reconstructed events: " << noreco << endl; 
  fOut->Write();
  
  TDatime now1;
  now1.Print();
  //vertexall->Write();
 
  fOut->Close();      //Need to be used for condor
}
