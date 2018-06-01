#include <exception>
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/program_options/parsers.hpp>

#include "evedisp/ProgramOptions.h"


namespace fs = boost::filesystem;


/**
 * Creates a number of options which can be used in a typical application
 * reading a file (containing e.g. a ROOT TTree) and producing an output (e.g.
 * a ROOT file with histograms and their corresponding images).
 */
ProgramOptions::ProgramOptions(int argc, char **argv) :
   fArgc(argc), fArgv(argv),
   fOptions("Available program options", 120),
   fOptionsValues(),
   fConfigFile(),
   fGeometryFile(),
   fInputFile(),
   fTopNode(),
   fVisibleVolumePattern()
{
   fOptions.add_options()
      ("help,h",          "Print this help message")
      ("config-file,c",   po::value<std::string>(&fConfigFile),
                          "Path to config file with all options")
      ("geometry-file,g", po::value<std::string>(&fGeometryFile),
                          "Full path to a ROOT file containing TGeo geometry")
      ("input-file,i",    po::value<std::string>(&fInputFile),
                          "Full path to a ROOT file with event information")
      ("top-node",        po::value<std::string>(&fTopNode),
                          "Path to TGeo top node")
      ("visible-volume-pattern",
                          po::value<std::string>(&fVisibleVolumePattern),
                          "Visualize volumes whose full path matches this regex")
   ;
}


/**
 * Takes the standard command line arguments and parses them with the boost
 * program_options utility. Additional checks are implemented to verify the
 * validity of the supplied arguments.
 */
void ProgramOptions::ProcessOptions()
{
   try {
      po::store(po::parse_command_line(fArgc, fArgv, fOptions), fOptionsValues);
      po::notify(fOptionsValues);

      VerifyOptions();
   }
   catch(const po::error& ex) {
      std::cout << ex.what() << "\n"
                << fOptions << std::endl;
      _exit(EXIT_FAILURE);
   }
   catch(...) {
      std::cout << fOptions << std::endl;
      _exit(EXIT_SUCCESS);
   }
}


/** Prints the options and their values. */
void ProgramOptions::Print() const
{
   std::cout << "Program options set to following values:\n";

   for (const std::pair< std::string, po::variable_value >& option : fOptionsValues)
   {
      std::cout << "\t" << option.first << ":\t" << option.second.value() << "\n";
   }
}


/**
 * Performs basic test of values provided by the user in the command line to
 * maker sure that they are valid.
 */
void ProgramOptions::VerifyOptions()
{
   if (fOptionsValues.count("help"))
      throw std::logic_error("Just print help");

   // First check if a valid config file option is provided and read values from it
   if (fOptionsValues.count("config-file"))
   { 
      fs::path configFile(fConfigFile);

      if ( !fs::exists( configFile ) )
         throw po::error("Config file " + fConfigFile + " does not exist");

      std::cout << "Reading option values from config file: " << fConfigFile << std::endl;

      fs::ifstream my_config(configFile);
      po::store(po::parse_config_file(my_config, fOptions), fOptionsValues);
      po::notify(fOptionsValues);
   }

   // Verify left options

   if (fOptionsValues.count("geometry-file")) {
      if (!fs::exists( fs::path(fGeometryFile) ))
         throw po::error("Geometry file " + fGeometryFile + " does not exist");
   } else
      throw po::required_option("geometry-file");


   if (fOptionsValues.count("input-file")) {
      if (!fs::exists( fs::path(fInputFile) ))
         throw po::error("Input file " + fInputFile + " does not exist");
   } else
      throw po::required_option("input-file");
}



std::ostream& operator<<(std::ostream& os, const boost::any& any_value)
{
   // List all types you want to try
   if(!out_to_stream<int>(os, any_value))
   if(!out_to_stream<unsigned int>(os, any_value))
   if(!out_to_stream<float>(os, any_value))
   if(!out_to_stream<double>(os, any_value))
   if(!out_to_stream<bool>(os, any_value))
   if(!out_to_stream<std::string>(os, any_value))
      os << "{non-std type}"; // all cast are failed, an unknown type of any

   return os;
}
