#include <algorithm>
#include <cfloat>
#include <iostream>
#include <string>

#include "TTree.h"
#include "TChain.h"

#include "StEvent/StBTofHeader.h"
#include "StEvent/StTrackTopologyMap.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuDstMaker.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StMuDSTMaker/COMMON/StMuMcTrack.h"
#include "StMuDSTMaker/COMMON/StMuMcVertex.h"
#include "StMuDSTMaker/COMMON/StMuPrimaryVertex.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "StSecondaryVertexMaker/DecayVertexFinder.h"
#include "StVertexRootIO/TPrimaryVertex.h"
#include "StVertexRootIO/TDecayVertexContainers.h"

#include "travex/ProgramOptions.h"
#include "vtxeval/utils.h"
#include "vtxeval/VertexRootFile.h"


void process_muDst(VertexRootFile& outFile);

// Verify whether this vertex has an HFT track with a PXL hit
bool checkVertexHasPxlHit(int vertexIndex, const StMuDst& stMuDst);

// Currently used to work around a bug in reco chains giving different numerical
// values in the second event
bool SkipCurrentEvent(const StMuDstMaker& muDstMaker);



int main(int argc, char **argv)
{
   tvx::ProgramOptions prgOpts(argc, argv, "_vtxhist.root..");

   prgOpts.ProcessOptions();
   prgOpts.Print();

   VertexRootFile vertexOutFile(prgOpts, "recreate");

   process_muDst(vertexOutFile);

   vertexOutFile.Write();
   vertexOutFile.Close();

   return EXIT_SUCCESS;
}



void process_muDst(VertexRootFile& outFile)
{
   // Create a TTree with primary vertex info
   TPrimaryVertex *primVtx = new TPrimaryVertex();

   TTree *primVertexTree = new TTree("primVertexTree", "The Primary Vertices");
   primVertexTree->Branch("v.", "TPrimaryVertex", &primVtx, 64000, 99);

   // Create a TTree with secondary decay vertex info
   TDecayVertexVec *decayVertices = new TDecayVertexVec();

   TTree *decayVertexTree = new TTree("decayVertexTree", "Secondary Decay Vertices");
   decayVertexTree->Branch("v.", "TDecayVertexVec", &decayVertices, 64000, 99);

   DecayVertexFinder decayVertexFinder;

   StMuDstMaker muDstMaker(0, 0, "", outFile.GetPrgOptions().PathToInputFile().c_str(), "st:MuDst.root", 1e9); // set up maker in read mode
   //                                     0, 0                        this mean read mode
   //                                           dir                    read all files in this directory
   //                                               file               bla.lis real all file in this list, if (file!="") dir is ignored
   //                                                    filter        apply filter to filenames, multiple filters are separated by ':'
   //                                                           10      maximum number of file to read

   // Disable all branches
   muDstMaker.SetStatus("*", 0);

   std::vector<std::string> activeBranchNames = {
      "MuEvent",
      "PrimaryVertices",
      "PrimaryTracks",
      "GlobalTracks",
      "BTofHit",
      "BTofHeader",
      "StStMuMcVertex",
      "StStMuMcTrack"
   };

   // Enable selected branches
   for (const auto& branchName : activeBranchNames)
      muDstMaker.SetStatus(branchName.c_str(), 1);

   TChain &muDstChain = *muDstMaker.chain();

   unsigned int nEntries      = muDstChain.GetEntries();
   unsigned int nEventsUser   = outFile.GetPrgOptions().GetMaxEventsUser();
   unsigned int nEventsToRead = nEventsUser > 0 ? std::min(nEventsUser, nEntries) : nEntries;

   std::cout << nEntries << " events in chain, " << nEventsToRead << " will be read." << std::endl;


   // Keep track of the number of events with 0 reconstructed verticies
   int nEventsNoRecoVertex = 0;

   // Main loop over events
   for (unsigned int iEvent = 0; iEvent < nEventsToRead; iEvent++)
   {
      if (muDstMaker.Make()) break;

      if ( SkipCurrentEvent(muDstMaker) ) continue;

      // Get a pointer to the StMuDst object that provides access to all of the
      // event data
      StMuDst &muDst = *muDstMaker.muDst();

      // Identify secondary decay vertices and put them in the container
      decayVertexFinder.Find(muDst, decayVertices->mVertices);

      TClonesArray *primaryVertices = muDst.primaryVertices();
      int nPrimaryVertices = primaryVertices->GetEntriesFast();

      if (nPrimaryVertices == 0) nEventsNoRecoVertex++;

      TClonesArray *MuMcVertices = muDst.mcArray(0);
      int nMcVertices = MuMcVertices->GetEntriesFast();

      int maxMultiplicity = 0;
      double maxRank = -DBL_MAX;
      // Pointer to the max rank vertex
      StMuPrimaryVertex *maxRankVertex = nullptr;

      // Loop over primary verticies in the event
      for (int iVertex = 0; iVertex < nPrimaryVertices; iVertex++)
      {
         StMuPrimaryVertex *recoVertex = (StMuPrimaryVertex *) primaryVertices->UncheckedAt(iVertex);

         if (!recoVertex) continue;

         // Find vertex with the highest track multiplicity
         if (recoVertex->noTracks() > maxMultiplicity) {
            maxMultiplicity = recoVertex->noTracks();
         }

         // Find vertex with the highest rank
         if (recoVertex->ranking() > maxRank) {
            maxRank = recoVertex->ranking();
            maxRankVertex = recoVertex;
         }

         //////Mc info/////////
         int idTruth = recoVertex->idTruth();

         // Proceed only if idTruth == 1, i.e. we reconstructed a candidate for the primary vertex
         if (idTruth != 1) continue;

         StMuMcVertex *mcVertex = (idTruth > 0 && idTruth <= nMcVertices) ? (StMuMcVertex *) MuMcVertices->UncheckedAt(idTruth - 1) : nullptr;

         float zVpd      = (muDst.btofHeader() ? muDst.btofHeader()->vpdVz(): 999.);
         bool  isMaxMult = (recoVertex->noTracks() == maxMultiplicity);

         primVtx->Set(*recoVertex, mcVertex, isMaxMult, zVpd);
         primVertexTree->Fill();

         bool hasPxlTrack = checkVertexHasPxlHit(iVertex, muDst);

         if (hasPxlTrack)
            outFile.FillHistsHftTracks(*recoVertex, mcVertex);

         outFile.FillHists(*recoVertex, mcVertex);
         outFile.FillHists(*recoVertex, decayVertices->mVertices);
      }

      // Consider vertex with maximum rank and its simulated counterpart if available
      if (maxRankVertex)
      {
         int idTruth = maxRankVertex->idTruth();
         StMuMcVertex* mcVertex = (idTruth > 0 && idTruth <= nMcVertices) ?
            (StMuMcVertex *) MuMcVertices->UncheckedAt(idTruth - 1) : nullptr;

         // Fill vertex hist container for max rank vertex
         outFile.FillHistsMaxRank(*maxRankVertex, mcVertex);
      }

      outFile.FillHists(muDst);

      // Also add primary struct to the saved struct
      decayVertices->mPrimaryVertex = *primVtx;
      // Save the tree with secondary decay vertices
      decayVertexTree->Fill();
   }

   outFile.Finalize();

   std::cout << "Number of events: " <<  nEventsToRead
             << ", with 0 reconstructed verticies: " << nEventsNoRecoVertex << std::endl;
}


