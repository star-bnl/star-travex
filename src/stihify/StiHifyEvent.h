#ifndef StiHifyEvent_h
#define StiHifyEvent_h

#include "StEvent/StEnumerations.h"
#include "stirootio/TStiEvent.h"

class StiVolumeFilter;


class StiHifyEvent : public TStiEvent
{
public:

   StiHifyEvent();
   StiHifyEvent(const StiVolumeFilter& stiVolumeFilter);

   virtual EReturnCodes Fill(const StiTrackContainer &stiTrackContainer);
   void AssignClosestCandidateHit();

   ClassDef(StiHifyEvent, 1)
};

#endif
