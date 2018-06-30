#include <iostream>
#include <string>

#include <boost/regex.hpp>

#include "TApplication.h"
#include "TEveBrowser.h"
#include "TEveGeoNode.h"
#include "TEveGeoShape.h"
#include "TEveManager.h"
#include "TEvePointSet.h"
#include "TEveTrack.h"
#include "TEveTrackPropagator.h"
#include "TGeoManager.h"
#include "TGeoNavigator.h"
#include "TGeoNode.h"
#include "TGLLightSet.h"
#include "TGLViewer.h"
#include "TGTab.h"

#include "geo/common.h"
#include "evedisp/EventReader.h"
#include "evedisp/ProgramOptions.h"
#include "evedisp/GuiManager.h"


// Forward declared local functions
template<typename Container_t>
void split(const std::string &s, char delim, Container_t result);

std::vector<std::string> split(const std::string &s, char delim);

void make_visible_daughters(TGeoNode *currNode, ProgramOptions &prgOpts);


TEveManager*        create_eve_manager();
TEveGeoTopNode*     create_eve_geometry(ProgramOptions &prgOpts);
TEveGeoShape*       import_eve_geometry();
TEveGeoShape*       import_eve_geometry(TGeoNode* geoTopNode);
TEvePointSet*       create_eve_hit_container();
TEveTrackList*      create_eve_track_container();
TEvePointSetArray*  create_eve_trackhit_container();
EventReader*        create_event_reader(std::string input_file, EventType inputEventType,
                                       TEvePointSet &hits, TEveTrackList &tracks, TEvePointSetArray &trackHits);



int main(int argc, char **argv)
{
   ProgramOptions prgOpts(argc, argv);
   prgOpts.ProcessOptions();
   prgOpts.Print();

   TApplication evedisp("evedisp", 0, 0);

   TEveManager*       eveManager   = create_eve_manager();
   TEveGeoTopNode*    eveTGeometry = create_eve_geometry(prgOpts);
   //TEveGeoShape*      eveTopShape  = import_eve_geometry();
   TEveGeoShape*      eveTopShape  = import_eve_geometry( gGeoManager->GetTopNode() );
   TEvePointSet*      eveHits      = create_eve_hit_container();
   TEveTrackList*     eveTracks    = create_eve_track_container();
   TEvePointSetArray* eveTrackHits = create_eve_trackhit_container();
   EventReader*       eventReader  = create_event_reader(prgOpts.fInputFile, EventType::kStEvent,
                                     *eveHits, *eveTracks, *eveTrackHits);

   // Read current event and fill the appropriate Eve containers
   eventReader->ReadNext();

   eveManager->AddGlobalElement(eveTGeometry);
   eveManager->AddElement(eveHits);
   eveManager->AddElement(eveTracks);
   eveManager->AddElement(eveTrackHits);

   GuiManager guiManager(eveManager, eveTopShape);

   eveManager->Redraw3D();//true);

   evedisp.Run();

   delete eveManager;
   delete eveHits;
   delete eveTracks;
   delete eventReader;

   return EXIT_SUCCESS;
}



template<typename Container_t>
void split(const std::string &s, char delim, Container_t result)
{
   std::stringstream ss(s);
   std::string item;

   while (std::getline(ss, item, delim)) {
      *(result++) = item;
   }
}



std::vector<std::string> split(const std::string &s, char delim)
{
   std::vector<std::string> elems;
   split(s, delim, std::back_inserter(elems));
   return elems;
}



void make_visible_daughters(TGeoNode *currNode, ProgramOptions &prgOpts)
{
   static std::vector<std::string> volume_names = split(prgOpts.fVisibleVolumePattern, '/');

   // Keep track of current level with respect to the first call passing the top node
   static int currLevel = 1;

   std::string currNodeName( currNode->GetName() );

   if ( volume_names.size() == 0 || boost::regex_match(currNodeName, boost::regex(volume_names[currLevel - 1])) ) {
      currNode->SetVisibility(true);
      currNode->VisibleDaughters(true);
   }
   else {
      currNode->SetVisibility(false);
      currNode->VisibleDaughters(false);

      if (currLevel > 1) {
         currLevel--;
         return;
      }
   }

   TObjArray *daugters = currNode->GetVolume()->GetNodes();
   ROOT::Detail::TTypedIter<TGeoNode> next(daugters);

   while (auto daughter = next()) {
      currLevel++;
      make_visible_daughters(daughter, prgOpts);
   }

   if (currLevel > 1) {
      currLevel--;
   }
}



