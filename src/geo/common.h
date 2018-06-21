#ifndef geo_common_header
#define geo_common_header

#include "common/root_v6.h"


TEveGeoShapeExtract* DumpShapeTree(TGeoNode*            geoNode,
                                   TEveGeoShapeExtract* parent,
                                   bool                 leafs_only);

#endif
