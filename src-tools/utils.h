#ifndef vtxeval_utils_h
#define vtxeval_utils_h

#include <iostream>

class StMuDst;
class StMuMcTrack;
class StMuMcVertex;


namespace vtxeval
{

using VectorMcTracks = std::vector<const StMuMcTrack*>;

//
vtxeval::VectorMcTracks getMcTracksMatchingMcVertex(const StMuDst& stMuDst, const StMuMcVertex* mcVertex);

}

#endif
