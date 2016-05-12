#ifndef vtxeval_utils_h
#define vtxeval_utils_h

#include <iostream>

class StMuDst;
class StMuMcTrack;
class StMuMcVertex;


namespace vtxeval
{

using VectorMcTracks = std::vector<const StMuMcTrack*>;

//
vtxeval::VectorMcTracks getMcTracksMatchingMcVertex(const StMuDst& stMuDst, const StMuMcVertex* mcVertex);



static bool gDebugFlag = false;

/** Get input from user to control amount of print out interactively. */
inline bool ask_user()
{
   static bool fAsk = true;
   char symbol;

   if (fAsk) {
      std::cout << "ask (enter - next, r - don't ask, q - quit) >";

      do {
         std::cin.get(symbol);

         if (symbol == 'r') {
            gDebugFlag = false;
            fAsk = false;
         }
         else if (symbol == 'q') return true;
      }
      while (symbol != '\n');

      std::cout << std::endl;
   }

   return false;
}


}

#endif
