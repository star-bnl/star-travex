#ifndef StiHifyEvent_h
#define StiHifyEvent_h

#include "StEvent/StEnumerations.h"
#include "StiRootIO/TStiEvent.h"

class StiVolumeFilter;


class StiHifyEvent : public TStiEvent
{
public:

   StiHifyEvent();
   StiHifyEvent(const StiVolumeFilter& stiVolumeFilter);
   StiHifyEvent(StDetectorId detGroupId, bool detActiveOnly);

   virtual EReturnCodes Fill(const StiTrackContainer &stiTrackContainer);

protected:

   const StiVolumeFilter& fStiVolumeFilter;

   ClassDef(StiHifyEvent, 1)
};

#endif
