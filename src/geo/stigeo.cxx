#include <iostream>

#include "StarAgmlLib/AgBlock.h"
#include "StarAgmlLib/StarTGeoStacker.h"
#include "StarGeometry/StarGeo.h"
#include "StarMagField/StarMagField.h"
#include "St_db_Maker/St_db_Maker.h"
#include "StiMaker/StiDetectorVolume.h"
#include "Sti/StiDefaultToolkit.h"
#include "StiTpc/StiTpcDetectorBuilder.h"
#include "StTpcDb/StTpcDb.h"

#include "TFile.h"
#include "TGeoManager.h"



int main(int argc, char **argv)
{
   StiToolkit* stitk = new StiDefaultToolkit();
 
   St_db_Maker* st_db_maker = new St_db_Maker("db", "MySQL:StarDb", "StarDb");
   st_db_maker->Init();
   st_db_maker->SetDateTime(20180101, 0);

   StarMagField* starMagField = new StarMagField(StarMagField::kMapped, 0.5, true);

   StTpcDb* myStTpcDb = StTpcDb::instance();
   gStTpcDb->SetTpcRotations();

   int tmpErrorIgnoreLevel = gErrorIgnoreLevel;
   gErrorIgnoreLevel = 9999;
   StarTGeoStacker *stacker = new StarTGeoStacker();
   AgBlock::SetStacker( stacker );
   StarGeometry* starGeometry = new StarGeometry();
   starGeometry->Construct("y2017a");
   gErrorIgnoreLevel = tmpErrorIgnoreLevel;

   gGeoManager->CloseGeometry();
 
   StiTpcDetectorBuilder stiTpcDetectorBuilder(true);
   stiTpcDetectorBuilder.build(*st_db_maker);


   TFile stigeo_out("stigeo.root", "RECREATE");

   StiDetectorVolume stiDetectorVolume(stiTpcDetectorBuilder);
   stiDetectorVolume.Write();

   stigeo_out.Close();

   return EXIT_SUCCESS;
}
