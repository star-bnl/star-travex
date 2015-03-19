#ifndef StiHifyEvent_h
#define StiHifyEvent_h

#include "StEvent/StEnumerations.h"
#include "StiRootIO/TStiEvent.h"


class StiHifyEvent : public TStiEvent
{
public:

   StiHifyEvent();
   StiHifyEvent(StDetectorId detGroupId, bool detActiveOnly);

   ClassDef(StiHifyEvent, 1)
};

#endif
