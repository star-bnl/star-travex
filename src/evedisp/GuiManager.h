#include "TEveManager.h"
#include "TEveViewer.h"
#include "TEveScene.h"
#include "TEveProjectionManager.h"
#include "TGLCameraOverlay.h"
#include "TGLViewer.h"


class GuiManager
{
public:

   GuiManager(TEveManager* eveManager, TEveGeoShape* eveTopShape)
   {
      fRPhiGeomScene  = eveManager->SpawnNewScene("RPhi Geometry",
                                            "Scene holding projected geometry for the RPhi view.");
      fRPhiEventScene = eveManager->SpawnNewScene("RPhi Event Data",
                                            "Scene holding projected event-data for the RPhi view.");
      fRhoZGeomScene  = eveManager->SpawnNewScene("RhoZ Geometry",
                                            "Scene holding projected geometry for the RhoZ view.");
      fRhoZEventScene = eveManager->SpawnNewScene("RhoZ Event Data",
                                            "Scene holding projected event-data for the RhoZ view.");

      fRPhiMgr = new TEveProjectionManager(TEveProjection::kPT_RPhi);
      fRhoZMgr = new TEveProjectionManager(TEveProjection::kPT_RhoZ);

      f3DViewer = eveManager->SpawnNewViewer("3D View", "");
      f3DViewer->AddScene(eveManager->GetGlobalScene());
      f3DViewer->AddScene(eveManager->GetEventScene());
      TGLPerspectiveCamera& cam = (TGLPerspectiveCamera&) f3DViewer->GetGLViewer()->CurrentCamera();
      double center[3]= {0, 300, 0};
      cam.Configure(30, 300, center, -0.3, -2.4);


      fRPhiViewer = eveManager->SpawnNewViewer("RPhi View", "");
      fRPhiViewer->GetGLViewer()->SetCurrentCamera(TGLViewer::kCameraOrthoXOY);
      fRPhiViewer->GetGLViewer()->GetCameraOverlay()->SetOrthographicMode(TGLCameraOverlay::kGridFront);
      fRPhiViewer->GetGLViewer()->GetCameraOverlay()->SetShowOrthographic(true);
      static_cast<TGLOrthoCamera&>( fRPhiViewer->GetGLViewer()->CurrentCamera() ).SetZoom(0.45);
      fRPhiViewer->AddScene(fRPhiGeomScene);
      fRPhiViewer->AddScene(fRPhiEventScene);


      fRhoZViewer = eveManager->SpawnNewViewer("RhoZ View", "");
      fRhoZViewer->GetGLViewer()->SetCurrentCamera(TGLViewer::kCameraOrthoXOY);
      fRhoZViewer->GetGLViewer()->GetCameraOverlay()->SetOrthographicMode(TGLCameraOverlay::kGridFront);
      fRhoZViewer->GetGLViewer()->GetCameraOverlay()->SetShowOrthographic(true);
      static_cast<TGLOrthoCamera&>( fRhoZViewer->GetGLViewer()->CurrentCamera() ).SetZoom(0.4);
      fRhoZViewer->AddScene(fRhoZGeomScene);
      fRhoZViewer->AddScene(fRhoZEventScene);

      // Do not allow to
      eveTopShape->IncDenyDestroy();

      fRPhiMgr->ImportElements(eveTopShape, fRPhiGeomScene);
      fRPhiMgr->ImportElements(eveManager->GetEventScene(), fRPhiEventScene);

      fRhoZMgr->ImportElements(eveTopShape, fRhoZGeomScene);
      fRhoZMgr->ImportElements(eveManager->GetEventScene(), fRhoZEventScene);
   }

private:

   TEveProjectionManager *fRPhiMgr;
   TEveProjectionManager *fRhoZMgr;

   TEveViewer            *f3DViewer;
   TEveViewer            *fRPhiViewer;
   TEveViewer            *fRhoZViewer;

   TEveScene             *fRPhiGeomScene;
   TEveScene             *fRhoZGeomScene;
   TEveScene             *fRPhiEventScene;
   TEveScene             *fRhoZEventScene;
};
