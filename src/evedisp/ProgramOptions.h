#ifndef ProgramOptions_h
#define ProgramOptions_h

#include <set>
#include <string>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>


// A shorthand for a long namespace
namespace po = boost::program_options;


/**
 * Processes and controls user options provided in the command line. An input
 * file is required. The path to an output file is built from the input file
 * name and a prefix path provided by the user.
 */
class ProgramOptions
{
public:

   ProgramOptions(int argc, char **argv);

   void ProcessOptions();
   void Print() const;

   std::string  fConfigFile;
   std::string  fGeometryFile;
   std::string  fInputFile;
   std::string  fTopNode;
   std::string  fVisibleVolumePattern;

private:

   /// Verifies user submitted values
   virtual void VerifyOptions();

   /// Standard main() function arguments
   int     fArgc;
   char**  fArgv;

   /// User defined command line options defined with boost::program_options
   po::options_description  fOptions;

   /// Option values extracted from the command line argument
   po::variables_map  fOptionsValues;
};


/**
 * A helper function to print values of boost's any type.
 */
template<class T>
bool out_to_stream(std::ostream& os, const boost::any& any_value)
{
   try {
      os << boost::any_cast<T>(any_value);
      return true;
   } catch(boost::bad_any_cast& e) {
      return false;
   }
}

std::ostream& operator<<(std::ostream& os, const boost::any& any_value);


#endif
