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
   std::string GetVolumeName() const { return fVolumeName; }
   const TVector3& GetPosition() const { return fPosition; }
   virtual void Print(Option_t *opt = "") const;

   friend bool operator< (const TStiHit& lhs, const TStiHit& rhs);

private:

   StDetectorId fDetectorId;
   TVector3  fPosition;   ///< Coordinates of the hit
   std::string fVolumeName; //!< Name of Sti volume
   TVector3  fPositionLocal;   ///< Local coordinates of the hit in the detector CS
   char      fTimesUsed;  ///< The number of tracks this hit was assigned to

   ClassDef(TStiHit, 2)
};


bool operator< (const TStiHit& lhs, const TStiHit& rhs);

#endif
