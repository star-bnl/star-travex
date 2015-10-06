#ifndef VertexRootFile_h
#define VertexRootFile_h

#include "travex/RootFile.h"


class VertexRootFile : public tvx::RootFile
{
public:

   VertexRootFile(tvx::ProgramOptions& prgOpts, Option_t* option="", const char* ftitle="", Int_t compress=1);

   void FillHists();

};

#endif
