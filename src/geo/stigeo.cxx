#include <iostream>

#include "StChain/StMaker.h"
#include "St_db_Maker/St_db_Maker.h"
#include "Sti/StiDefaultToolkit.h"
#include "StiMaker/StiDetectorVolume.h"
#include "StiPxl/StiPxlDetectorBuilder.h"
#include "StiTpc/StiTpcDetectorBuilder.h"
#include "StTpcDb/StTpcDb.h"
#include "StTpcDb/StTpcDbMaker.h"
#include "StarGeometry/StarGeo.h"
#include "StarAgmlLib/AgBlock.h"
#include "StarAgmlLib/StarTGeoStacker.h"
#include "tables/St_TrackingParameters_Table.h"

#include "TFile.h"
#include "TGeoManager.h"



int main(int argc, char **argv)
{
   StiToolkit* stitk = new StiDefaultToolkit();
 
   St_db_Maker* st_db_maker = new St_db_Maker("db", "MySQL:StarDb", "$STAR/StarDb", "StarDb");
   st_db_maker->Init();
   st_db_maker->SetDateTime(20180101, 0);

   StarMagField* starMagField = new StarMagField(StarMagField::kMapped, 0.5, true);

   StTpcDb* myStTpcDb = StTpcDb::instance();
   gStTpcDb->SetTpcRotations();

   // Initialize gGeoManager with geometry from a ROOT file
   //gGeoManager = new TGeoManager("star", "star geo");
   //std::cout << "XXX gGeoManager " << gGeoManager << std::endl;

   ////TGeoManager::Import("tgeo.root", "dyson");
   //////gGeoManager = myGeoManager;
   ////std::cout << "XXX myGeoManager " << myGeoManager << std::endl;

   int tmpErrorIgnoreLevel = gErrorIgnoreLevel;
   gErrorIgnoreLevel = 9999;
   StarTGeoStacker *stacker = new StarTGeoStacker();
   AgBlock::SetStacker( stacker );
   StarGeometry* starGeometry = new StarGeometry();
   starGeometry->Construct("y2017a");
   gErrorIgnoreLevel = tmpErrorIgnoreLevel;

   gGeoManager->CloseGeometry();
 
   StiTpcDetectorBuilder detBuilder(true);
   detBuilder.build(*st_db_maker);

   //gGeoManager->Export("tgeo.root");

   TFile stigeo_out("stigeo.root", "RECREATE");

   StiDetectorVolume stiDetectorVolume(detBuilder);
   stiDetectorVolume.Write();

   stigeo_out.Close();

   //delete myGeoManager;

   return EXIT_SUCCESS;
}