TEveManager *create_eve_manager()
{
   if (gEve) return gEve;

   TApplication::NeedGraphicsLibs();
   gApplication->InitializeGraphics();

   TEveUtil::SetupGUI();

   int dummy;
   unsigned window_w, window_h;
   gVirtualX->GetWindowSize(gVirtualX->GetDefaultRootWindow(), dummy, dummy, window_w, window_h);

   gEve = new TEveManager(int(0.80 * window_w), int(0.90 * window_h), true, "");
   //gEve->GetBrowser()->Move(0, 0);

   //TGLViewer *glViewer = gEve->GetDefaultGLViewer();
   //double ref_point[3] = {0, 0, 0};
   //glViewer->SetGuideState(TGLUtil::kAxesOrigin, true, false, ref_point);
   ////glViewer->SetCurrentCamera(TGLViewer::kCameraOrthoXOY);

   return gEve;
}



TEveGeoTopNode* create_eve_geometry(ProgramOptions &prgOpts)
{
   TGeoManager *geoManager = TGeoManager::Import(prgOpts.fGeometryFile.c_str());
   geoManager->cd(prgOpts.fTopNode.c_str());
   TGeoNode *geoTopNode = geoManager->GetCurrentNode();

   make_visible_daughters(geoTopNode, prgOpts);

   TEveGeoTopNode *eveGeoTopNode = new TEveGeoTopNode(geoManager, geoTopNode);
   eveGeoTopNode->SetMainTransparency(80);

   return eveGeoTopNode;
}



TEveGeoShape* import_eve_geometry()
{
   TFile* geom_file = TFile::Open("eve_shapes.root", "READ");

   TEveGeoShapeExtract* gse = (TEveGeoShapeExtract*) geom_file->Get("shapes");
   TEveGeoShape *eveTopShape = TEveGeoShape::ImportShapeExtract(gse, 0);

   geom_file->Close();
   delete geom_file;

   eveTopShape->SetName( (std::string(eveTopShape->GetName()) + "_shapes").c_str() );

   return eveTopShape;
}



TEveGeoShape* import_eve_geometry(TGeoNode* geoTopNode)
{
   TEveGeoShapeExtract* gse = DumpShapeTree(geoTopNode, 0, false);
   TEveGeoShape *eveTopShape = TEveGeoShape::ImportShapeExtract(gse, 0);

   eveTopShape->SetName( (std::string(eveTopShape->GetName()) + "_shapes").c_str() );

   return eveTopShape;
}



TEvePointSet *create_eve_hit_container()
{
   TEvePointSet *hits = new TEvePointSet("hits");

   hits->SetMarkerColor(kRed);
   hits->SetMarkerStyle(20);
   hits->SetMarkerSize(.1);
   hits->SetRnrState(false);

   return hits;
}



TEveTrackList *create_eve_track_container()
{
   TEveTrackList *tracks = new TEveTrackList("tracks");

   tracks->SetMainColor(kGreen);
   tracks->SetMarkerColor(kYellow);
   tracks->SetLineColor(kYellow);
   tracks->SetLineWidth(2);
   tracks->SetMarkerStyle(4);
   tracks->SetMarkerSize(0.5);

   const double kR_MAX = 250.;
   const double kZ_MAX = 250.;
   // Solenoid field along z, in Tesla. Magnetic field direction -/+ 1
   const double kMagField = +0.498948;

   TEveTrackPropagator *trackProp = tracks->GetPropagator();
   trackProp->SetMagField(kMagField);
   trackProp->SetMaxR(kR_MAX);
   trackProp->SetMaxZ(kZ_MAX);

   return tracks;
}



TEvePointSetArray *create_eve_trackhit_container()
{
   TEvePointSetArray *track_hits = new TEvePointSetArray("track_hits");

   track_hits->SetMarkerColor(kOrange + 9);
   track_hits->SetMarkerStyle(20);
   track_hits->SetMarkerSize(1.);

   return track_hits;
}



EventReader *create_event_reader(std::string input_file, EventType inputEventType,
                                 TEvePointSet &hits, TEveTrackList &tracks, TEvePointSetArray &trackHits)
{
   EventReader *eventReader = nullptr;

   switch (inputEventType) {
   case EventType::kStEvent:
      eventReader = new StEventReader(input_file, hits, tracks, trackHits);
      break;

   case EventType::kTmlEvent:
      eventReader = new TmlEventReader(input_file, hits, tracks, trackHits);
      break;

   default:
      break;
   }

   return eventReader;
}
