/**
 * Macro to run a custom c++ stepping routine in starsim. The macro runs
 * AgUStep.cxx on each tracking step. A tree is saved, saving each track and the
 * steps taken on each track. The energy lost in each tracking step is saved,
 * along with the energy loss accumulated along the track. The idTruth of each
 * track is saved to enable offline comparison with event reconstruction.
 */

#include "TString.h"

class St_geant_Maker;
class StarGenEvent;
class StarPrimaryMaker;
class StarKinematics;

St_geant_Maker *geant_maker = 0;

const int random_seed = 12345;


void LoadGeometry(TString tag)
{
   TString cmd = "DETP GEOM " + tag + " phys_off=1";

   ExecuteCommand(cmd);

   // Make certain AgML loads
   TString  path = "$STAR/StarDb/AgMLGeometry";
   char *file = gSystem->Which(path.Data(), Form("Geometry.%s.C", tag.Data()), kReadPermission);
   assert(file);

   TString cmdL = Form(".L %s", file);
   TString cmdX = Form("CreateTable()");
   TString cmdU = Form(".U %s", file);

   gInterpreter->ProcessLine(cmdL);
   gInterpreter->Calc       (cmdX);
   gInterpreter->ProcessLine(cmdU);
}


void ExecuteCommand(TString cmd)
{
   if (!geant_maker) geant_maker = (St_geant_Maker *) chain->GetMaker("geant");

   geant_maker->Do(cmd);
}


void LoadLibraries( const char *chopts = "y2014a geant gstar usexgeom agml " )
{
   gROOT->ProcessLine(".L bfc.C");

   bfc(0, TString(chopts));

   gSystem->Load("libVMC.so");

   gSystem->Load("StarGeneratorUtil.so");
   gSystem->Load("StarGeneratorEvent.so");
   gSystem->Load("StarGeneratorBase.so");
   gSystem->Load("libGeaRootIO.so");
   gSystem->Load("libAgUStep.so");

   gSystem->Load("libMathMore.so");
   gSystem->Load("xgeometry.so");
   gSystem->Load("libKinematics.so");
}


void starsim_stepper(double zslice = 0.0 /* cm */, int nevents = 20, int ntracks = 50, double track_pt = 5 /* GeV */,
                     const char *fzname = "starsim_zslice.fz",                   // MC zebra file
                     const char *roname = "starsim_zslice.track_history.root",   // Particle history file
                     const char *rcname = "starsim_zslice.evgen_record.root"     // Event generator record
                    )
{
   LoadLibraries();

   // Setup custom stepping
   AgUStep *step = AgUStep::Instance();

   step->Init( roname );

   // Setup RNG seed and map all ROOT TRandom here
   StarRandom::seed(random_seed);
   StarRandom::capture();

   // Create the primary event generator and insert it before the geant maker
   StarPrimaryMaker *primary = new StarPrimaryMaker();
   primary->SetFileName(rcname);
   primary->SetSigma(0.0, 0.0, 0.0);

   chain->AddBefore("geant", primary);

   StarKinematics *kinematics = new StarKinematics();

   primary->AddGenerator(kinematics);

   // Initialize primary event generator and all sub makers
   primary->Init();

   // Setup geometry and agusread for particle input
   LoadGeometry("y2014a");

   ExecuteCommand("gkine -4 0");
   ExecuteCommand("DCAY 0");
   ExecuteCommand("ANNI 0");
   ExecuteCommand("BREM 0");
   ExecuteCommand("COMP 0");
   ExecuteCommand("HADR 0");
   ExecuteCommand("MUNU 0");
   ExecuteCommand("PAIR 0");
   ExecuteCommand("PFIS 0");
   ExecuteCommand("PHOT 0");
   ExecuteCommand("RAYL 0");
   ExecuteCommand("LOSS 2");
   ExecuteCommand("DRAY 0");
   ExecuteCommand("MULS 0");
   ExecuteCommand("STRA 0");
   ExecuteCommand("physi" );

   // Setup output file
   ExecuteCommand( Form("gfile o %s", fzname) );

   // Generate events
   for (int i = 1; i <= nevents; i++) {
      // Clear the chain from the previous event
      chain->Clear();

      primary->SetVertex(0.0, 0.0, zslice);
      kinematics->Kine(ntracks, "pi-", track_pt-1e-5, track_pt+1e-5, -1e-5, +1e-5);

      chain->Make();
   }

   // Finish up the stepper
   step->Finish();

   ExecuteCommand("call agexit");  // Make sure that STARSIM exits properly
}
