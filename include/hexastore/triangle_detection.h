#ifndef _HEXASTORE_TRIANGLE_DETECTION_H_
#define _HEXASTORE_TRIANGLE_DETECTION_H_

#include <hexastore/hexastore.h>
#include <hexastore/query_chain.h>

// Finds all the triangles that form a directed loop
// ie. A -> B -> C -> A
std::vector<QueryChain> findDirectedTriangles(Hexastore& hexastore);

// Finds all the triangles that don't form a directed loop
// ie. A -> B -> C <- A
std::vector<QueryResult> findNonDirectedTriangles(Hexastore& hexastore);

#endif
