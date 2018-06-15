#ifndef AgUStep_h
#define AgUStep_h

#include "TFile.h"
#include "TGiant3.h"
#include "TTree.h"
#include "TObject.h"

#include "GeaRootIO/TGeaEvent.h"


class AgUStep : public TObject
{
public:
  AgUStep();

  static AgUStep *Instance();
  void operator()();

  void Finish();

  /// Initialize stepping routine.  Opens TFile and creates TTree
  void Init( const Char_t *filename="" );

  static Float_t rmin;// =   0.0;
  static Float_t rmax;//= 200.0;

private:

  static AgUStep *sInstance;
  TTree *mTree;
  TFile *mFile;

public:
  TGeaEvent *mEvent; // Current event 
  TGeaTrack *mTrack; // Current track
  
  static TGiant3   *geant3; //!
  static Quest_t   *cquest; //! 
  static Gclink_t  *clink; //! 
  static Gcflag_t  *cflag; //! 
  static Gcvolu_t  *cvolu; //! 
  static Gcnum_t   *cnum; //! 
  static Gcsets_t  *csets; //!
  static Gckine_t  *ckine; //!
  static Gcking_t  *cking; //!
  static Gctrak_t  *ctrak; //!
  static Gcmate_t  *cmate; //!
  static Gccuts_t  *ccuts; //!
  static Gcphys_t  *cphys; //!
  static Int_t      nlev; //!

private:
  Int_t   idEvent;  // current event number
  Int_t   idTruth;  // current track number
  Float_t aDeStep;  // accumulated energy loss
  Float_t aStep;    // accumulated path length

  Float_t vect0[7]; // previous step

  Int_t oldEvent;

  ClassDef(AgUStep, 0)
};

#endif
