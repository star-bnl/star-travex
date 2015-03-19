#include "StiHify/StiHifyEvent.h"

ClassImp(StiHifyEvent);


StiHifyEvent::StiHifyEvent() : TStiEvent()
{
}


/**
 * Constructs event in which track nodes will be constrained by the provided
 * detector Id.
 */
StiHifyEvent::StiHifyEvent(StDetectorId detGroupId, bool detActiveOnly) : TStiEvent(detGroupId, detActiveOnly)
{
}
