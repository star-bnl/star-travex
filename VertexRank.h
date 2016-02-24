#ifndef VertexRank_h
#define VertexRank_h

#include "StarClassLibrary/StThreeVectorF.hh"


void VertexRank(Long64_t nevent = 999999, const char *file = "./*.MuDst.root", const  char *outFile = "test");

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
