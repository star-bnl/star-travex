#ifndef TGeaEvent_h
#define TGeaEvent_h

#include <string>
#include <set>

#include "TObject.h"
#include "TClonesArray.h"
#include "TRefArray.h"

class TGiant3;
class TGeaTrack;
class TGeaStep;


class TGeaEvent : public TObject
{
public:
  TGeaEvent();
  ~TGeaEvent(){ Clear(); }

  static std::string DecodeHashedPath(size_t hashValue);

  static void      *fgHash2PathMap;  //!< 
  static TGiant3   *geant3;          //!< Pointer to all geant3 structures

  Int_t     idEvent;
  Int_t     nTracks;
  Int_t     nSteps;
  TClonesArray *tracks;
  TClonesArray *steps;

  TGeaTrack *AddTrack();

  void   Clear( const Option_t *opts="" );

  ClassDef(TGeaEvent,1);
};


class TGeaTrack : public TObject
{
public:
  explicit TGeaTrack(TGeaEvent *parentEvent=nullptr);
  ~TGeaTrack(){ Clear(); }

  TGeaEvent *mParentEvent; //!< Transient pointer to parent event
  Int_t idTruth;     // ID truth for the track
  Float_t eta;       // initial eta
  Float_t phi;       // initial phi
  Float_t x, y, z;   // track 1st position
  Float_t px, py, pz; // track momentum
  Float_t mass, charge;
  Int_t nSteps;      // number of tracking steps
  //TObjArray steps;   // track history
  TRefArray steps;//[nSteps]

  TGeaStep *AddStep();
  void  Clear(const Option_t *opts="" );

  ClassDef(TGeaTrack,1);
};


class TGeaStep : public TObject
{
public:
  TGeaStep();
  bool MatchedVolName(const std::string & pattern) const;
  bool MatchedVolName(const std::set<std::string> & patterns) const;

  void Clear( const Option_t *opts="" );

  Int_t idStep; // Number of tracking steps in this track
  Int_t idTruth; // idTruth of the parent track
  Float_t x,y,z,r; // Position of step;
  Int_t state; // Tracking state (inwvol)
  Float_t dEstep; // energy lost in this step [GeV]
  Float_t adEstep; // accumulated energy lost by this point [GeV]
  Float_t step; // step size [cm]
  Float_t dens; // density of material in this tracking step
  Float_t relRadLength;
  size_t  volNameHash;

  ClassDef(TGeaStep,1);
};

#endif
