#ifndef VertexRootFile_h
#define VertexRootFile_h

#include "travex/RootFile.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuPrimaryVertex.h"
#include "StMuDSTMaker/COMMON/StMuMcVertex.h"


class VertexRootFile : public tvx::RootFile
{
public:

   VertexRootFile(tvx::ProgramOptions& prgOpts, Option_t* option="", const char* ftitle="", Int_t compress=1);

   void FillHists(const StMuDst &event);
   void FillHists(const StMuPrimaryVertex &vertex, const StMuMcVertex* mcVertex=nullptr);
   void FillHistsHftTracks(const StMuPrimaryVertex &vertex, const StMuMcVertex* mcVertex=nullptr);
   void FillHistsMaxRank(const StMuPrimaryVertex &vertex, const StMuMcVertex* mcVertex=nullptr);

};

#endif
