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

   virtual EReturnCodes Fill(const StiTrackContainer &stiTrackContainer);

   ClassDef(StiHifyEvent, 1)
};

#endif
