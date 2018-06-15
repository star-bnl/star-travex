#include <iostream>
#include <string>
#include <deque>
#include <boost/regex.hpp>

#include "TApplication.h"
#include "TColor.h"
#include "TEveBrowser.h"
#include "TEveGeoShapeExtract.h"
#include "TEveGeoNode.h"
#include "TEveManager.h"
#include "TEveTrans.h"
#include "TFile.h"
#include "TGeoManager.h"
#include "TGeoNode.h"
#include "TGeoPhysicalNode.h"
#include "TList.h"
#include "TROOT.h"

#include "geo/common.h"
#include "geo/ProgramOptions.h"


// Forward declared local functions
template<typename Container_t>
void split(const std::string &s, char delim, Container_t result);

std::vector<std::string> split(const std::string &s, char delim);

void fill_shapes(TGeoManager*, ProgramOptions&, TGeoNode*, TEveGeoShapeExtract*);
TEveGeoShapeExtract* create_shape(TGeoPhysicalNode*);
TEveGeoShapeExtract* DumpShapeTree(TGeoNode*            geon,
                                   TEveGeoShapeExtract* parent,
                                   Bool_t               leafs_only, ProgramOptions &prgOpts);


int main(int argc, char **argv)
{
   ProgramOptions prgOpts(argc, argv);
   prgOpts.ProcessOptions();
   prgOpts.Print();

   TGeoManager* geoManager = TGeoManager::Import(prgOpts.fGeometryFile.c_str());
   geoManager->cd(prgOpts.fTopNode.c_str());
   TGeoNode* geoTopNode = geoManager->GetCurrentNode();
   TEveGeoShapeExtract* gse = DumpShapeTree(geoTopNode, 0, true, prgOpts);

   TFile f("eve_shapes.root", "RECREATE");
   gse->Write("shapes");
   f.Close();

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



void fill_shapes(TGeoManager* geoManager, ProgramOptions& prgOpts,
   TGeoNode* currNode, TEveGeoShapeExtract* gse)
{
   // Keep track of current level with respect to the first call passing the top node
   static int currLevel = 1;

   std::string currNodeName( currNode->GetName() );

   if ( boost::regex_match(currNodeName, boost::regex(prgOpts.fVisibleVolumePattern)) ) {
      std::cout << geoManager->GetPath() << " - match" << std::endl;

      TGeoPhysicalNode* physicalNode = geoManager->MakePhysicalNode(geoManager->GetPath());
      TEveGeoShapeExtract* daughterShape = create_shape(physicalNode);
      gse->AddElement(daughterShape);
   }
   else {
      std::cout << geoManager->GetPath() << " - no match" << std::endl;
   }

   TObjArray *daughters = currNode->GetVolume()->GetNodes();
   ROOT::Detail::TTypedIter<TGeoNode> next(daughters);

   int i_daughter = 0;
   while (auto daughterNode = next()) {
      currLevel++;
      geoManager->CdDown(i_daughter++);
      fill_shapes(geoManager, prgOpts, daughterNode, gse);
   }

   if (currLevel > 1) {
      currLevel--;
      geoManager->CdUp();
   }
}



TEveGeoShapeExtract* create_shape(TGeoPhysicalNode* physNode)
{
   if (!physNode) return nullptr;

   std::cout << physNode->GetName() << " title: " << physNode->GetTitle() << std::endl;

   TEveGeoShapeExtract* eveShape = new TEveGeoShapeExtract(physNode->GetName(), physNode->GetTitle());

   TGeoNode*   tnode   = physNode->GetNode();
   TGeoVolume* tvolume = physNode->GetVolume();
   TGeoShape*  tshape  = physNode->GetShape();

   TGeoMatrix     *gm = physNode->GetMatrix();
   const Double_t *rm = gm->GetRotationMatrix();
   const Double_t *tv = gm->GetTranslation();

   double trans[16] = {
      rm[0], rm[3], rm[6], 0,
      rm[1], rm[4], rm[7], 0,
      rm[2], rm[5], rm[8], 0,
      tv[0], tv[1], tv[2], 1  
   };

   eveShape->SetTrans(trans);

   int  ci = tvolume->GetLineColor();
   char transp = tvolume->GetTransparency();

   TColor* c = gROOT->GetColor(ci);
   float rgba[4] = {1, 0, 0, 1.0f - transp/100.0f};
   if (c) {
      rgba[0] = c->GetRed();
      rgba[1] = c->GetGreen();
      rgba[2] = c->GetBlue();
   }
   eveShape->SetRGBA(rgba);
   rgba[3] = 1;
   c = gROOT->GetColor(TColor::GetColorDark(ci));
   if (c) {
      rgba[0] = c->GetRed();
      rgba[1] = c->GetGreen();
      rgba[2] = c->GetBlue();
   }
   eveShape->SetRGBALine(rgba);

   bool rnr     = tnode->IsVisible();
   bool rnr_els = tnode->IsVisDaughters();

   rnr     = rnr     && tvolume->IsVisible();
   rnr_els = rnr_els && tvolume->IsVisDaughters();

   eveShape->SetRnrSelf(rnr);
   eveShape->SetRnrElements(rnr_els);
   eveShape->SetRnrFrame(true);
   eveShape->SetMiniFrame(true);
   eveShape->SetShape(tshape);

   return eveShape;
}



TEveGeoShapeExtract* DumpShapeTree(TGeoNode*            geon,
                                   TEveGeoShapeExtract* parent,
                                   Bool_t               leafs_only, ProgramOptions &prgOpts)
{
   static const TEveException eh("DumpShapeTree ");

   static std::vector<std::string> volume_names = split(prgOpts.fVisibleVolumePattern, '/');
   static std::deque<std::string> currStack;

   TGeoNode*   tnode   = 0;
   TGeoVolume* tvolume = 0;
   TGeoShape*  tshape  = 0;

   tnode = geon;

   if (tnode == 0)
   {
      Info(eh, "Null TGeoNode for TEveGeoNode '%s': assuming it's a holder and descending.", geon->GetName());
   }
   else
   {
      tvolume = tnode->GetVolume();
      if (tvolume == 0) {
         Warning(eh, "Null TGeoVolume for TEveGeoNode '%s'; skipping its sub-tree.\n", geon->GetName());
         return 0;
      }
      tshape  = tvolume->GetShape();
      if (tshape->IsComposite())
      {
         Warning(eh, "Failed extracting CSG tesselation TEveGeoNode '%s'; skipping its sub-tree.\n", geon->GetName());
         return 0;
      }
   }

   currStack.push_back(geon->GetName());

   // Keep track of current level with respect to the first call passing the top node
   int currLevel = currStack.size();

   // transformation
   TEveTrans trans;
   if (parent)
      trans.SetFromArray(parent->GetTrans());
   if (tnode)
   {
      TGeoMatrix     *gm = tnode->GetMatrix();
      const Double_t *rm = gm->GetRotationMatrix();
      const Double_t *tv = gm->GetTranslation();
      TEveTrans t;
      t(1,1) = rm[0]; t(1,2) = rm[1]; t(1,3) = rm[2];
      t(2,1) = rm[3]; t(2,2) = rm[4]; t(2,3) = rm[5];
      t(3,1) = rm[6]; t(3,2) = rm[7]; t(3,3) = rm[8];
      t(1,4) = tv[0]; t(2,4) = tv[1]; t(3,4) = tv[2];
      trans *= t;
   }

   TEveGeoShapeExtract* gse = new TEveGeoShapeExtract(geon->GetName(), geon->GetTitle());
   gse->SetTrans(trans.Array());
   Int_t  ci = 0;
   Char_t transp = 0;
   if (tvolume) {
      ci = tvolume->GetLineColor();
      transp = tvolume->GetTransparency();
   }
   TColor* c = gROOT->GetColor(ci);
   Float_t rgba[4] = {1, 0, 0, 1.0f - transp/100.0f};
   if (c) {
      rgba[0] = c->GetRed();
      rgba[1] = c->GetGreen();
      rgba[2] = c->GetBlue();
   }
   gse->SetRGBA(rgba);
   rgba[3] = 1;
   c = gROOT->GetColor(TColor::GetColorDark(ci));
   if (c) {
      rgba[0] = c->GetRed();
      rgba[1] = c->GetGreen();
      rgba[2] = c->GetBlue();
   }
   gse->SetRGBALine(rgba);

   // Keep default extract line color --> black.
   Bool_t rnr     = false; //tnode->IsVisible();
   Bool_t rnr_els = false; //tnode->IsVisDaughters();

   std::string currNodeName( geon->GetName() );

   if ( currLevel <= volume_names.size() && boost::regex_match(currNodeName, boost::regex(volume_names[currLevel - 1])) ) {
      rnr = true;
      rnr_els = true;
      std::cout<< " - match" << std::endl;
   } else {
      std::cout<< " - no match" << std::endl;
      currStack.pop_back();
      return 0;
   }

   gse->SetRnrSelf    (rnr);
   gse->SetRnrElements(rnr_els);
   gse->SetRnrFrame   (false);
   gse->SetMiniFrame  (false);

   TObjArray *daughters = tvolume->GetNodes();

   gse->SetShape((leafs_only && daughters && daughters->GetEntries()) ? 0 : tshape);

   if (daughters && daughters->GetEntries())
   {
      TList* ele = new TList();
      gse->SetElements(ele);
      gse->GetElements()->SetOwner(true);

      ROOT::Detail::TTypedIter<TGeoNode> next(daughters);

      while (auto daughterNode = next())
      {
         DumpShapeTree(daughterNode, gse, leafs_only, prgOpts);
      }
   }

   if (parent)
      parent->GetElements()->Add(gse);

   currStack.pop_back();
   return gse;
}
