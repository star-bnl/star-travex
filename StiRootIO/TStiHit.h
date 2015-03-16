#ifndef TStiSti_h
#define TStiSti_h

#include "TObject.h"
#include "Sti/StiHit.h"


class TStiHit : public TObject
{
public:

   TStiHit();
   TStiHit(const StiHit & stiHit);
   virtual void Print(Option_t *opt = "") const;

   friend bool operator< (const TStiHit& lhs, const TStiHit& rhs);

private:

   TVector3  fPosition;   ///< Coordinates of the hit

   ClassDef(TStiHit, 1)
};


bool operator< (const TStiHit& lhs, const TStiHit& rhs);

#endif
