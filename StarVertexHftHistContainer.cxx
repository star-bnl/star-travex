#include "StarVertexHftHistContainer.h"


StarVertexHftHistContainer::StarVertexHftHistContainer(const std::string name, TDirectory* motherDir, const std::string option) :
   StarVertexHistContainer(name, motherDir, option)
{
   fHs["hMcRecoVertexDeltaX"]->SetBins(50, -0.01, 0.01);
   fHs["hMcRecoVertexDeltaY"]->SetBins(50, -0.01, 0.01);
   fHs["hMcRecoVertexDeltaZ"]->SetBins(50, -0.01, 0.01);

   fHs["hVertexErrorX"]->SetBins(50, 0, 0.05);
   fHs["hVertexErrorY"]->SetBins(50, 0, 0.05);
   fHs["hVertexErrorZ"]->SetBins(50, 0, 0.05);
}
