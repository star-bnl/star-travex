#ifndef VertexRank_h
#define VertexRank_h

#include <string>

#include "StarClassLibrary/StThreeVectorF.hh"

class StMuDst;


void VertexRank(Long64_t nevent=1000, const std::string& fileName="./*.MuDst.root", const std::string& outFile="my_output_file");

// Verify whether this vertex has an HFT track with a PXL hit
bool checkVertexHasPxlHit(int vertexIndex, const StMuDst& stMuDst);

// A structure to hold info about a vertex
struct VertexData {
   int event, index, rank, mult, refMult, maxmult;
   float primX, primY, primZ, zVpd;
   StThreeVectorF positionError;
   float McX, McY, McZ;
   float chi2;
   int beam, postx, prompt, cross, tof, notof, EEMC, noEEMC, BEMC, noBEMC;
};

#endif
