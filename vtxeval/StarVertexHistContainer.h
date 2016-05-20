#ifndef StarVertexHistContainer_h
#define StarVertexHistContainer_h

#include "travex/HistContainer.h"

class StMuPrimaryVertex;
class StMuMcVertex;

class StarVertexHistContainer : public tvx::HistContainer
{
public:

   StarVertexHistContainer(const std::string name, TDirectory* motherDir=nullptr, const std::string option="");

   void FillHists(const StMuPrimaryVertex &vertex, const StMuMcVertex* mcVertex=nullptr);

protected:

   virtual void BookHists() {}


};

#endif
