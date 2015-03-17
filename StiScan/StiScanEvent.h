#ifndef StiScanEvent_h
#define StiScanEvent_h

#include "StEvent/StEnumerations.h"
#include "StiRootIO/TStiEvent.h"


class StiScanEvent : public TStiEvent
{
public:

   StiScanEvent();
   StiScanEvent(StDetectorId detGroupId);

   ClassDef(StiScanEvent, 3)
};

#endif
