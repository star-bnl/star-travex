/*
   root.exe lMuDst.C MuMcPrV.C+
*/
#ifndef __CINT__
#include <map>
#include <utility>
#include <cstdlib>
#include <string>
#include <vector>
#include <boost/algorithm/string/replace.hpp>

#include "TROOT.h"
#include "TSystem.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TChain.h"
#include "TFile.h"
#include "TString.h"
#include "TObjString.h"
#include "TArrayF.h"
#include "TArrayD.h"
#include "TMath.h"
#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#include "StMuDSTMaker/COMMON/StMuTimer.h"
#include "StMuDSTMaker/COMMON/StMuDebug.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "StMuDSTMaker/COMMON/StMuPrimaryVertex.h"
#include "StMuDSTMaker/COMMON/StMuMcVertex.h"
#include "StMuDSTMaker/COMMON/StMuMcTrack.h"
#include "StMuDSTMaker/COMMON/StMuPrimaryTrackCovariance.h"
#include "utils.h"
#include "StMuDSTMaker/COMMON/StMuDstMaker.h"
#endif

#ifdef __TMVA__
#include "TMVAClassification_BDT.class.C"
#endif /* __TMVA__ */

struct data_t {Float_t  beam, postx, prompt, cross, tof, notof, EEMC, noEEMC, chi2;};
const Char_t *vnames = "beam:postx:prompt:cross:tof:notof:EEMC:noEEMC:chi2";



void FillData(data_t &data, const StMuPrimaryVertex *Vtx)
{
   memset(&data.beam, 0, sizeof(data_t));
   //memset(&data.postx, 0, sizeof(data_t));    //PPV
   data.beam   =  Vtx->isBeamConstrained() ? 1 : 0;
   data.postx  =  Vtx->nPostXtracks();
   data.prompt =  Vtx->nPromptTracks();
   data.cross  =  Vtx->nCrossCentralMembrane();
   data.tof    = (Vtx->nCTBMatch()     + Vtx->nBTOFMatch());
   data.notof  = (Vtx->nCTBNotMatch()  + Vtx->nBTOFNotMatch());
   data.EEMC   =  Vtx->nEEMCMatch();
   data.noEEMC =  Vtx->nEEMCNotMatch();
   data.chi2   =  Vtx->chiSquared();
}


//________________________________________________________________________________
Bool_t Accept(const StMuTrack *gTrack = 0)
{
   if (! gTrack)            return kFALSE;
   if (! gTrack->idTruth()) return kFALSE;
   if (! gTrack->charge())  return kFALSE;
   if (  gTrack->flag() < 100 ||  gTrack->flag() % 100 == 11) return kFALSE; // bad fit or short track pointing to EEMC
   if (  gTrack->flag() > 1000) return kFALSE;  // pile up track in TPC
   if (  gTrack->nHitsFit() < 10) return kFALSE;

   return kTRUE;
}


//________________________________________________________________________________
Bool_t AcceptVX(const StMuPrimaryVertex *Vtx = 0)
{
   if (! Vtx) return kFALSE;
   if (! Vtx->idTruth())  return kFALSE;
   if (  Vtx->position().perp() > 3.0) return kFALSE;

   return kTRUE;
}


//________________________________________________________________________________
void ForceAnimate(unsigned int times = 0, int msecDelay = 0)
{
   unsigned int  counter = times;

   while ( (!times || counter) && !gSystem->ProcessEvents()) { --counter; if (msecDelay) gSystem->Sleep(msecDelay);}
}


/**
 * 1. Data sample : pp200 W->e nu with  pile-up corresponding to 1 MHz min. bias
 * events, 50 K event y2011, 10 K event y2012.
 *
 * 2. Proof of principal: no pile-up for both PPV and KFV
 *
 *   a.  Reconstructed primary track multiplicity versus corresponding MC
 *   "reconstructable" (i.e. in n STAR acceptance,no. TPC MC hits >= 15)  tracks
 *   multiplicity.
 *
 *   b.  Corrected reconstructed primary track multiplicity (i.e. multiplied by
 *   QA/100.) versus corresponding MC "reconstructable"  (i.e. in n STAR
 *   acceptance,no. TPC MC hits >= 15)  tracks multiplicity.
 *
 *   c.  Efficiency primary vertex reconstruction versus  MC "reconstructable"
 *   tracks multiplicity.
 *
 * 3. With pileup. repeat above (a-c) with old ranking scheme for
 *
 *     I. Any reconstructed primary vertex which is matched with MC trigger
 *     vertex (MC = 1)
 *
 *    II. The best (in sense of ranking) reconstructed primary vertex which is
 *    matched with MC trigger vertex (MC = 1)
 *
 *   III. The best (in sense of ranking) reconstructed primary vertex which is
 *   not matched with MC trigger vertex (MC != 1)
 *
 * 4. With pileup. repeat above (a-c) with new ranking scheme for cases I-III
 */
