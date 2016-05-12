#include "StarVertexHftHistContainer.h"


StarVertexHftHistContainer::StarVertexHftHistContainer(const std::string name, TDirectory* motherDir, const std::string option) :
   StarVertexHistContainer(name, motherDir, option)
{
   h("hMcRecoVertexDeltaX")->SetBins(50, -0.01, 0.01);
   h("hMcRecoVertexDeltaY")->SetBins(50, -0.01, 0.01);
   h("hMcRecoVertexDeltaZ")->SetBins(50, -0.01, 0.01);

   h("hVertexErrorX")->SetBins(50, 0, 0.05);
   h("hVertexErrorY")->SetBins(50, 0, 0.05);
   h("hVertexErrorZ")->SetBins(50, 0, 0.05);
}
