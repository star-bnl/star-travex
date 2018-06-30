void viewTGeo(std::string tgeoFileName = "tgeo_geom.root")
{
   // Create global gEve
   TEveManager::Create();

   // Load TGeo geometry from a file
   gEve->RegisterGeometryAlias("Default", tgeoFileName.c_str());
 
   TGLViewer* viewer = gEve->GetDefaultGLViewer();
   TGeoManager* tgeo = gEve->GetDefaultGeometry();

   TGLPerspectiveCamera& cam = (TGLPerspectiveCamera&) viewer->CurrentCamera();
   double center[3]= {0, 400, 0};
   cam.Configure(30, -100, center, -0.3, -2.4);
   viewer->SetGuideState(TGLUtil::kAxesOrigin, true, false, 0);
   viewer->GetClipSet()->SetClipType(TGLClip::kClipBox);
 
   // Get the specified top volume and node to draw
   TGeoNode *topNode = tgeo->GetTopNode();
   TEveGeoTopNode *eveTopNode = new TEveGeoTopNode(tgeo, topNode);
 
   gEve->AddGlobalElement(eveTopNode);
   gEve->Redraw3D();
}
