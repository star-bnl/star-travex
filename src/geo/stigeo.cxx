#include <iostream>
#include <boost/format.hpp>

#include "StarAgmlLib/AgBlock.h"
#include "StarAgmlLib/StarTGeoStacker.h"
#include "StarGeometry/StarGeo.h"
#include "StarMagField/StarMagField.h"
#include "St_db_Maker/St_db_Maker.h"
#include "StiMaker/StiDetectorVolume.h"
#include "Sti/StiCylindricalShape.h"
#include "Sti/StiDefaultToolkit.h"
#include "Sti/StiDetector.h"
#include "Sti/StiMaterial.h"
#include "Sti/StiShape.h"
#include "StiTpc/StiTpcDetectorBuilder.h"
#include "StTpcDb/StTpcDb.h"

#include "TFile.h"
#include "TGeoManager.h"


void export_sti2tvol(const StiDetectorBuilder& builder, unsigned select = StiDetectorVolume::kAll);
void export_sti2tgeo(const StiDetectorBuilder& builder, unsigned select = StiDetectorVolume::kAll);

TGeoMedium*  create_medium(const TGeoManager& tgeo, const StiMaterial& sti_material);
EColor volume_color(const StiDetector& sti_detector);


int main(int argc, char **argv)
{
   TGeoManager::Import("stargeo_y2017a.root");

   StiToolkit* stitk = new StiDefaultToolkit();
 
   St_db_Maker* st_db_maker = new St_db_Maker("db", "MySQL:StarDb", "StarDb");
   st_db_maker->Init();
   st_db_maker->SetDateTime(20180101, 0);

   StarMagField* starMagField = new StarMagField(StarMagField::kMapped, 0.5, true);

   StTpcDb* myStTpcDb = StTpcDb::instance();
   gStTpcDb->SetTpcRotations();

   StiTpcDetectorBuilder stiTpcDetectorBuilder(true);
   stiTpcDetectorBuilder.build(*st_db_maker);

   return EXIT_SUCCESS;
}



void export_sti2tvol(const StiDetectorBuilder& builder, unsigned select)
{
   TFile stigeo_out("stigeo_tpc.root", "RECREATE");
   StiDetectorVolume stiDetectorVolume(builder);
   stiDetectorVolume.Write();
   stigeo_out.Close();
}



