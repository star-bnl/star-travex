#include "TROOT.h"
#include "Riostream.h"
#include "TFile.h"
#include "TMath.h"
#include "TTree.h"
#include "TDatime.h"
#include "TBranch.h"
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/Factory.h"
#include "weights/TMVAClassification_Fisher.class.C"

void SetTreeVariables(TTree* oldtree){
  
  struct primVtxPoint_t{ 
    Int_t event,index,rank,mult,refMult,maxmult;
    Float_t primX,primY,primZ,zVpd;
    Float_t McX, McY, McZ;
    Float_t chi2;
    Int_t beam,postx,prompt,cross,tof,notof,EEMC,noEEMC,BEMC,noBEMC;
  };
  primVtxPoint_t primVtx;
  
  TTree *primaryvtx = new TTree("primaryvtx","The Primary Vertices");
  primaryvtx->SetAutoSave(1000000);
  primaryvtx->Branch("event",&primVtx.event,"event/I");
  primaryvtx->Branch("index",&primVtx.index,"index/I");
  primaryvtx->Branch("rank",&primVtx.rank,"rank/I");
  primaryvtx->Branch("mult",&primVtx.mult,"mult/I");
  primaryvtx->Branch("refMult",&primVtx.refMult,"refMult/I");
  primaryvtx->Branch("maxmult",&primVtx.maxmult,"maxmult/I");
  primaryvtx->Branch("primX",&primVtx.primX,"primX/F");
  primaryvtx->Branch("primY",&primVtx.primY,"primY/F");
  primaryvtx->Branch("primZ",&primVtx.primZ,"primZ/F");
  primaryvtx->Branch("zVpd",&primVtx.zVpd,"zVpd/F");
  primaryvtx->Branch("McX",&primVtx.McX,"McX/F");
  primaryvtx->Branch("McY",&primVtx.McY,"McY/F");
  primaryvtx->Branch("McZ",&primVtx.McZ,"McZ/F");
  primaryvtx->Branch("chi2",&primVtx.chi2,"chi2/F");
  primaryvtx->Branch("beam",&primVtx.beam,"beam/I");
  primaryvtx->Branch("postx",&primVtx.postx,"postx/I");
  primaryvtx->Branch("prompt",&primVtx.prompt,"prompt/I");
  primaryvtx->Branch("cross",&primVtx.cross,"cross/I");
  primaryvtx->Branch("tof",&primVtx.tof,"tof/I");
  primaryvtx->Branch("notof",&primVtx.notof,"notof/I");
  primaryvtx->Branch("EEMC",&primVtx.EEMC,"EEMC/I");
  primaryvtx->Branch("noEEMC",&primVtx.noEEMC,"noEEMC/I");
  primaryvtx->Branch("BEMC",&primVtx.BEMC,"BEMC/I");
  primaryvtx->Branch("noBEMC",&primVtx.noBEMC,"noBEMC/I");
}

void ApplyClassifier(){

  TDatime now;                                          //Set time in Root
  now.Print();
  
  gROOT->cd();                                          //Need to erase the memory so no problem filling the tree
   
  ///////////////////////////////////////TMVA///////////////////////////////////////
  TString separator(":");
  TString Vars ("postx:prompt:cross:tof:notof:BEMC:noBEMC:EEMC:noEEMC");
  TObjArray *array = Vars.Tokenize(separator);
  
  vector<string> inputVars;
  TIter next(array);
  TObjString *objs;
  while ((objs = (TObjString *) next())){
    inputVars.push_back(objs->GetString().Data() );}
  
  vector<double>* inputVec = new vector<double>(inputVars.size());
  
  IClassifierReader     *classReader = new ReadFisher(inputVars);
  //////////////////////////////////////////////////////////////////////////////////  
  
  TFile *input = new TFile("outvertex.root");
  TTree *oldtree = (TTree*)input->Get("primaryvtx");
  SetTreeVariables(oldtree);
  oldtree->SetBranchStatus("*",1);

  TFile *newfile = new TFile("outvertex_tmva","recreate");
  TTree *newtree = oldtree->CloneTree();
  Float_t tmva=-2.1;
  TBranch *Ttmva = newtree->Branch("tmva",&tmva,"tmva/F");
  
  Long64_t nentries = oldtree->GetEntriesFast();

  cout << "Number of events: " << nentries << endl;
  
  for (Long64_t i=0; i<nentries;i++) {
    oldtree->GetEntry(i);
    if( i % 1000000 == 0) cout << "Reading event: " << i << endl;
    
    //////////////////////
    Float_t dataArray1[9] = {primVtx.postx,primVtx.prompt,primVtx.cross,
			     primVtx.tof,primVtx.notof,
			     primVtx.BEMC,primVtx.noBEMC,
			     primVtx.EEMC,primVtx.noEEMC};
    UInt_t N1 = inputVars.size();
    for (UInt_t jjj = 0 ; jjj < N1 ; jjj++) (*inputVec)[jjj] = dataArray1[jjj];
    tmva = classReader->GetMvaValue(*inputVec);
    //////////////////////
    
    Ttmva->Fill();
  } 
  
  newfile->Write();
  //cout << "Number of entries: " << nentries << endl;
  //cout << "Good eventes: " << good << endl;
  TDatime now1;                                          //Set time in Root
  now1.Print();
}
