#include <boost/regex.hpp>
#include <string>
#include <set>
#include <unordered_map>

#include "TGeaEvent.h"

#include "TGiant3.h"
#include "TError.h"


void*    TGeaEvent::fgHash2PathMap = nullptr;
TGiant3* TGeaEvent::geant3 = nullptr;


TGeaEvent::TGeaEvent() :
  TObject(),
  idEvent(-1),
  nTracks(0),
  nSteps(0),
  tracks(new TClonesArray("TGeaTrack")),
  steps(new TClonesArray("TGeaStep"))
{
  Clear();
}


TGeaTrack* TGeaEvent::AddTrack()
{
  TLorentzVector p, x;

  if (TGeaEvent::geant3) {
     TGeaEvent::geant3->TrackMomentum( p );
     TGeaEvent::geant3->TrackPosition( x );
  }

  TClonesArray &TRACKS = *tracks;

  TGeaTrack *t = new (TRACKS[nTracks++]) TGeaTrack(this);

  t->eta = p.Eta();
  t->phi = p.Phi();

  t->px = p[0];
  t->py = p[1];
  t->pz = p[2];

  t->x = x[0];
  t->y = x[1];
  t->z = x[2];

  if (TGeaEvent::geant3) {
     t->mass   = TGeaEvent::geant3->TrackMass();
     t->charge = TGeaEvent::geant3->TrackCharge();
  }

  //  tracks.Add( t );
  return t;
}


void TGeaEvent::Clear( const Option_t *opts )
{
  idEvent = -1;
  nTracks = 0;
  nSteps  = 0;
  tracks->Clear("C");
}


std::string TGeaEvent::DecodeHashedPath(size_t hashValue)
{
   if (!fgHash2PathMap) return "";

   std::unordered_map<size_t, std::string>* hash2PathMap = static_cast<std::unordered_map<size_t, std::string>* >( fgHash2PathMap );

   auto hash2Path = hash2PathMap->find(hashValue);

   if (hash2Path != hash2PathMap->end()) {
      return hash2Path->second;
   }

   return "no_match";
}


TGeaTrack::TGeaTrack(TGeaEvent* parentEvent) : TObject(), mParentEvent(parentEvent), idTruth(-1), eta(0), phi(0), nSteps(0), steps() { Clear(); }


TGeaStep *TGeaTrack::AddStep()
{
   if (!mParentEvent) {
      Error("TGeaTrack", "Parent event must be defined");
      return nullptr;
   }

  TLorentzVector x;
  if (TGeaEvent::geant3) {
     TGeaEvent::geant3->TrackPosition( x );
  }

  Int_t &n = mParentEvent->nSteps;
  TClonesArray &STEPS = *mParentEvent->steps;
  TGeaStep *s = new (STEPS[n++]) TGeaStep();

  s->x = x[0];
  s->y = x[1];
  s->z = x[2];
  s->r = x.Perp();
  s->idStep = n; // ID of tracking step
  s->idTruth = idTruth;
  nSteps++; // Why is this not incremented in the track object?

  steps.Add(s);

  //  LOG_INFO << "      New TGeaStep at index: " << n << " nSteps=" << nSteps << endm;

  return s;
}


void  TGeaTrack::Clear( const Option_t *opts )
{
  idTruth = -1;
  eta = 0;
  phi = 0;
  px = 0; py = 0; pz = 0;
  x = 0; y = 0; z = 0; mass = 0; charge = 0;
  nSteps=0;
  steps.Clear("");
}


TGeaStep::TGeaStep() : TObject(),
          idStep(-1),
          x(0), y(0), z(0), r(0),
          state(0),
          dEstep(-1),
          adEstep(-1),
          step(-1) ,
          dens(0),
          relRadLength(0),
          volNameHash(0)
{
  Clear();
}

void TGeaStep::Clear(Option_t *opts)
{
  idStep=-1;
  x=0; y=0; z=0; dEstep=-1; adEstep=-1; step=-1; state=0;
}


bool TGeaStep::MatchedVolName(const std::string & pattern) const
{
   std::string volumeNamePath = TGeaEvent::DecodeHashedPath(volNameHash);

   if (volumeNamePath.empty()) return true;

   boost::regex r(pattern);
   bool matched = boost::regex_match(volumeNamePath, r);

   return matched;
}


bool TGeaStep::MatchedVolName(const std::set<std::string> & patterns) const
{
   std::string volumeNamePath = TGeaEvent::DecodeHashedPath(volNameHash);

   if (volumeNamePath.empty() || patterns.empty())
      return true;

   if (volumeNamePath == "no_match")
      return false;

   std::set<std::string>::const_iterator iPattern = patterns.begin();

   for( ; iPattern != patterns.end(); ++iPattern )
   {
      if ( MatchedVolName(*iPattern) )
         return true;
   }

   return false;
}
