{
   gROOT->Reset();
   gROOT->Clear();

   gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
   loadSharedLibraries();

   gSystem->Load("$OPTSTAR/lib/libboost_system");
   gSystem->Load("$OPTSTAR/lib/libboost_program_options");
   gSystem->Load("$OPTSTAR/lib/libboost_regex");

   gSystem->Load(".$STAR_HOST_SYS/lib/libSti");
   gSystem->Load(".$STAR_HOST_SYS/lib/libStiRootIO");
}
