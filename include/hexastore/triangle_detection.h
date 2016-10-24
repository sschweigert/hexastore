#ifndef _HEXASTORE_TRIANGLE_DETECTION_H_
#define _HEXASTORE_TRIANGLE_DETECTION_H_

#include <hexastore/hexastore.h>
#include <hexastore/query_chain.h>

std::vector<QueryChain> findDirectedTriangles(Hexastore& hexastore);

std::vector<QueryResult> findNonDirectedTriangles(Hexastore& hexastore);

#endif
