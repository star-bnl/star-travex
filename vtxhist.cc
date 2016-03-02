#include <iostream>
#include <string>

#include "travex/ProgramOptions.h"

namespace po = boost::program_options;

int main(int argc, char **argv)
{
   tvx::ProgramOptions prgOpts(argc, argv);

   prgOpts.ProcessOptions();
   prgOpts.Print();

   return EXIT_SUCCESS;
}
