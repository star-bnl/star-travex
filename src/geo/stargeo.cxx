#include <iostream>
#include <string>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/regex.hpp>

#include "StarAgmlLib/AgBlock.h"
#include "StarAgmlLib/StarTGeoStacker.h"
#include "StarGeometry/StarGeo.h"

#include "TGeoManager.h"


// A shorthand for a long namespace
namespace po = boost::program_options;

po::variables_map process_options(int, char**);
void verify_options(po::variables_map& );


int main(int argc, char** argv)
{
   po::variables_map option_values = process_options(argc, argv);
   std::string geo_tag( option_values["geo-tag"].as<std::string>() );

   StarTGeoStacker *stacker = new StarTGeoStacker();
   AgBlock::SetStacker( stacker );

   StarGeometry* starGeometry = new StarGeometry();
   starGeometry->Construct(geo_tag.c_str());

   if (!gGeoManager || !gGeoManager->GetTopNode())
   {
      std::cerr << "ERROR: Cannot save STAR geometry. No valid gGeoManager available" << std::endl;
      return EXIT_FAILURE;
   }

   std::string out_file_name("stargeo_" + geo_tag + ".root");

   gGeoManager->CloseGeometry();
   gGeoManager->Export(out_file_name.c_str());

   return EXIT_SUCCESS;
}



po::variables_map process_options(int argc, char** argv)
{
   po::options_description  prg_options;

   prg_options.add_options()
      ("help,h",      "Print this help message")
      ("geo-tag,g",   po::value<std::string>()->default_value("y2017a"), "STAR geometry tag, e.g. y2018x")
   ;

   po::variables_map  option_values;

   try {
      po::store(po::parse_command_line(argc, argv, prg_options), option_values);
      po::notify(option_values);

      verify_options(option_values);
   }
   catch(const po::error& ex) {
      std::cout << ex.what() << "\n"
                << prg_options << std::endl;
      _exit(EXIT_FAILURE);
   }
   catch(...) {
      std::cout << prg_options << std::endl;
      _exit(EXIT_SUCCESS);
   }

   return option_values;
}



void verify_options(po::variables_map& option_values)
{
   if (option_values.count("help"))
      throw std::logic_error("Just print help");

   if (option_values.count("geo-tag"))
   {
      std::string geo_tag( option_values["geo-tag"].as<std::string>() );

      if ( !boost::regex_match(geo_tag, boost::regex("y20\\d\\d\\l")) )
      {
         throw po::invalid_option_value("geo-tag option value must match PERL regex y20##x");
      }
   }
   else
      throw po::required_option("geo-tag");
}
