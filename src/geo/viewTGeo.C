void viewTGeo(std::string tgeoFileName = "tgeo_geom.root")
{
   // Create global gEve
   TEveManager::Create();

   // Load TGeo geometry from a file
   gEve->RegisterGeometryAlias("Default", tgeoFileName.c_str());
 
   TGLViewer* viewer = gEve->GetDefaultGLViewer();
   TGeoManager* tgeo = gEve->GetDefaultGeometry();
 
   // Get the specified top volume and node to draw
   TGeoNode *topNode = tgeo->GetTopNode();
   TEveGeoTopNode *eveTopNode = new TEveGeoTopNode(tgeo, topNode);
 
   gEve->AddGlobalElement(eveTopNode);
   gEve->Redraw3D(true);
}