void MuMcPrVKFV2012(Long64_t nevent, const char *file, const std::string& outFile, bool fillNtuple)
{
#ifdef __TMVA__
   boost::replace_last(outFile, ".root", "");
   outFile += ".TMVArank.root";

   // create a set of variables and declare them to the reader
   // - the variable names must corresponds in name and type to
   // those given in the weight file(s) that you use
   TString separator(":");
   TString Vnames(vnames);
   TObjArray *array = Vnames.Tokenize(separator);

   std::vector<std::string> inputVars;
   TIter next(array);
   TObjString *objs;

   while ((objs = (TObjString *) next())) {
      std::cout << objs->GetString() << std::endl;
   }

   inputVars.push_back("beam");
   inputVars.push_back("postx");
   inputVars.push_back("prompt");
   inputVars.push_back("cross");
   inputVars.push_back("tof");
   inputVars.push_back("notof");
   inputVars.push_back("EEMC");
   inputVars.push_back("noEEMC");
   inputVars.push_back("chi2");

   std::vector<double> *inputVec = new std::vector<double>( inputVars.size() );
   IClassifierReader *classReader = new ReadBDT( inputVars );

#endif /* __TMVA__ */

   TFile *fOut = TFile::Open(outFile.c_str(), "recreate");
   data_t data;

   // Book histograms
   const int nMcRecMult = 75;
   TArrayD xMult(nMcRecMult + 1);
   xMult[0] = -0.5;

   for (int i = 1; i <= nMcRecMult; i++) {
      if      (xMult[i - 1] <  50) xMult[i] = xMult[i - 1] +   1; //  1 - 50
      else if (xMult[i - 1] < 100) xMult[i] = xMult[i - 1] +   2; // 51 - 75
      else if (xMult[i - 1] < 200) xMult[i] = xMult[i - 1] +  10; // 76 - 85
      else                         xMult[i] = xMult[i - 1] + 100; // 86 -100
   }

   TH1D *McRecMulT = new TH1D("McRecMulT", "Reconstructable multiplicity for trigger Mc Vertex", nMcRecMult, xMult.GetArray());
   struct Name_t {
      const Char_t *Name;
      const Char_t *Title;
   };
   const Name_t HCases[3] = {
      {"Any", "Any vertex matched with MC == 1"},
      {"Good", "The best rank vertex with MC == 1"},
      {"Bad", "The best rank vertex with MC != 1"}
   };
   const Name_t Plots[4] = {
      {"Mult"    , "the reconstructed (uncorrected) track multiplicity versus Reconstructable multiplicity"},
      {"MultQA"  , "the reconstructed (corrected for QA) track multiplicity versus Reconstructable multiplicity"},
      {"McRecMul", "Reconstructable multiplicity"},
      {"YvsX"    , "Bad versus Good value"}
   };
   /*         h  p  */
   TH1 *hists[3][4];

   for (int h = 0; h < 3; h++) {
      for (int p = 0; p < 4; p++) {
         TString Name(Plots[p].Name); Name += HCases[h].Name;
         TString Title(Plots[p].Title); Title += " for "; Title += HCases[h].Title; Title += " vertex";

         if      (p <  2)  hists[h][p] = new TH2D(Name, Title, nMcRecMult, xMult.GetArray(), nMcRecMult, xMult.GetArray());
         else if (p == 2)  hists[h][p] = new TH1D(Name, Title, nMcRecMult, xMult.GetArray());
      }
   }

   TNtuple *VertexG = new TNtuple("VertexG", "good vertex & global params info", vnames);
   TNtuple *VertexB = new TNtuple("VertexB", "bad  vertex & global params info", vnames);
   // ----------------------------------------------
   StMuTimer timer;
   timer.start();
   StMuDebug::setLevel(0);
   StMuDstMaker *maker = new StMuDstMaker(0, 0, "", file, "st:MuDst.root", 1e9); // set up maker in read mode
   //                       0,0                        this mean read mode
   //                           dir                    read all files in this directory
   //                               file               bla.lis real all file in this list, if (file!="") dir is ignored
   //                                    filter        apply filter to filenames, multiple filters are separated by ':'
   //                                          10      maximum number of file to read
   std::cout << "time to load chain: " << timer.elapsedTime() << std::endl;
   maker->SetStatus("*", 0);

   std::vector<std::string> activeBranchNames = {
      "MuEvent",
      "PrimaryVertices",
      "StStMuMcVertex",
      "StStMuMcTrack"
   };

   // Set Active braches
   for (const auto& branchName : activeBranchNames)
      maker->SetStatus(branchName.c_str(), 1);

   StMuDebug::setLevel(0);
   timer.reset();
   timer.start();
   TChain *tree = maker->chain();
   Long64_t nentries = tree->GetEntries();
   nevent = TMath::Min(nevent, nentries);
   std::cout << nentries << " events in chain " << nevent << " will be read." << std::endl;
   tree->SetCacheSize(-1);        //by setting the read cache to -1 we set it to the AutoFlush value when writing
   tree->SetCacheLearnEntries(1); //one entry is sufficient to learn
   tree->SetCacheEntryRange(0, nevent);

   for (Long64_t ev = 0; ev < nevent; ev++) {
      if (maker->Make()) break;

      StMuDst *muDst = maker->muDst();   // get a pointer to the StMuDst class, the class that points to all the data
      StMuEvent *muEvent = muDst->event(); // get a pointer to the class holding event-wise information
      int referenceMultiplicity = muEvent->refMult(); // get the reference multiplicity

      TClonesArray *PrimaryVertices   = muDst->primaryVertices();
      int nPrimaryVertices = PrimaryVertices->GetEntriesFast();

      TClonesArray *MuMcVertices   = muDst->mcArray(0);
      int nMuMcVertices = MuMcVertices->GetEntriesFast();

      TClonesArray *MuMcTracks     = muDst->mcArray(1);
      int nMuMcTracks = MuMcTracks->GetEntriesFast();

      if ( nevent >= 10 && ev % int(nevent*0.1) == 0 )
      {
         std::cout << "Event #" << ev << "\tRun\t" << muEvent->runId()
                   << "\tId: " << muEvent->eventId()
                   << " refMult= " << referenceMultiplicity
                   << "\tPrimaryVertices " << nPrimaryVertices
                   << "\t" << StMuArrays::mcArrayTypes[0] << " " << nMuMcVertices
                   << "\t" << StMuArrays::mcArrayTypes[1] << " " << nMuMcTracks
                   << std::endl;
      }

      //    const Double_t field = muEvent->magneticField()*kilogauss;
      if (! nMuMcVertices || ! nMuMcTracks || nPrimaryVertices <= 0) {
         std::cout << "Ev. " << ev << " has no MC information ==> skip it" << std::endl;
         std::cout << "OR no reconstructed verticies found" << std::endl;
         continue;
      }

      // Count number of MC tracks at a vertex with TPC reconstructable tracks
      std::multimap<int, int> Mc2McHitTracks;

      for (int m = 0; m < nMuMcTracks; m++) {
         StMuMcTrack *McTrack = (StMuMcTrack *) MuMcTracks->UncheckedAt(m);

         if (McTrack->No_tpc_hit() < 15) continue;

         Mc2McHitTracks.insert(std::pair<int, int>(McTrack->IdVx(), McTrack->Id()));
      }

      // This is the "reconstructable" track multiplicity
      int nMcTracksWithHits = Mc2McHitTracks.count(1);

      // Let's skip events in which we do not expect to reconstruct any tracks
      // (and thus vertex) from the primary vertex
      if (nMcTracksWithHits <= 0) continue;

      // This is our denominator histogram for efficiencies
      McRecMulT->Fill(nMcTracksWithHits);

      // =============  Build map between  Rc and Mc vertices
      std::map<StMuPrimaryVertex *, StMuMcVertex *> reco2McVertices;
      TArrayF vertexRanks(nPrimaryVertices);
      int mcMatchedVertexIndex  = -1; // any vertex with MC==1 and highest reconstrated multiplicity.
      int vertexMaxMultiplicity = -1;

      // First loop over all verticies in this event. There is at least one
      // must be available
      for (int recoVertexIndex = 0; recoVertexIndex < nPrimaryVertices; recoVertexIndex++)
      {
         vertexRanks[recoVertexIndex] = -1e10;

         StMuPrimaryVertex *recoVertex = (StMuPrimaryVertex *) PrimaryVertices->UncheckedAt(recoVertexIndex);

         if ( !AcceptVX(recoVertex) ) continue;

         // Check Mc
         if (recoVertex->idTruth() < 0 || recoVertex->idTruth() > nMuMcVertices) {
            std::cout << "ERROR: Illegal idTruth " << recoVertex->idTruth() << " The track is ignored" << std::endl;
            continue;
         }

         StMuMcVertex *mcVertex = (StMuMcVertex *) MuMcVertices->UncheckedAt(recoVertex->idTruth() - 1);

         if (mcVertex->Id() != recoVertex->idTruth()) {
            std::cout << "ERROR: Mismatched idTruth " << recoVertex->idTruth() << " and mcVertex Id " <<  mcVertex->Id()
                 << " The vertex is ignored" <<  std::endl;
            continue;
         }

         reco2McVertices[recoVertex] = mcVertex;
         vertexRanks[recoVertexIndex] = recoVertex->ranking();

         if (recoVertex->idTruth() == 1 && recoVertex->noTracks() > vertexMaxMultiplicity)
         {
            mcMatchedVertexIndex  = recoVertexIndex;
            vertexMaxMultiplicity = recoVertex->noTracks();
         }

         FillData(data, recoVertex);

#ifdef __TMVA__
         Float_t *dataArray = &data.beam;

         for (size_t j = 0; j < inputVec->size(); j++)
            (*inputVec)[j] = dataArray[j];

         vertexRanks[recoVertexIndex] = classReader->GetMvaValue( *inputVec );
#endif
      }

      // If we reconstructed a vertex which matches the MC one we fill the
      // numerator of the "Any" efficiency histogram
      if (mcMatchedVertexIndex != -1) {

         StMuPrimaryVertex *recoVertexMatchedMc = (StMuPrimaryVertex*) PrimaryVertices->UncheckedAt(mcMatchedVertexIndex);

         double nTracks = recoVertexMatchedMc->noTracks();
         double nTracksQA = nTracks * recoVertexMatchedMc->qaTruth() / 100.;

         hists[0][0]->Fill(nMcTracksWithHits, nTracks);
         hists[0][1]->Fill(nMcTracksWithHits, nTracksQA);
         hists[0][2]->Fill(nMcTracksWithHits);
      }

      // Now deal with the highest rank vertex
      int maxRankVertexIndex = TMath::LocMax(nPrimaryVertices, vertexRanks.GetArray());

      StMuPrimaryVertex *recoVertexMaxRank = (StMuPrimaryVertex*) PrimaryVertices->UncheckedAt(maxRankVertexIndex);
      StMuMcVertex *mcVertex = reco2McVertices[recoVertexMaxRank];

      double nTracks = recoVertexMaxRank->noTracks();
      double nTracksQA = nTracks * recoVertexMaxRank->qaTruth() / 100.;

      // Fill numerator for "good" and "bad" efficiencies
      int h = ( mcVertex && mcVertex->Id() == 1) ? 1 : 2;

      hists[h][0]->Fill(nMcTracksWithHits, nTracks);
      hists[h][1]->Fill(nMcTracksWithHits, nTracksQA);
      hists[h][2]->Fill(nMcTracksWithHits);


      // Proceed with filling ntuple only if requested by the user
      if ( !fillNtuple ) continue;


      // Second loop over all verticies in this event
      for (int recoVertexIndex = 0; recoVertexIndex < nPrimaryVertices; recoVertexIndex++)
      {
         StMuPrimaryVertex *recoVertex = (StMuPrimaryVertex *) PrimaryVertices->UncheckedAt(recoVertexIndex);

         if ( !AcceptVX(recoVertex) ) continue;

         StMuMcVertex *mcVertex = reco2McVertices[recoVertex];

         if ( !mcVertex ) {
            std::cout << "No Match from RC to MC" << std::endl;
            continue;
         }

         if (vtxeval::gDebugFlag) {
            std::cout << Form("Vx[%3i]", recoVertexIndex) << *recoVertex << " " << *mcVertex;
            int nMcTracksWithHitsatL = Mc2McHitTracks.count(recoVertex->idTruth());
            std::cout << Form("Number of McTkHit %4i rank %8.3f", nMcTracksWithHitsatL, vertexRanks[recoVertexIndex]);
         }

         int IdPar = mcVertex->IdParTrk();

         if (IdPar > 0 && IdPar <= nMuMcTracks) {
            StMuMcTrack *mcTrack = (StMuMcTrack *) MuMcTracks->UncheckedAt(IdPar - 1);

            if (mcTrack && vtxeval::gDebugFlag) std::cout << " " << mcTrack->GeName();
         }

         FillData(data, recoVertex);

         double nTracks = recoVertex->noTracks();

         if (mcVertex->Id() == 1 && nTracks == vertexMaxMultiplicity) {// good
            VertexG->Fill(&data.beam);
         }
         else {   // bad
            VertexB->Fill(&data.beam);
         }
      }

      if ( !gROOT->IsBatch() ) {
         if (vtxeval::ask_user()) return;
      }
      else { vtxeval::gDebugFlag = false; }
   }

   fOut->Write();
}


