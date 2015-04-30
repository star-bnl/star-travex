#ifndef StiHistContainer_h
#define StiHistContainer_h

#include <map>
#include <set>
#include <string>

#include "TH1.h"
#include "TDirectoryFile.h"

#include "StiRootIO/TStiEvent.h"


typedef std::map<std::string, TH1*>                    HistMap;
typedef std::map<std::string, TH1*>::iterator          HistMapIter;
typedef std::map<std::string, TH1*>::const_iterator    HistMapConstIter;


class StiHistContainer : public TDirectoryFile
{
public:

   StiHistContainer(const char* name, TDirectory* motherDir=nullptr, Option_t* option="");
   ~StiHistContainer();

   const HistMap& GetHists() const { return mHs; }
   virtual void FillDerivedHists() = 0;
   void SaveAllAs(std::string prefix="./");

protected:

   virtual void BookHists() = 0;

   HistMap mHs;
};

#endif
