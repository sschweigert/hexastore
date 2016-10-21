#ifndef _HEXASTORE_TRIANGLE_DETECTION_H_
#define _HEXASTORE_TRIANGLE_DETECTION_H_

#include <hexastore/hexastore.h>
#include <hexastore/datatypes.h>
#include <hexastore/query_chain.h>

std::vector<QueryChain> findAllDirectedTriangles(Hexastore& store);

std::vector<QueryChain> findDescendingTriangles(Hexastore& store, RootType rootNode);

#endif
