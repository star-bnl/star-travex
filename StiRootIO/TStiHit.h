#ifndef TStiSti_h
#define TStiSti_h

#include "TObject.h"

#include "Sti/StiHit.h"
#include "StEvent/StEnumerations.h"


class TStiHit : public TObject
{
public:

   TStiHit();
   TStiHit(const StiHit & stiHit);
   const TVector3& GetPosition() const { return fPosition; }
   virtual void Print(Option_t *opt = "") const;

   friend bool operator< (const TStiHit& lhs, const TStiHit& rhs);

private:

   StDetectorId fDetectorId;
   TVector3  fPosition;   ///< Coordinates of the hit
   char      fTimesUsed;  ///< The number of tracks this hit was assigned to

   ClassDef(TStiHit, 2)
};


bool operator< (const TStiHit& lhs, const TStiHit& rhs);

#endif
