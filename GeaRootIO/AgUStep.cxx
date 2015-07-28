#include <functional>

#include "TGeoManager.h"
#include "St_geant_Maker/St_geant_Maker.h"
#include "GeaRootIO/AgUStep.h"


ClassImp(AgUStep)


extern "C" {

  void agustep_() {
    if ( AgUStep::Instance() ) {
      (*AgUStep::Instance())();
    }
  }

  struct Agcstep_t {
    
    Float_t vect0[7];
    Float_t vloc0[7];
    Float_t vloc[7];
    Float_t xloc[7];
    Float_t astep;
    Float_t adestep;

  } agcstep_;

}


#define agcstep agcstep_


Float_t AgUStep::rmin = 0.0;
Float_t AgUStep::rmax = 50.0;

TGiant3  *AgUStep::geant3; // G3 VMC
Quest_t  *AgUStep::cquest; // G3 Commons ...
Gclink_t *AgUStep::clink; 
Gcflag_t *AgUStep::cflag; 
Gcvolu_t *AgUStep::cvolu; 
Gcnum_t  *AgUStep::cnum; 
Gcsets_t *AgUStep::csets;
Gckine_t *AgUStep::ckine;
Gcking_t *AgUStep::cking;
Gctrak_t *AgUStep::ctrak;
Gcmate_t *AgUStep::cmate;
Gccuts_t *AgUStep::ccuts;
Gcphys_t *AgUStep::cphys;
Int_t     AgUStep::nlev;

AgUStep *AgUStep::sInstance = 0;

AgUStep *AgUStep::Instance()
{ 
  if ( 0 == sInstance ) sInstance = new AgUStep();
  return sInstance; 
}


AgUStep::AgUStep() : TObject(),
     mTree(0),
     mFile(0),
     mEvent( new TGeaEvent() ),
     mTrack(0),
     idEvent(-1)
{ 
  geant3 = St_geant_Maker::instance()->Geant3();
  TGeaEvent::geant3 = geant3;

  cquest = (Quest_t  *) geant3->Quest();
  clink  = (Gclink_t *) geant3->Gclink();
  cflag  = (Gcflag_t *) geant3->Gcflag();
  cvolu  = (Gcvolu_t *) geant3->Gcvolu();
  cnum   = (Gcnum_t  *) geant3->Gcnum();
  csets  = (Gcsets_t *) geant3->Gcsets();
  ckine  = (Gckine_t *) geant3->Gckine();
  cking  = (Gcking_t *) geant3->Gcking();
  ctrak  = (Gctrak_t *) geant3->Gctrak();
  cmate  = (Gcmate_t *) geant3->Gcmate();
  ccuts  = (Gccuts_t *) geant3->Gccuts();
  cphys  = (Gcphys_t *) geant3->Gcphys();

  oldEvent = -999;

}


// Take a step through the G3 geometry
void AgUStep::operator()()
{
  Double_t x = ctrak->vect[0];
  Double_t y = ctrak->vect[1];
//Double_t z = ctrak->vect[2];

  Double_t r = TMath::Sqrt(x*x+y*y);      
  if (r > rmax) return; // track is exiting region of interest

  // Get current event
  idEvent = geant3->CurrentEvent();
  
  // Detect new event and reset sums, clear event, etc...
  if ( oldEvent != idEvent )
    {

      if (mTree && idEvent>1) mTree->Fill(); // last event should be filled on finish

      mEvent->Clear("C"); // clear old event
      aDeStep = 0;          // clear sums etc...
      aStep   = 0;
      idTruth = 0;
      //      LOG_INFO << "New Event " << idEvent << endm;
      mEvent->idEvent = idEvent; // and set the event number
      oldEvent = idEvent;
    }

  // Detect a new track
  if ( 0 == ctrak->sleng ) 
    {
      aDeStep = 0;
      aStep   = 0;
      idTruth++;
      //      LOG_INFO << "  New Track " << idTruth << endm;

      // Add track to this event
      mTrack = mEvent->AddTrack();
      mTrack->idTruth = idTruth;
      
    }
  
  aDeStep += ctrak->destep;
  aStep   += ctrak->step;

  // Add a new step to the track
  assert(mTrack);

  if ( r < rmin ) return; // track has not entered region of interest

  TGeaStep *mStep = mTrack -> AddStep();

  mStep->adEstep      = aDeStep;
  mStep->dEstep       = ctrak -> destep;
  mStep->step         = ctrak -> step;
  mStep->state        = ctrak->inwvol;
  mStep->dens         = cmate->dens;
  mStep->relRadLength = ctrak->step / cmate->radl * 100.;

  if (!gGeoManager) return; // step through before complete init?

  // Print out current path...
  //LOG_INFO << "N level = " << cvolu->nlevel << endm;
  TString path = "";
  for ( Int_t i=0;i<cvolu->nlevel;i++ )
    {
      path += "/"; 
      Char_t buff[16];
      memcpy( buff, &cvolu->names[i], sizeof(cvolu->names[i]) );
      
      TString volume; for ( Int_t ii=0;ii<4;ii++ ) volume += buff[ii];

      path += volume; path += "_"; 
      path += cvolu->number[i];
    }
  //  LOG_INFO << path.Data() << endm;
  
   std::hash<std::string> hash_fn;
   mStep->volNameHash = hash_fn(std::string(path.Data()));
}


void AgUStep::Init( const Char_t *filename )
{
  //  LOG_INFO << "Initialize stepper with filename= " << filename << endm;
  mFile = TFile::Open( filename, "recreate" );
  mTree = new TTree( "stepping", "custom stepping tree" );
  mTree->Branch("TGeaEvent", &mEvent, 32000, 99); // Add the event to the ttree
}


void AgUStep::Finish()
{
  if (mTree&&mFile) {
    mTree->Fill();

    mFile->cd();
    mTree->Write();
    //  mFile->Write();
    delete mFile;
  }
}
