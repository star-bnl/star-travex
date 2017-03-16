#ifndef VtxRecoProgramOptions_h
#define VtxRecoProgramOptions_h

#include "StGenericVertexMaker/VertexFinderOptions.h"

#include "travex/ProgramOptions.h"



/**
 * Processes and controls user options provided in the command line.
 */
class VtxRecoProgramOptions : public tvx::ProgramOptions
{

   using VertexFit_t = star_vertex::VertexFit_t;
   using SeedFinder_t = star_vertex::SeedFinder_t;

public:

   VtxRecoProgramOptions(int argc, char **argv, const std::string& postfix="_vtxreco.MuDst.root..");

   void Print() const;

   VertexFit_t  GetVertexFit() { return fVertexFit; }

   SeedFinder_t  GetSeedFinder() { return fSeedFinder; }

protected:

   virtual void VerifyOptions();

   VertexFit_t   fVertexFit;

   SeedFinder_t  fSeedFinder;

};

#endif
