#ifndef StarEventHistContainer_h
#define StarEventHistContainer_h

#include "travex/HistContainer.h"

class StMuDst;
class StMuMcVertex;
class StMuPrimaryVertex;


class StarEventHistContainer : public tvx::HistContainer
{

public:

   StarEventHistContainer(const std::string name, TDirectory* motherDir=nullptr, const std::string option="");

   void FillHists(const StMuDst &event);

   void FillEfficyHists(const StMuDst &event, const StMuMcVertex &mcVertex,
      const StMuPrimaryVertex *recoVertex=nullptr, const StMuPrimaryVertex *recoVertexMaxRank=nullptr);

   virtual void FillDerivedHists();

protected:

   virtual void BookHists() {}

};

#endif
