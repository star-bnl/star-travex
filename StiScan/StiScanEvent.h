#ifndef StiScanEvent_h
#define StiScanEvent_h

#include "StHftPool/EventT/EventT.h"
#include "Sti/StiTrackContainer.h"
#include "StiScan/TStiKalmanTrack.h"
#include "StiScan/TStiHit.h"


class StiScanEvent : public EventT
{
public:

   StiScanEvent();

   Int_t  Fill(StiTrackContainer &stiTrackContainer);
   std::pair<std::set<TStiHit>::iterator, bool>   InsertStiHit(const TStiHit &stiHit) { return fTStiHits.insert(stiHit); }
   const std::set<TStiHit>& GetStiHits() const { return fTStiHits; }
   virtual void  Clear(Option_t *opt = "");
   virtual void  Print(Option_t *opt = "") const;
   const std::vector<TStiKalmanTrack>& GetTStiKalmanTracks() const { return fTStiKalmanTracks; }

private:

   std::vector<TStiKalmanTrack>  fTStiKalmanTracks;
   std::set<TStiHit>             fTStiHits;           ///< A collection of all Sti hits in this event

   ClassDef(StiScanEvent, 2)
};

#endif
