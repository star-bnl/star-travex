#include <iostream>
#include <string>

#include "TTree.h"
#include "TChain.h"
#include "TMath.h"

#include "StarClassLibrary/StThreeVectorF.hh"
#include "StEvent/StBTofHeader.h"
#include "StEvent/StTrackTopologyMap.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StMuDSTMaker/COMMON/StMuPrimaryVertex.h"
#include "StMuDSTMaker/COMMON/StMuMcTrack.h"
#include "StMuDSTMaker/COMMON/StMuMcVertex.h"
#include "StMuDSTMaker/COMMON/StMuDstMaker.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"

#include "travex/ProgramOptions.h"
#include "src-tools/utils.h"
#include "vtxeval/VertexRootFile.h"


namespace po = boost::program_options;


void process_muDst(VertexRootFile& outFile, int nevent=10000);

// Verify whether this vertex has an HFT track with a PXL hit
bool checkVertexHasPxlHit(int vertexIndex, const StMuDst& stMuDst);

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



void process_muDst(VertexRootFile& outFile, int nevent)
{
   VertexData primVtx;

   TTree *vertexTree = new TTree("vertexTree", "The Primary Vertices");
   vertexTree->Branch("event",   &primVtx.event, "event/I");
   vertexTree->Branch("index",   &primVtx.index, "index/I");
   vertexTree->Branch("rank",    &primVtx.rank, "rank/I");
   vertexTree->Branch("mult",    &primVtx.mult, "mult/I");
   vertexTree->Branch("refMult", &primVtx.refMult, "refMult/I");
   vertexTree->Branch("maxmult", &primVtx.maxmult, "maxmult/I");
   vertexTree->Branch("primX",   &primVtx.primX, "primX/F");
   vertexTree->Branch("primY",   &primVtx.primY, "primY/F");
   vertexTree->Branch("primZ",   &primVtx.primZ, "primZ/F");
   vertexTree->Branch("positionError", &primVtx.positionError, "positionError/StThreeVectorF");
   vertexTree->Branch("zVpd",    &primVtx.zVpd, "zVpd/F");
   vertexTree->Branch("McX",     &primVtx.McX, "McX/F");
   vertexTree->Branch("McY",     &primVtx.McY, "McY/F");
   vertexTree->Branch("McZ",     &primVtx.McZ, "McZ/F");
   vertexTree->Branch("chi2",    &primVtx.chi2, "chi2/F");
   vertexTree->Branch("beam",    &primVtx.beam, "beam/I");
   vertexTree->Branch("postx",   &primVtx.postx, "postx/I");
   vertexTree->Branch("prompt",  &primVtx.prompt, "prompt/I");
   vertexTree->Branch("cross",   &primVtx.cross, "cross/I");
   vertexTree->Branch("tof",     &primVtx.tof, "tof/I");
   vertexTree->Branch("notof",   &primVtx.notof, "notof/I");
   vertexTree->Branch("EEMC",    &primVtx.EEMC, "EEMC/I");
   vertexTree->Branch("noEEMC",  &primVtx.noEEMC, "noEEMC/I");
   vertexTree->Branch("BEMC",    &primVtx.BEMC, "BEMC/I");
   vertexTree->Branch("noBEMC",  &primVtx.noBEMC, "noBEMC/I");



   StMuDstMaker *maker = new StMuDstMaker(0, 0, "", outFile.GetPrgOptions().PathToInputFile().c_str(), "st:MuDst.root", 1e9); // set up maker in read mode
   //                                     0, 0                        this mean read mode
   //                                           dir                    read all files in this directory
   //                                               file               bla.lis real all file in this list, if (file!="") dir is ignored
   //                                                    filter        apply filter to filenames, multiple filters are separated by ':'
   //                                                           10      maximum number of file to read

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

   for (const auto& branchName : activeBranchNames) maker->SetStatus(branchName.c_str(), 1); // Set Active braches

   TChain *tree = maker->chain();
   int nentries = tree->GetEntries();
   nevent = TMath::Min(nevent, nentries);
   std::cout << nentries << " events in chain, " << nevent << " will be read." << std::endl;
   tree->SetCacheSize(-1);        //by setting the read cache to -1 we set it to the AutoFlush value when writing
   tree->SetCacheLearnEntries(1); //one entry is sufficient to learn
   tree->SetCacheEntryRange(0, nevent);


   // Keep track of the number of events with 0 reconstructed verticies
   int noreco = 0;

   // Main loop over events
   for (int iEvent = 0; iEvent < nevent; iEvent++) {
      if (maker->Make()) break;

      StMuDst *muDst = maker->muDst();   // get a pointer to the StMuDst class, the class that points to all the data
      StMuEvent *muEvent = muDst->event(); // get a pointer to the class holding event-wise information

      if (vtxeval::gDebugFlag) std::cout << "Read event #" << iEvent << "\tRun\t" << muEvent->runId() << "\tId: " << muEvent->eventId() << std::endl;

      TClonesArray *primaryVertices   = muDst->primaryVertices();
      int numPrimaryVertices = primaryVertices->GetEntriesFast();

      if (numPrimaryVertices == 0) noreco++;

      TClonesArray *MuMcVertices   = muDst->mcArray(0);
      int NoMuMcVertices = MuMcVertices->GetEntriesFast();

      StBTofHeader *BTofHeader = muDst->btofHeader();

      // Max multiplicity
      // Usually the correct vertex
      int maxVertexMult = 0;
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
         primVtx.positionError  = recoVertex->posError();
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
            std::cout << Form("[%i]", iVertex) << Form(" %8.3f  %8.3f  %8.3f ", recoVertex->position().x(), recoVertex->position().y(), recoVertex->position().z())
                      << Form("  Rank:%1.0f", recoVertex->ranking()) << "    Mult: " << primVtx.mult;

            if (primVtx.maxmult == 1 && iVertex != 0) std::cout << "\t WRONG RANK" << std::endl;
            else std::cout << std::endl;
         }
      }

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

   std::cout << "Number of events: " <<  nevent << ", with 0 reconstructed verticies: " << noreco << std::endl;
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