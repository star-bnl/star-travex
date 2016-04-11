#include "VertexRootFile.h"

#include "StarEventHistContainer.h"
#include "StarVertexHistContainer.h"
#include "StarVertexHftHistContainer.h"


VertexRootFile::VertexRootFile(tvx::ProgramOptions& prgOpts, Option_t *option, const char *ftitle, Int_t compress) :
   tvx::RootFile(prgOpts, option, ftitle, compress)
{
   fDirs["event"] = new StarEventHistContainer("event", this);
   fDirs["vertex"] = new StarVertexHistContainer("vertex", this);
   fDirs["vertex_hft"] = new StarVertexHftHistContainer("vertex_hft", this);
   fDirs["vertex_maxrank"] = new StarVertexHistContainer("vertex_maxrank", this);

   // When done creating hist containers make parent TDirectoryFile the current one
   cd();
}


void VertexRootFile::FillHists(const StMuDst &event)
{
   static_cast<StarEventHistContainer*>(fDirs["event"])->FillHists(event);
}


void VertexRootFile::FillHists(const StMuPrimaryVertex &vertex, const StMuMcVertex* mcVertex)
{
   static_cast<StarVertexHistContainer*>(fDirs["vertex"])->FillHists(vertex, mcVertex);
}


void VertexRootFile::FillHistsHftTracks(const StMuPrimaryVertex &vertex, const StMuMcVertex* mcVertex)
{
   static_cast<StarVertexHistContainer*>(fDirs["vertex_hft"])->FillHists(vertex, mcVertex);
}


void VertexRootFile::FillHistsMaxRank(const StMuPrimaryVertex &vertex, const StMuMcVertex* mcVertex)
{
   static_cast<StarVertexHistContainer*>(fDirs["vertex_maxrank"])->FillHists(vertex, mcVertex);
}
