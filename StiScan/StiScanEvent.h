#ifndef StiScanEvent_h
#define StiScanEvent_h

#include "StHftPool/EventT/EventT.h"
#include "Sti/StiTrackContainer.h"
#include "StiScan/TStiKalmanTrack.h"


class StiScanEvent : public EventT
{
public:

   StiScanEvent();
   ~StiScanEvent();

   Int_t  Fill(StiTrackContainer &stiTrackContainer);
   virtual void  Print(Option_t *opt = "") const;
   const std::vector<TStiKalmanTrack>& GetTStiKalmanTracks() const { return fTStiKalmanTracks; }

private:

   std::vector<TStiKalmanTrack>  fTStiKalmanTracks;

   ClassDef(StiScanEvent, 1)
};

#endif
