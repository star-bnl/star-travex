#include <algorithm>
#include <iostream>
#include <string>

#include "TTree.h"
#include "TChain.h"

#include "StarClassLibrary/StThreeVectorF.hh"
#include "StEvent/StBTofHeader.h"
#include "StEvent/StTrackTopologyMap.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuDstMaker.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StMuDSTMaker/COMMON/StMuMcTrack.h"
#include "StMuDSTMaker/COMMON/StMuMcVertex.h"
#include "StMuDSTMaker/COMMON/StMuPrimaryVertex.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"

#include "travex/ProgramOptions.h"
#include "src-tools/utils.h"
#include "VtxEval/VertexRootFile.h"


namespace po = boost::program_options;


void process_muDst(VertexRootFile& outFile);

// Verify whether this vertex has an HFT track with a PXL hit
bool checkVertexHasPxlHit(int vertexIndex, const StMuDst& stMuDst);

// Currently used to work around a bug in reco chains giving different numerical
// values in the second event
bool SkipCurrentEvent(const StMuDstMaker& maker);


// A structure to hold info about a vertex
struct VertexData {
   int event, index, rank, mult, refMult, maxmult;
   float primX, primY, primZ, zVpd;
   StThreeVectorF positionError;
   float McX, McY, McZ;
   float chi2;
   int beam, postx, prompt, cross, tof, notof, EEMC, noEEMC, BEMC, noBEMC;

   VertexData() { Init(); }

   void Init() {
      event = index = rank = mult = refMult = maxmult = -999;
      primX = primY = primZ = zVpd = 999.f;
      positionError.set(999.f, 999.f, 999.f);
      McX = McY = McZ = chi2 = 999.f;
   }
};