void export_sti2tgeo(const StiDetectorBuilder& builder, unsigned select)
{
   TGeoManager tgeo("sti_geo", "Sti geometry");
   TGeoVolume*  tgeo_top = tgeo.MakeBox("TOP", nullptr, 300, 300, 300);
   tgeo_top->SetVisibility(true);
   tgeo.SetTopVolume(tgeo_top);

   unsigned n_rows = builder.getNRows();
   int det_id = 0;

   std::cout << "Sti geometry: " << builder.getName() << " has " << n_rows << " rows" << std::endl;

   for (unsigned i_row = 0; i_row < n_rows; i_row++)
   {
      unsigned nSectors = builder.getNSectors(i_row);
      for (unsigned i_sector = 0; i_sector < nSectors; i_sector++)
      {
         StiDetector* curr_sti_detector = builder.getDetector(i_row, i_sector);

         if (!curr_sti_detector) {
            std::cout << "The is no sti detector for row " << i_row << " sector " << i_sector << " Skipping..." << std::endl;
            continue;
         }

         if (select && ( (select == StiDetectorVolume::kActive   && !curr_sti_detector->isActive()) ||
                         (select == StiDetectorVolume::kPassivie &&  curr_sti_detector->isActive()) ))
         {
            std::cout << "Volume does not pass selection. Skipping..." << std::endl;
            continue;
         }

         const StiShape& sti_shape = *curr_sti_detector->getShape();
         const StiPlacement& sti_placement = *curr_sti_detector->getPlacement();
         StiShapeCode sti_shape_id = sti_shape.getShapeCode();
         TGeoVolume*  tgeo_volume = nullptr;
         TGeoMedium*  tgeo_medium = create_medium( tgeo, *curr_sti_detector->getMaterial() );
         std::array<double, 3> transformation{0, 0, 0};

         std::string tgeo_volume_name( (boost::format("_%|02|_%|02|") % i_sector % i_row).str() );

         if (sti_shape_id == kPlanar)
         {
            tgeo_volume = tgeo.MakeBox( ("box" + tgeo_volume_name).c_str(), tgeo_medium,
               sti_shape.getThickness()/2, sti_shape.getHalfWidth(), sti_shape.getHalfDepth());

            transformation[0] = sti_placement.getCenterRadius()*TMath::Cos(sti_placement.getCenterRefAngle());
            transformation[1] = sti_placement.getCenterRadius()*TMath::Sin(sti_placement.getCenterRefAngle());
            transformation[2] = sti_placement.getZcenter();
         }
         else if (sti_shape_id == kCylindrical)
         {
            const StiCylindricalShape& cyl_shape = static_cast<const StiCylindricalShape&>(sti_shape);

             if ( cyl_shape.getOpeningAngle() < (TMath::TwoPi() - 0.001) )
             {
                tgeo_volume = tgeo.MakeTubs( ("tubs" + tgeo_volume_name).c_str(), tgeo_medium,
                                  cyl_shape.getOuterRadius() - cyl_shape.getThickness(),
                                  cyl_shape.getOuterRadius(),
                                  cyl_shape.getHalfDepth(),
                                 -cyl_shape.getOpeningAngle()*TMath::RadToDeg()/2,
                                 +cyl_shape.getOpeningAngle()*TMath::RadToDeg()/2 );
             }
             else
             {
                tgeo_volume = tgeo.MakeTube( ("tube" + tgeo_volume_name).c_str(), tgeo_medium,
                                 cyl_shape.getOuterRadius() - cyl_shape.getThickness(),
                                 cyl_shape.getOuterRadius(),
                                 cyl_shape.getHalfDepth() );
             }
         }
         else {
            std::cout << "Only planar and cylindrical shapes supported. Skipping..." << std::endl;
            continue;
         }

         double angle = sti_placement.getNormalRefAngle();

         std::array<double, 9> rot_matrix{
            TMath::Cos(angle), -TMath::Sin(angle), 0,
            TMath::Sin(angle),  TMath::Cos(angle), 0,
                            0,                  0, 1
         };

         TGeoRotation *tgeo_rot = new TGeoRotation();
         tgeo_rot->SetMatrix( rot_matrix.data() );

         TGeoCombiTrans *tgeo_combi = new TGeoCombiTrans(
            transformation[0],
            transformation[1],
            transformation[2],
            tgeo_rot);

         tgeo_volume->SetLineColor( volume_color(*curr_sti_detector) );
         tgeo_top->AddNode(tgeo_volume, det_id++, tgeo_combi);
      }
   }

   tgeo.CloseGeometry();
   tgeo.Export("stigeo_tpc.root");
}


/** */
TGeoMedium* create_medium(const TGeoManager& tgeo, const StiMaterial& sti_material)
{
   std::string material_name(sti_material.getName());

   TGeoMaterial* tgeo_material = tgeo.GetMaterial(material_name.c_str());

   if (!tgeo_material) {
      tgeo_material = new TGeoMaterial(sti_material.getName().c_str(),
                                       sti_material.getA(),
                                       sti_material.getZ(),
                                       sti_material.getDensity(),
                                       sti_material.getX0());
   }

   TGeoMedium* tgeo_medium = tgeo.GetMedium(material_name.c_str());

   static int id = 0;

   if (!tgeo_medium) {
      tgeo_medium = new TGeoMedium(sti_material.getName().c_str(), id++, tgeo_material);
   }

   return tgeo_medium;
}



EColor volume_color(const StiDetector& sti_detector)
{
   const StiPlacement& sti_placement = *sti_detector.getPlacement();
   const StiShape& sti_shape = *sti_detector.getShape();
   StiShapeCode sti_shape_id = sti_shape.getShapeCode();

   EColor color = kGray;

   if (sti_shape_id == kPlanar)
      color = kGray;
   else if (sti_shape_id == kCylindrical)
      color = kCyan;

   color = sti_placement.getZcenter() < 0 ? EColor(kBlue - 9) : color;
   color = sti_placement.getZcenter() > 0 ? EColor(kMagenta - 9) : color;

   return color;
}
