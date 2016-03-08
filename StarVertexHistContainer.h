#ifndef StarVertexHistContainer_h
#define StarVertexHistContainer_h

#include "travex/HistContainer.h"
#include "StMuDSTMaker/COMMON/StMuPrimaryVertex.h"
#include "StMuDSTMaker/COMMON/StMuMcVertex.h"


class StarVertexHistContainer : public tvx::HistContainer
{
public:

   StarVertexHistContainer(const std::string name, TDirectory* motherDir=nullptr, const std::string option="");

   void FillHists(const StMuPrimaryVertex &vertex, const StMuMcVertex* mcVertex=nullptr);

protected:

   virtual void BookHists() {}


};

#endif
