#include "StiScan/StiScanEvent.h"

ClassImp(StiScanEvent);


StiScanEvent::StiScanEvent() : TStiEvent()
{
}


StiScanEvent::StiScanEvent(StDetectorId detGroupId) : TStiEvent(detGroupId)
{
}
