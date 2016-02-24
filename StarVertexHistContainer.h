#ifndef StarVertexHistContainer_h
#define StarVertexHistContainer_h

#include "travex/HistContainer.h"
#include "StMuDSTMaker/COMMON/StMuPrimaryVertex.h"


class StarVertexHistContainer : public tvx::HistContainer
{
public:

   StarVertexHistContainer(const std::string name, TDirectory* motherDir=nullptr, const std::string option="");

   void FillHists(const StMuPrimaryVertex &vertex);
   virtual void FillDerivedHists() {}

protected:

   virtual void BookHists() {}


};

#endif
