#ifndef StarEventHistContainer_h
#define StarEventHistContainer_h

#include "travex/HistContainer.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"


class StarEventHistContainer : public tvx::HistContainer
{

public:

   StarEventHistContainer(const std::string name, TDirectory* motherDir=nullptr, const std::string option="");

   void FillHists(const StMuDst &event);
   virtual void FillDerivedHists() {}

protected:

   virtual void BookHists() {}

};

#endif
