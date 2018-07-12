#include "common/root_v6.h"

#if ROOT_VERSION_CODE < ROOT_VERSION(6,0,0)

const TCollection &ROOT::Internal::EmptyCollection()
{
   static TObjArray sEmpty;
   return sEmpty;
}


bool ROOT::Internal::ContaineeInheritsFrom(TClass *cl, TClass *base)
{
   return cl->InheritsFrom(base);
}

#endif
