#include "VertexRootFile.h"

#include "StarEventHistContainer.h"
#include "StarVertexHistContainer.h"


VertexRootFile::VertexRootFile(tvx::ProgramOptions& prgOpts, Option_t *option, const char *ftitle, Int_t compress) :
   tvx::RootFile(prgOpts, option, ftitle, compress)
{
}