int main(int argc, char **argv)
{
   tvx::ProgramOptions prgOpts(argc, argv);

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
   VertexData primVtx;

   TTree *vertexTree = new TTree("vertexTree", "The Primary Vertices");
   vertexTree->Branch("event",   &primVtx.event,   "event/I");
   vertexTree->Branch("index",   &primVtx.index,   "index/I");
   vertexTree->Branch("rank",    &primVtx.rank,    "rank/I");
   vertexTree->Branch("mult",    &primVtx.mult,    "mult/I");
   vertexTree->Branch("refMult", &primVtx.refMult, "refMult/I");
   vertexTree->Branch("maxmult", &primVtx.maxmult, "maxmult/I");
   vertexTree->Branch("primX",   &primVtx.primX,   "primX/F");
   vertexTree->Branch("primY",   &primVtx.primY,   "primY/F");
   vertexTree->Branch("primZ",   &primVtx.primZ,   "primZ/F");
   vertexTree->Branch("positionError", &primVtx.positionError, "positionError/StThreeVectorF");
   vertexTree->Branch("zVpd",    &primVtx.zVpd,    "zVpd/F");
   vertexTree->Branch("McX",     &primVtx.McX,     "McX/F");
   vertexTree->Branch("McY",     &primVtx.McY,     "McY/F");
   vertexTree->Branch("McZ",     &primVtx.McZ,     "McZ/F");
   vertexTree->Branch("chi2",    &primVtx.chi2,    "chi2/F");
   vertexTree->Branch("beam",    &primVtx.beam,    "beam/I");
   vertexTree->Branch("postx",   &primVtx.postx,   "postx/I");
   vertexTree->Branch("prompt",  &primVtx.prompt,  "prompt/I");
   vertexTree->Branch("cross",   &primVtx.cross,   "cross/I");
   vertexTree->Branch("tof",     &primVtx.tof,     "tof/I");
   vertexTree->Branch("notof",   &primVtx.notof,   "notof/I");
   vertexTree->Branch("EEMC",    &primVtx.EEMC,    "EEMC/I");
   vertexTree->Branch("noEEMC",  &primVtx.noEEMC,  "noEEMC/I");
   vertexTree->Branch("BEMC",    &primVtx.BEMC,    "BEMC/I");
   vertexTree->Branch("noBEMC",  &primVtx.noBEMC,  "noBEMC/I");



   StMuDstMaker *maker = new StMuDstMaker(0, 0, "", outFile.GetPrgOptions().PathToInputFile().c_str(), "st:MuDst.root", 1e9); // set up maker in read mode
   //                                     0, 0                        this mean read mode
   //                                           dir                    read all files in this directory
   //                                               file               bla.lis real all file in this list, if (file!="") dir is ignored
   //                                                    filter        apply filter to filenames, multiple filters are separated by ':'
   //                                                           10      maximum number of file to read

   // Disable all branches
   maker->SetStatus("*", 0);

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
      maker->SetStatus(branchName.c_str(), 1);

   TChain *tree = maker->chain();
   unsigned int nentries = tree->GetEntries();
   unsigned int nEventsUser = outFile.GetPrgOptions().GetMaxEventsUser();
   unsigned int nevent = nEventsUser > 0 ? std::min(nEventsUser, nentries) : nentries;
   std::cout << nentries << " events in chain, " << nevent << " will be read." << std::endl;
   tree->SetCacheSize(-1);        //by setting the read cache to -1 we set it to the AutoFlush value when writing
   tree->SetCacheLearnEntries(1); //one entry is sufficient to learn
   tree->SetCacheEntryRange(0, nevent);


   // Keep track of the number of events with 0 reconstructed verticies
   int nEventsNoRecoVertex = 0;

   // Main loop over events
   for (int iEvent = 0; iEvent < nevent; iEvent++)
   {
      if (maker->Make()) break;

      if ( SkipCurrentEvent(*maker) ) continue;

      StMuDst *muDst = maker->muDst();   // get a pointer to the StMuDst class, the class that points to all the data
      StMuEvent *muEvent = muDst->event(); // get a pointer to the class holding event-wise information

      if (vtxeval::gDebugFlag)
         std::cout << "Read event #" << iEvent
                   << ", run: " << muEvent->runId()
                   << ", id: " << muEvent->eventId() << std::endl;


      TClonesArray *primaryVertices = muDst->primaryVertices();
      int numPrimaryVertices = primaryVertices->GetEntriesFast();

      if (numPrimaryVertices == 0) nEventsNoRecoVertex++;

      TClonesArray *MuMcVertices = muDst->mcArray(0);
      int NoMuMcVertices = MuMcVertices->GetEntriesFast();

      StBTofHeader *BTofHeader = muDst->btofHeader();

      // Max multiplicity
      int maxVertexMult = 0;
      // Pointer to the max rank vertex
      StMuPrimaryVertex *maxRankVertex = nullptr;
      float vertexMaxRank = -1e10;

      // Loop over primary verticies in the event
      for (int iVertex = 0; iVertex < numPrimaryVertices; iVertex++)
      {
         StMuPrimaryVertex *recoVertex = (StMuPrimaryVertex *) primaryVertices->UncheckedAt(iVertex);

         if (!recoVertex) continue;

         Float_t numTracksToVertex = recoVertex->noTracks();

         if (maxVertexMult < numTracksToVertex) {   //Amilkar: check if the numTracksToVertex is higher than previous
            maxVertexMult = numTracksToVertex;      //Amilkar: asign the new maximum value
         }

         // Find the highest rank vertex
         if (recoVertex->ranking() > vertexMaxRank )
         {
            vertexMaxRank = recoVertex->ranking();
            maxRankVertex = recoVertex;
         }

         //////Mc info/////////
         int idTruth = recoVertex->idTruth();
         StMuMcVertex *mcVertex = (idTruth > 0 && idTruth <= NoMuMcVertices) ? (StMuMcVertex *) MuMcVertices->UncheckedAt(idTruth - 1) : nullptr;

         primVtx.event   = iEvent;
         primVtx.index   = iVertex;
         primVtx.rank    = recoVertex->ranking();
         primVtx.mult    = recoVertex->noTracks();
         primVtx.refMult = recoVertex->refMult();
         primVtx.maxmult = (maxVertexMult == primVtx.mult ? 1 : 0);
         primVtx.primX   = recoVertex->position().x();
         primVtx.primY   = recoVertex->position().y();
         primVtx.primZ   = recoVertex->position().z();
         primVtx.zVpd    = BTofHeader ? BTofHeader->vpdVz() : 999;
         primVtx.positionError = recoVertex->posError();
         primVtx.McX     = mcVertex ? mcVertex->XyzV().x() : 999.f;
         primVtx.McY     = mcVertex ? mcVertex->XyzV().y() : 999.f;
         primVtx.McZ     = mcVertex ? mcVertex->XyzV().z() : 999.f;
         primVtx.chi2    = recoVertex->chiSquared();
         primVtx.beam    = recoVertex->isBeamConstrained() ? 1 : 0;
         primVtx.postx   = recoVertex->nPostXtracks();
         primVtx.prompt  = recoVertex->nPromptTracks();
         primVtx.cross   = recoVertex->nCrossCentralMembrane();
         primVtx.tof     = recoVertex->nCTBMatch()    + recoVertex->nBTOFMatch();
         primVtx.notof   = recoVertex->nCTBNotMatch() + recoVertex->nBTOFNotMatch();
         primVtx.EEMC    = recoVertex->nEEMCMatch();
         primVtx.noEEMC  = recoVertex->nEEMCNotMatch();
         primVtx.BEMC    = recoVertex->nBEMCMatch();
         primVtx.noBEMC  = recoVertex->nBEMCNotMatch();

         vertexTree->Fill();

         bool hasPxlTrack = checkVertexHasPxlHit(iVertex, *muDst);

         if (hasPxlTrack)
            outFile.FillHistsHftTracks(*recoVertex, mcVertex);

         outFile.FillHists(*recoVertex, mcVertex);

         if (vtxeval::gDebugFlag) {
            std::cout << Form("%5d: %8.3f %8.3f %8.3f, rank: %f, multiplicity: %d\n",
               iVertex, recoVertex->position().x(), recoVertex->position().y(), recoVertex->position().z(),
               recoVertex->ranking(), primVtx.mult);

            if (primVtx.maxmult == 1 && iVertex != 0)
               std::cout << "\t WRONG RANK" << std::endl;
         }
      }

      // Consider vertex with maximum rank and its simulated counterpart if available
      if (maxRankVertex)
      {
         int idTruth = maxRankVertex->idTruth();
         StMuMcVertex* mcVertex = (idTruth > 0 && idTruth <= NoMuMcVertices) ?
            (StMuMcVertex *) MuMcVertices->UncheckedAt(idTruth - 1) : nullptr;

         // Fill vertex hist container for max rank vertex
         outFile.FillHistsMaxRank(*maxRankVertex, mcVertex);
      }

      outFile.FillHists(*muDst);
   }

   outFile.FillDerivedHists();

   std::cout << "Number of events: " <<  nevent
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



bool SkipCurrentEvent(const StMuDstMaker& maker)
{
   static std::string prev_file_name("");
   static std::string curr_file_name("");
   static int curr_event_index = 0;

   curr_file_name = maker.GetFileName();

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
