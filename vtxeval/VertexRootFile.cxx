#include <float.h>

#include "VertexRootFile.h"

#include "TClonesArray.h"

#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuPrimaryVertex.h"
#include "StMuDSTMaker/COMMON/StMuMcVertex.h"

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

   // Currently consider only one primary vertex with idTruth = 1
   int idTruth = 1;
   TClonesArray *muMcVertices = event.mcArray(0);
   StMuMcVertex *mcVertex = static_cast<StMuMcVertex*>(muMcVertices->UncheckedAt(idTruth - 1));

   TClonesArray *primaryVertices = event.primaryVertices();
   int numPrimaryVertices = primaryVertices->GetEntriesFast();
   StMuPrimaryVertex *recoVertex = nullptr;
   StMuPrimaryVertex *recoVertexMaxRank = nullptr;
   double maxRank = -DBL_MAX;

   // Loop over primary verticies in the event
   for (int iVertex = 0; iVertex < numPrimaryVertices; iVertex++)
   {
      StMuPrimaryVertex *testRecoVertex = (StMuPrimaryVertex *) primaryVertices->UncheckedAt(iVertex);

      // Theoretically, there can be other reco vertices with the same truth id
      // but for now we consider the first in the list matching the id of the
      // simulated vertex
      if (testRecoVertex && testRecoVertex->idTruth() == mcVertex->Id()) {
         recoVertex = testRecoVertex;
      }

      if (testRecoVertex->ranking() > maxRank) {
         maxRank = testRecoVertex->ranking();
         recoVertexMaxRank = testRecoVertex;
      }
   }

   static_cast<StarEventHistContainer*>(fDirs["event"])->FillEfficyHists(event, *mcVertex, recoVertex, recoVertexMaxRank);
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
