#ifndef StarVertexHftHistContainer_h
#define StarVertexHftHistContainer_h

#include "StarVertexHistContainer.h"

class StMuPrimaryVertex;
class StMuMcVertex;



class StarVertexHftHistContainer : public StarVertexHistContainer
{
public:

   StarVertexHftHistContainer(const std::string name, TDirectory* motherDir=nullptr, const std::string option="");

};

#endif
