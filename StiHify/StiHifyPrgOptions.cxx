#include "StiHify/StiHifyPrgOptions.h"


StiHifyPrgOptions::StiHifyPrgOptions() : StiHifyPrgOptions(0, nullptr) { }


StiHifyPrgOptions::StiHifyPrgOptions(int argc, char **argv, const std::string& stiTreeName) :
   PrgOptionProcessor(argc, argv, stiTreeName),
   fHistZMin(0), fHistZMax(0),
   fHistYMin(0), fHistYMax(0)
{
   InitOptions();
}


/**
 * Declare supported options.
 */
void StiHifyPrgOptions::InitOptions()
{
   fOptions.add_options()
      ("z-min-local", po::value<double>(&fHistZMin)->default_value(fHistZMin), "If provided the minimum limit along local z will be overwritten by this value in the output histograms")
      ("z-max-local", po::value<double>(&fHistZMax)->default_value(fHistZMax), "If provided the maximum limit along local z will be overwritten by this value in the output histograms")
      ("y-min-local", po::value<double>(&fHistYMin)->default_value(fHistYMin), "If provided the minimum limit along local y will be overwritten by this value in the output histograms")
      ("y-max-local", po::value<double>(&fHistYMax)->default_value(fHistYMax), "If provided the maximum limit along local y will be overwritten by this value in the output histograms")
   ;
}
