#ifndef TStiHit_h
#define TStiHit_h

#include "TObject.h"

#include "Sti/StiHit.h"
#include "StEvent/StEnumerations.h"


class TStiHit : public TObject
{
public:

   TStiHit();
   TStiHit(const StiHit & stiHit);
   const StiHit* GetStiHit() const { return fStiHit; }
   std::string GetVolumeName() const { return fVolumeName; }
   const TVector3& GetPosition() const { return fPosition; }
   const TVector3& GetPositionLocal() const { return fPositionLocal; }
   virtual void Print(Option_t *opt = "") const;

   friend bool operator< (const TStiHit& lhs, const TStiHit& rhs);

private:

   const StiHit*  fStiHit;         //!< Transient pointer to original StiHit to access non-persistent info
   StDetectorId   fDetectorId;
   std::string    fVolumeName;     //!< Name of Sti volume
   TVector3       fPosition;       ///< Global coordinates of the hit in the STAR CS
   TVector3       fPositionLocal;  ///< Local coordinates of the hit in the detector CS
   char           fTimesUsed;      ///< The number of tracks this hit was assigned to

   ClassDef(TStiHit, 2)
};


bool operator< (const TStiHit& lhs, const TStiHit& rhs);

#endif
