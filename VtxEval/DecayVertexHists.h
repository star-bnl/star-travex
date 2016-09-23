#ifndef DecayVertexHists_h
#define DecayVertexHists_h

#include "travex/HistContainer.h"
#include "StVertexRootIO/TDecayVertex.h"

class StMuPrimaryVertex;
class StMuMcVertex;


class DecayVertexHists : public tvx::HistContainer
{
public:

   DecayVertexHists(const std::string name, TDirectory* motherDir=nullptr, const std::string option="");

   void FillHists(const StMuPrimaryVertex &vertex, const TDecayVertex& decayVtx);

};

#endif
