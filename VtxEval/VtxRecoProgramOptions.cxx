#include <iostream>

#include <boost/program_options/errors.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include "travex/utils.h"
#include "VtxEval/VtxRecoProgramOptions.h"

// A shorthand for a long namespace
namespace po = boost::program_options;

using VertexFit_t = star_vertex::VertexFit_t;
using SeedFinder_t = star_vertex::SeedFinder_t;


VtxRecoProgramOptions::VtxRecoProgramOptions(int argc, char **argv, const std::string& out_sfx, const std::string& out_ext) :
   tvx::ProgramOptions(argc, argv, out_sfx, out_ext),
   fVertexFit(VertexFit_t::Unspecified),
   fSeedFinder(SeedFinder_t::Unspecified)
{
   // Declare supported options
   fOptions.add_options()
      ("vertex-seed-finder,v",
          po::value<SeedFinder_t>(&fSeedFinder)->default_value(SeedFinder_t::PPVLikelihood),
          "Type of vertex seeding algorithm. Currently supported values are: PPVLikelihood")

      ("vertex-fit,f",
          po::value<VertexFit_t>(&fVertexFit)->default_value(VertexFit_t::Beamline3D),
          "Type of vertex fit. Currently supported values are: NoBeamline, Beamline1D, and Beamline3D")
   ;
}



void VtxRecoProgramOptions::VerifyOptions()
{
   // First check the parent class' options
   tvx::ProgramOptions::VerifyOptions();


   if (fOptionsValues.count("vertex-fit") && fVertexFit == VertexFit_t::Unspecified)
   {
      throw po::validation_error(po::validation_error::invalid_option_value, "", "vertex-fit");
   }


   if (fOptionsValues.count("vertex-seed-finder") &&
       fSeedFinder == SeedFinder_t::Unspecified )
   {
      throw po::validation_error(po::validation_error::invalid_option_value, "", "vertex-seed-finder");
   }
}


/** Prints the options and their values. */
void VtxRecoProgramOptions::Print() const
{
   ProgramOptions::Print();

   std::cout << "\tvertex-fit:\t" << fVertexFit << "\n";
   std::cout << "\tvertex-seed-finder:\t" << fSeedFinder << "\n";
}
