#ifndef VxEff_h
#define VxEff_h

#include <map>
#include <string>
#include <vector>


// Because this file is processed by rootcint we cannot use nice C++11
// initializatoin here, instead need to use extern
extern const std::map<std::string, std::string> myDefaultVertexFiles;

int VxEff(const std::vector<std::string> &vtx_file_keys, const std::vector<std::string> &vtx_file_names);
int VxEff(const std::map<std::string, std::string> & vtx_file_names=myDefaultVertexFiles);

#endif
