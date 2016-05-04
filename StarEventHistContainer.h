#ifndef StarEventHistContainer_h
#define StarEventHistContainer_h

#include "travex/HistContainer.h"

class StMuDst;


class StarEventHistContainer : public tvx::HistContainer
{

public:

   StarEventHistContainer(const std::string name, TDirectory* motherDir=nullptr, const std::string option="");

   void FillHists(const StMuDst &event);

protected:

   virtual void BookHists() {}

};

#endif
