#ifndef _HEXASTORE_TRIANGLE_DETECTION_H_
#define _HEXASTORE_TRIANGLE_DETECTION_H_

#include <hexastore/hexastore.h>

struct DirectedTriangle
{

	/* The nodes that form the triangle */
	HexastoreDataType* first;	
	HexastoreDataType* second;	
	HexastoreDataType* third;	

	/* The edges connecting the nodes */
	HexastoreDataType* firstToSecond;	
	HexastoreDataType* secondToThird;	
	HexastoreDataType* thirdToFirst;	

};

std::set<DirectedTriangle> findAllDirectedTriangles(Hexastore& store);

#endif
