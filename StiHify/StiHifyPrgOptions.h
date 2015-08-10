#ifndef StiHifyPrgOptions_h
#define StiHifyPrgOptions_h

#include <string>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>

#include "StiRootIO/PrgOptionProcessor.h"


namespace {
   namespace po = boost::program_options;
}


/**
 * Processes and controls user options provided in the command line.
 */
class StiHifyPrgOptions : public PrgOptionProcessor
{
public:

   StiHifyPrgOptions();
   StiHifyPrgOptions(int argc, char **argv, const std::string& stiTreeName="t");

   double GetHistZMin() const { return fHistZMin; }
   double GetHistZMax() const { return fHistZMax; }
   double GetHistYMin() const { return fHistYMin; }
   double GetHistYMax() const { return fHistYMax; }
   void   SetHistZRange(double minZ, double maxZ) { fHistZMin = minZ; fHistZMax = maxZ; }
   void   SetHistYRange(double minY, double maxY) { fHistYMin = minY; fHistYMax = maxY; }

protected:

   double  fHistZMin;
   double  fHistZMax;
   double  fHistYMin;
   double  fHistYMax;

private:

   void InitOptions();
};

#endif
