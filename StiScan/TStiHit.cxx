#include "StiScan/TStiHit.h"

#include <boost/regex.hpp>

#include "StarClassLibrary/StThreeVector.hh"
#include "Sti/StiPlacement.h"
#include "StiScan/TStiKalmanTrack.h"
#include "StiScan/StiScanEvent.h"


ClassImp(TStiHit)


TStiHit::TStiHit() : TObject(),
   fPosition()
{
}


TStiHit::TStiHit(const StiHit & stiHit) : TObject(),
   fPosition()
{
   fPosition.SetXYZ(stiHit.x_g(), stiHit.y_g(), stiHit.z_g());
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