//+++++++++++++++++++++ from Jonathan +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/**********************************************************************************
 * Project   : TMVA - a ROOT-integrated toolkit for multivariate data analysis    *
 * Package   : TMVA                                                               *
 * Root Macro: TMVAClassification                                                 *
 *                                                                                *
 * This macro provides examples for the training and testing of the               *
 * TMVA classifiers.                                                              *
 *                                                                                *
 * As input data is used a toy-MC sample consisting of four Gaussian-distributed  *
 * and linearly correlated input variables.                                       *
 *                                                                                *
 * The methods to be used can be switched on and off by means of booleans, or     *
 * via the prompt command, for example:                                           *
 *                                                                                *
 *    root -l ./TMVAClassification.C\(\"Fisher,Likelihood\"\)                     *
 *                                                                                *
 * (note that the backslashes are mandatory)                                      *
 * If no method given, a default set of classifiers is used.                      *
 *                                                                                *
 * The output file "TMVA.root" can be analysed with the use of dedicated          *
 * macros (simply say: root -l <macro.C>), which can be conveniently              *
 * invoked through a GUI that will appear at the end of the run of this macro.    *
 * Launch the GUI via the command:                                                *
 *                                                                                *
 *    root -l ./TMVAGui.C                                                         *
 *                                                                                *
 **********************************************************************************/
void TMVAClassification( TString myMethodList = "")
{
   TTree *signal     = (TTree *)gDirectory->Get("VertexG");

   if (! signal) { std::cout << "No signal TTree" << std::endl; return;}

   TTree *background = (TTree *)gDirectory->Get("VertexB");

   if (! background) { std::cout << "No background TTree" << std::endl; return;}

   //---------------------------------------------------------------
   // This loads the library
   TMVA::Tools::Instance();

   // Default MVA methods to be trained + tested
   std::map<std::string, int> Use;

   // --- Cut optimisation
   Use["Cuts"]            = 1;
   Use["CutsD"]           = 1;
   Use["CutsPCA"]         = 0;
   Use["CutsGA"]          = 0;
   Use["CutsSA"]          = 0;
   //
   // --- 1-dimensional likelihood ("naive Bayes estimator")
   Use["Likelihood"]      = 1;
   Use["LikelihoodD"]     = 0; // the "D" extension indicates decorrelated input variables (see option strings)
   Use["LikelihoodPCA"]   = 1; // the "PCA" extension indicates PCA-transformed input variables (see option strings)
   Use["LikelihoodKDE"]   = 0;
   Use["LikelihoodMIX"]   = 0;
   //
   // --- Mutidimensional likelihood and Nearest-Neighbour methods
   Use["PDERS"]           = 1;
   Use["PDERSD"]          = 0;
   Use["PDERSPCA"]        = 0;
   Use["PDEFoam"]         = 1;
   Use["PDEFoamBoost"]    = 0; // uses generalised MVA method boosting
   Use["KNN"]             = 1; // k-nearest neighbour method
   //
   // --- Linear Discriminant Analysis
   Use["LD"]              = 1; // Linear Discriminant identical to Fisher
   Use["Fisher"]          = 0;
   Use["FisherG"]         = 0;
   Use["BoostedFisher"]   = 0; // uses generalised MVA method boosting
   Use["HMatrix"]         = 0;
   //
   // --- Function Discriminant analysis
   Use["FDA_GA"]          = 1; // minimisation of user-defined function using Genetics Algorithm
   Use["FDA_SA"]          = 0;
   Use["FDA_MC"]          = 0;
   Use["FDA_MT"]          = 0;
   Use["FDA_GAMT"]        = 0;
   Use["FDA_MCMT"]        = 0;
   //
   // --- Neural Networks (all are feed-forward Multilayer Perceptrons)
   Use["MLP"]             = 0; // Recommended ANN
   Use["MLPBFGS"]         = 0; // Recommended ANN with optional training method
   Use["MLPBNN"]          = 1; // Recommended ANN with BFGS training method and bayesian regulator
   Use["CFMlpANN"]        = 0; // Depreciated ANN from ALEPH
   Use["TMlpANN"]         = 0; // ROOT's own ANN
   //
   // --- Support Vector Machine
   Use["SVM"]             = 1;
   //
   // --- Boosted Decision Trees
   Use["BDT"]             = 1; // uses Adaptive Boost
   Use["BDTG"]            = 0; // uses Gradient Boost
   Use["BDTB"]            = 0; // uses Bagging
   Use["BDTD"]            = 0; // decorrelation + Adaptive Boost
   Use["BDTF"]            = 0; // allow usage of fisher discriminant for node splitting
   Use["myBDTD"]          = 1; // mine
   //
   // --- Friedman's RuleFit method, ie, an optimised series of cuts ("rules")
   Use["RuleFit"]         = 1;
   // ---------------------------------------------------------------

   std::cout << std::endl;
   std::cout << "==> Start TMVAClassification" << std::endl;

   // Select methods (don't look at this code - not of interest)
   if (myMethodList != "") {
      for (std::map<std::string, int>::iterator it = Use.begin(); it != Use.end(); it++) it->second = 0;

      std::vector<TString> mlist = TMVA::gTools().SplitString( myMethodList, ',' );

      for (size_t i = 0; i < mlist.size(); i++) {
         std::string regMethod(mlist[i]);

         if (Use.find(regMethod) == Use.end()) {
            std::cout << "Method \"" << regMethod << "\" not known in TMVA under this name. Choose among the following:" << std::endl;

            for (std::map<std::string, int>::iterator it = Use.begin(); it != Use.end(); it++) std::cout << it->first << " ";

            std::cout << std::endl;
            return;
         }

         Use[regMethod] = 1;
      }
   }

   // --------------------------------------------------------------------------------------------------

   // --- Here the preparation phase begins

   // Create a ROOT output file where TMVA will store ntuples, histograms, etc.
   TString outfileName( "TMVA.root" );
   TFile *outputFile = TFile::Open( outfileName, "RECREATE" );

   // Create the factory object. Later you can choose the methods
   // whose performance you'd like to investigate. The factory is
   // the only TMVA object you have to interact with
   //
   // The first argument is the base of the name of all the
   // weightfiles in the directory weight/
   //
   // The second argument is the output file for the training results
   // All TMVA output can be suppressed by removing the "!" (not) in
   // front of the "Silent" argument in the option string
   TMVA::Factory *factory = new TMVA::Factory( "TMVAClassification", outputFile,
         "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification" );

   // If you wish to modify default settings
   // (please check "src/Config.h" to see all available global options)
   //    (TMVA::gConfig().GetVariablePlotting()).fTimesRMS = 8.0;
   //    (TMVA::gConfig().GetIONames()).fWeightFileDir = "myWeightDirectory";

   // load the signal and background event samples from ROOT trees

   std::cout << " starts ... " << std::endl;
   // global event weights per tree (see below for setting event-wise weights)
   //   Float_t w;
   double signalWeight     = 1.0;
   double backgroundWeight = 1.0;

   std::cout << " signalWeight = " << signalWeight << " backWeight = " << backgroundWeight << std::endl;
   factory->AddSignalTree( signal,    signalWeight     );
   factory->AddBackgroundTree( background, backgroundWeight );

   TString separator(":");
   TString Vnames(vnames);
   TObjArray *array = Vnames.Tokenize(separator);

   std::vector<std::string> inputVars;
   TIter next(array);
   TObjString *objs;

   while ((objs = (TObjString *) next())) {
      //    std::cout << objs->GetString() << std::endl;
      TString name(objs->GetString());

      if (name == "BEMC") continue;

      if (name == "noBEMC") continue;

      factory->AddVariable(name, 'F');
   }

   // This would set individual event weights (the variables defined in the
   // expression need to exist in the original TTree)
   //    for signal    : factory->SetSignalWeightExpression("weight1*weight2");
   //    for background: factory->SetBackgroundWeightExpression("weight1*weight2");
   // commented JB : 04/26 ??
   //factory->dSetBackgroundWeightExpression("weight");

   // Apply additional cuts on the signal and background samples (can be different)
   TCut mycuts = "";
   TCut mycutb = "";

   // Tell the factory how to use the training and testing events
   //
   // If no numbers of events are given, half of the events in the tree are used
   // for training, and the other half for testing:
   //    factory->PrepareTrainingAndTestTree( mycut, "SplitMode=random:!V" );
   // To also specify the number of testing events, use:
   //factory->PrepareTrainingAndTestTree( mycuts,mycutb,"NSigTrain=9000:NBkgTrain=50000:NSigTest=9000:NBkgTest=50000:SplitMode=Random:!V" );
   factory->PrepareTrainingAndTestTree( mycuts, mycutb, "nTrain_Signal=4900:nTrain_Background=49000:nTest_Signal=4900:nTest_Background=49000:SplitMode=Random:!V"); // for KFVertex
   //   factory->PrepareTrainingAndTestTree( mycuts, mycutb,"nTrain_Signal=20000:nTrain_Background=40000:nTest_Signal=20000:nTest_Background=40000:SplitMode=Random:!V"); // for PPV

   // ---- Book MVA methods
   //
   // Please lookup the various method configuration options in the corresponding cxx files, eg:
   // src/MethoCuts.cxx, etc, or here: http://tmva.sourceforge.net/optionRef.html
   // it is possible to preset ranges in the option string in which the cut optimisation should be done:
   // "...:CutRangeMin[2]=-1:CutRangeMax[2]=1"...", where [2] is the third input variable

   // Cut optimisation
   if (Use["Cuts"])
      factory->BookMethod( TMVA::Types::kCuts, "Cuts",
                           "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart" );

   if (Use["CutsD"])
      factory->BookMethod( TMVA::Types::kCuts, "CutsD",
                           "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart:VarTransform=Decorrelate" );

   if (Use["CutsPCA"])
      factory->BookMethod( TMVA::Types::kCuts, "CutsPCA",
                           "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart:VarTransform=PCA" );

   if (Use["CutsGA"])
      factory->BookMethod( TMVA::Types::kCuts, "CutsGA",
                           "H:!V:FitMethod=GA:CutRangeMin[0]=-10:CutRangeMax[0]=10:VarProp[1]=FMax:EffSel:Steps=30:Cycles=3:PopSize=400:SC_steps=10:SC_rate=5:SC_factor=0.95" );

   if (Use["CutsSA"])
      factory->BookMethod( TMVA::Types::kCuts, "CutsSA",
                           "!H:!V:FitMethod=SA:EffSel:MaxCalls=150000:KernelTemp=IncAdaptive:InitialTemp=1e+6:MinTemp=1e-6:Eps=1e-10:UseDefaultScale" );

   // Likelihood ("naive Bayes estimator")
   if (Use["Likelihood"])
      factory->BookMethod( TMVA::Types::kLikelihood, "Likelihood",
                           "H:!V:TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmoothBkg[1]=10:NSmooth=1:NAvEvtPerBin=50" );

   // Decorrelated likelihood
   if (Use["LikelihoodD"])
      factory->BookMethod( TMVA::Types::kLikelihood, "LikelihoodD",
                           "!H:!V:TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmooth=5:NAvEvtPerBin=50:VarTransform=Decorrelate" );

   // PCA-transformed likelihood
   if (Use["LikelihoodPCA"])
      factory->BookMethod( TMVA::Types::kLikelihood, "LikelihoodPCA",
                           "!H:!V:!TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmooth=5:NAvEvtPerBin=50:VarTransform=PCA" );

   // Use a kernel density estimator to approximate the PDFs
   if (Use["LikelihoodKDE"])
      factory->BookMethod( TMVA::Types::kLikelihood, "LikelihoodKDE",
                           "!H:!V:!TransformOutput:PDFInterpol=KDE:KDEtype=Gauss:KDEiter=Adaptive:KDEFineFactor=0.3:KDEborder=None:NAvEvtPerBin=50" );

   // Use a variable-dependent mix of splines and kernel density estimator
   if (Use["LikelihoodMIX"])
      factory->BookMethod( TMVA::Types::kLikelihood, "LikelihoodMIX",
                           "!H:!V:!TransformOutput:PDFInterpolSig[0]=KDE:PDFInterpolBkg[0]=KDE:PDFInterpolSig[1]=KDE:PDFInterpolBkg[1]=KDE:PDFInterpolSig[2]=Spline2:PDFInterpolBkg[2]=Spline2:PDFInterpolSig[3]=Spline2:PDFInterpolBkg[3]=Spline2:KDEtype=Gauss:KDEiter=Nonadaptive:KDEborder=None:NAvEvtPerBin=50" );

   // Test the multi-dimensional probability density estimator
   // here are the options strings for the MinMax and RMS methods, respectively:
   //      "!H:!V:VolumeRangeMode=MinMax:DeltaFrac=0.2:KernelEstimator=Gauss:GaussSigma=0.3" );
   //      "!H:!V:VolumeRangeMode=RMS:DeltaFrac=3:KernelEstimator=Gauss:GaussSigma=0.3" );
   if (Use["PDERS"])
      factory->BookMethod( TMVA::Types::kPDERS, "PDERS",
                           "!H:!V:NormTree=T:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600" );

   if (Use["PDERSD"])
      factory->BookMethod( TMVA::Types::kPDERS, "PDERSD",
                           "!H:!V:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600:VarTransform=Decorrelate" );

   if (Use["PDERSPCA"])
      factory->BookMethod( TMVA::Types::kPDERS, "PDERSPCA",
                           "!H:!V:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:GaussSigma=0.3:NEventsMin=400:NEventsMax=600:VarTransform=PCA" );

   // Multi-dimensional likelihood estimator using self-adapting phase-space binning
   if (Use["PDEFoam"])
      factory->BookMethod( TMVA::Types::kPDEFoam, "PDEFoam",
                           "!H:!V:SigBgSeparate=F:TailCut=0.001:VolFrac=0.0666:nActiveCells=500:nSampl=2000:nBin=5:Nmin=100:Kernel=None:Compress=T" );

   if (Use["PDEFoamBoost"])
      factory->BookMethod( TMVA::Types::kPDEFoam, "PDEFoamBoost",
                           "!H:!V:Boost_Num=30:Boost_Transform=linear:SigBgSeparate=F:MaxDepth=4:UseYesNoCell=T:DTLogic=MisClassificationError:FillFoamWithOrigWeights=F:TailCut=0:nActiveCells=500:nBin=20:Nmin=400:Kernel=None:Compress=T" );

   // K-Nearest Neighbour classifier (KNN)
   if (Use["KNN"])
      factory->BookMethod( TMVA::Types::kKNN, "KNN",
                           "H:nkNN=20:ScaleFrac=0.8:SigmaFact=1.0:Kernel=Gaus:UseKernel=F:UseWeight=T:!Trim" );

   // H-Matrix (chi2-squared) method
   if (Use["HMatrix"])
      factory->BookMethod( TMVA::Types::kHMatrix, "HMatrix", "!H:!V:VarTransform=None" );

   // Linear discriminant (same as Fisher discriminant)
   if (Use["LD"])
      factory->BookMethod( TMVA::Types::kLD, "LD", "H:!V:VarTransform=None:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10" );

   // Fisher discriminant (same as LD)
   if (Use["Fisher"])
      factory->BookMethod( TMVA::Types::kFisher, "Fisher", "H:!V:Fisher:VarTransform=None:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10" );

   // Fisher with Gauss-transformed input variables
   if (Use["FisherG"])
      factory->BookMethod( TMVA::Types::kFisher, "FisherG", "H:!V:VarTransform=Gauss" );

   // Composite classifier: ensemble (tree) of boosted Fisher classifiers
   if (Use["BoostedFisher"])
      factory->BookMethod( TMVA::Types::kFisher, "BoostedFisher",
                           "H:!V:Boost_Num=20:Boost_Transform=log:Boost_Type=AdaBoost:Boost_AdaBoostBeta=0.2:!Boost_DetailedMonitoring" );

   // Function discrimination analysis (FDA) -- test of various fitters - the recommended one is Minuit (or GA or SA)
   if (Use["FDA_MC"])
      factory->BookMethod( TMVA::Types::kFDA, "FDA_MC",
                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=MC:SampleSize=100000:Sigma=0.1" );

   if (Use["FDA_GA"]) // can also use Simulated Annealing (SA) algorithm (see Cuts_SA options])
      factory->BookMethod( TMVA::Types::kFDA, "FDA_GA",
                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=GA:PopSize=300:Cycles=3:Steps=20:Trim=True:SaveBestGen=1" );

   if (Use["FDA_SA"]) // can also use Simulated Annealing (SA) algorithm (see Cuts_SA options])
      factory->BookMethod( TMVA::Types::kFDA, "FDA_SA",
                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=SA:MaxCalls=15000:KernelTemp=IncAdaptive:InitialTemp=1e+6:MinTemp=1e-6:Eps=1e-10:UseDefaultScale" );

   if (Use["FDA_MT"])
      factory->BookMethod( TMVA::Types::kFDA, "FDA_MT",
                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=2:UseImprove:UseMinos:SetBatch" );

   if (Use["FDA_GAMT"])
      factory->BookMethod( TMVA::Types::kFDA, "FDA_GAMT",
                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=GA:Converger=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=0:!UseImprove:!UseMinos:SetBatch:Cycles=1:PopSize=5:Steps=5:Trim" );

   if (Use["FDA_MCMT"])
      factory->BookMethod( TMVA::Types::kFDA, "FDA_MCMT",
                           "H:!V:Formula=(0)+(1)*x0+(2)*x1+(3)*x2+(4)*x3:ParRanges=(-1,1);(-10,10);(-10,10);(-10,10);(-10,10):FitMethod=MC:Converger=MINUIT:ErrorLevel=1:PrintLevel=-1:FitStrategy=0:!UseImprove:!UseMinos:SetBatch:SampleSize=20" );

   // TMVA ANN: MLP (recommended ANN) -- all ANNs in TMVA are Multilayer Perceptrons
   if (Use["MLP"])
      factory->BookMethod( TMVA::Types::kMLP, "MLP", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:!UseRegulator" );

   if (Use["MLPBFGS"])
      factory->BookMethod( TMVA::Types::kMLP, "MLPBFGS", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:TrainingMethod=BFGS:!UseRegulator" );

   if (Use["MLPBNN"])
      factory->BookMethod( TMVA::Types::kMLP, "MLPBNN", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:TrainingMethod=BFGS:UseRegulator" ); // BFGS training with bayesian regulators

   // CF(Clermont-Ferrand)ANN
   if (Use["CFMlpANN"])
      factory->BookMethod( TMVA::Types::kCFMlpANN, "CFMlpANN", "!H:!V:NCycles=2000:HiddenLayers=N+1,N"  ); // n_cycles:#nodes:#nodes:...

   // Tmlp(Root)ANN
   if (Use["TMlpANN"])
      factory->BookMethod( TMVA::Types::kTMlpANN, "TMlpANN", "!H:!V:NCycles=200:HiddenLayers=N+1,N:LearningMethod=BFGS:ValidationFraction=0.3"  ); // n_cycles:#nodes:#nodes:...

   // Support Vector Machine
   if (Use["SVM"])
      factory->BookMethod( TMVA::Types::kSVM, "SVM", "Gamma=0.25:Tol=0.001:VarTransform=Norm" );

   // Boosted Decision Trees
   if (Use["BDTG"]) // Gradient Boost
      factory->BookMethod( TMVA::Types::kBDT, "BDTG",
                           "!H:!V:NTrees=1000:BoostType=Grad:Shrinkage=0.10:UseBaggedGrad:GradBaggingFraction=0.5:nCuts=20:NNodesMax=5" );

   if (Use["BDT"])  // Adaptive Boost
      factory->BookMethod( TMVA::Types::kBDT, "BDT",
                           "!H:!V:NTrees=850:nEventsMin=150:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:SeparationType=GiniIndex:nCuts=20:PruneMethod=NoPruning" );


   if (Use["BDTB"]) // Bagging
      factory->BookMethod( TMVA::Types::kBDT, "BDTB",
                           "!H:!V:NTrees=400:BoostType=Bagging:SeparationType=GiniIndex:nCuts=20:PruneMethod=NoPruning" );

   if (Use["BDTD"]) // Decorrelation + Adaptive Boost
      factory->BookMethod( TMVA::Types::kBDT, "BDTD",
                           "!H:!V:NTrees=400:nEventsMin=400:MaxDepth=3:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=20:PruneMethod=NoPruning:VarTransform=Decorrelate" );

   if (Use["myBDTD"]) // Decorrelation + Adaptive Boost
      factory->BookMethod( TMVA::Types::kBDT, "BDTDTEST",
                           "!H:!V:NTrees=1000:nEventsMin=400:MaxDepth=6:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=20:PruneMethod=NoPruning:VarTransform=Decorrelate" );

   if (Use["BDTF"])  // Allow Using Fisher discriminant in node splitting for (strong) linearly correlated variables
      factory->BookMethod( TMVA::Types::kBDT, "BDTMitFisher",
                           "!H:!V:NTrees=50:nEventsMin=150:UseFisherCuts:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:SeparationType=GiniIndex:nCuts=20:PruneMethod=NoPruning" );

   // RuleFit -- TMVA implementation of Friedman's method
   if (Use["RuleFit"])
      factory->BookMethod( TMVA::Types::kRuleFit, "RuleFit",
                           "H:!V:RuleFitModule=RFTMVA:Model=ModRuleLinear:MinImp=0.001:RuleMinDist=0.001:NTrees=20:fEventsMin=0.01:fEventsMax=0.5:GDTau=-1.0:GDTauPrec=0.01:GDStep=0.01:GDNSteps=10000:GDErrScale=1.02" );

   // For an example of the category classifier usage, see: TMVAClassificationCategory

   //   TMVA::IMethod* category         = factory->BookMethod( TMVA::Types::kCategory,"Category","" );

   // --------------------------------------------------------------------------------------------------

   // ---- Now you can optimize the setting (configuration) of the MVAs using the set of training events
#if 0
   factory->OptimizeAllMethods("SigEffAt001", "Scan");
   factory->OptimizeAllMethods("ROCIntegral", "GA");
#endif
   // --------------------------------------------------------------------------------------------------

   // ---- Now you can tell the factory to train, test, and evaluate the MVAs

   // Train MVAs using the set of training events
   factory->TrainAllMethods();

   // ---- Evaluate all MVAs using the set of test events
   factory->TestAllMethods();

   // ----- Evaluate and compare performance of all configured MVAs
   factory->EvaluateAllMethods();

   // --------------------------------------------------------------

   // Save the output
   outputFile->Close();

   std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
   std::cout << "==> TMVAClassification is done!" << std::endl;

   delete factory;
}
