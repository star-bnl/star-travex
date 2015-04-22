#include "StiRootIO/TStiHit.h"

#include <boost/regex.hpp>

#include "StarClassLibrary/StThreeVector.hh"
#include "Sti/StiPlacement.h"
#include "StiRootIO/TStiKalmanTrack.h"


ClassImp(TStiHit)


TStiHit::TStiHit() : TObject(),
   fDetectorId(kUnknownId),
   fVolumeName(""),
   fPosition(),
   fPositionLocal(),
   fTimesUsed(0)
{
}


TStiHit::TStiHit(const StiHit & stiHit) : TObject(),
   fDetectorId( stiHit.detector() ? static_cast<StDetectorId>(stiHit.detector()->getGroupId()) : kUnknownId ),
   fVolumeName(""),
   fPosition(stiHit.x_g(), stiHit.y_g(), stiHit.z_g()),
   fPositionLocal(stiHit.x(), stiHit.y(), stiHit.z()),
   fTimesUsed(stiHit.timesUsed())
{
   fVolumeName = stiHit.detector() ? stiHit.detector()->getName() : "";
}


void TStiHit::Print(Option_t *opt) const
{
   fPosition.Print();
}


bool operator< (const TStiHit& lhs, const TStiHit& rhs)
{
   if (lhs.fPosition.Perp() < rhs.fPosition.Perp() )
      return true;
   else if ( lhs.fPosition.Perp() == rhs.fPosition.Perp() &&
             lhs.fPosition.Z() < rhs.fPosition.Z() )
      return true;
   else
      return false;
}
