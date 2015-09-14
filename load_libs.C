{
   gROOT->Reset();
   gROOT->Clear();

   // Note: It has not been checked that all the STAR libraries listed below are
   // actually need to be loaded. For now we just use the list similar to the
   // one in StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C of STAR CVS
   // repository
   gSystem->Load("libStTableUtilities");
   gSystem->Load("libTable");
   gSystem->Load("libPhysics");
   gSystem->Load("St_base");
   gSystem->Load("StChain");
   gSystem->Load("St_Tables");
   gSystem->Load("StUtilities");
   gSystem->Load("StTreeMaker");
   gSystem->Load("StIOMaker");
   gSystem->Load("StarClassLibrary");
   gSystem->Load("StTriggerDataMaker");
   gSystem->Load("StBichsel");
   gSystem->Load("StEvent");
   gSystem->Load("StEventUtilities");
   gSystem->Load("StDbLib");
   gSystem->Load("StEmcUtil");
   gSystem->Load("StTofUtil");
   gSystem->Load("StPmdUtil");
   gSystem->Load("StPreEclMaker");
   gSystem->Load("StStrangeMuDstMaker");
   gSystem->Load("StMuDSTMaker");

   gSystem->Load("libstar-vertex-eval");
}