/**
 * Returns true if at least one PXL hit is on a track pointing to vertex with
 * vertexIndex.
 */
bool checkVertexHasPxlHit(int vertexIndex, const StMuDst& stMuDst)
{
   stMuDst.setVertexIndex(vertexIndex);

   TObjArray *primaryTracks = stMuDst.primaryTracks();

   if (!primaryTracks)
      return false;

   int nPrimaryTracks = primaryTracks->GetEntriesFast();

   for (int i=0; i<nPrimaryTracks; i++)
   {
      StMuTrack *stTrack = static_cast<StMuTrack*>(primaryTracks->UncheckedAt(i));
      StTrackTopologyMap trackHitMap = stTrack->topologyMap();

      if ( trackHitMap.hasHitInPxlLayer(1) || trackHitMap.hasHitInPxlLayer(2) || trackHitMap.hasHitInPxlLayer(3) )
         return true;
   }

   return false;
}



bool SkipCurrentEvent(const StMuDstMaker& muDstMaker)
{
   static std::string prev_file_name("");
   static std::string curr_file_name("");
   static int curr_event_index = 0;

   curr_file_name = muDstMaker.GetFileName();

   if (curr_file_name != prev_file_name) {
      std::cout << "Processing new file: " << curr_file_name << std::endl;
      prev_file_name = curr_file_name;
      curr_event_index = 0;
   }

   // Do not process the first two events from each file
   if (curr_event_index < 2) {
      std::cout << "Skipping this event index: " << curr_event_index << std::endl;
      curr_event_index++;
      return true;
   }

   return false;
}


vtxeval::VectorMcTracks vtxeval::getMcTracksMatchingMcVertex(const StMuDst& stMuDst, const StMuMcVertex* mcVertex)
{
   std::vector<const StMuMcTrack*> mcTracks;

   if (!mcVertex) return mcTracks;

   TClonesArray *muMcTracks = stMuDst.mcArray(1);
   int nMuMcTracks = muMcTracks->GetEntriesFast();

   for (int i = 0; i < nMuMcTracks; i++)
   {
      StMuMcTrack *mcTrack = static_cast<StMuMcTrack*>( muMcTracks->UncheckedAt(i) );

      if ( !mcTrack || mcTrack->IdVx() != mcVertex->Id() ) continue;

      mcTracks.push_back(mcTrack);
   }

   return mcTracks;
}
