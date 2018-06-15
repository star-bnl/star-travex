#ifndef VertexRootFile_h
#define VertexRootFile_h

#include "travex/RootFile.h"
#include "StVertexRootIO/TDecayVertex.h"

class StMuDst;
class StMuPrimaryVertex;
class StMuMcVertex;


class VertexRootFile : public tvx::RootFile
{
public:

   VertexRootFile(tvx::ProgramOptions& prgOpts, Option_t* option="", const char* ftitle="", Int_t compress=1);

   void FillHists(const StMuDst &event);
   void FillHists(const StMuPrimaryVertex &vertex, const StMuMcVertex* mcVertex=nullptr);
   void FillHists(const StMuPrimaryVertex &vertex, const std::vector<TDecayVertex>& decayVertices);
   void FillHistsHftTracks(const StMuPrimaryVertex &vertex, const StMuMcVertex* mcVertex=nullptr);
   void FillHistsMaxRank(const StMuPrimaryVertex &vertex, const StMuMcVertex* mcVertex=nullptr);

};

#endif
