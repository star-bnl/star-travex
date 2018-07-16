#include "RVersion.h"

#if ROOT_VERSION_CODE >= ROOT_VERSION(6,0,0)
#include "TChain.h"
extern constexpr long long int TChain::kBigNumber;
#endif


class StSstDbMaker;
class StSsdDbMaker;
class StSvtDbMaker;

StSsdDbMaker *gStSsdDbMaker = nullptr;
StSstDbMaker *gStSstDbMaker = nullptr;
StSvtDbMaker *gStSvtDbMaker = nullptr;
