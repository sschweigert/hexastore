#include <hexastore/triangle_detection.h>

#include <hexastore/hexastore.h>

std::vector<QueryChain> findAllDirectedTriangles(Hexastore& hexastore)
{
	std::vector<QueryChain> toReturn;
	std::vector<QueryChain> opsTriangles = findDescendingTriangles(hexastore, ops);
	std::vector<QueryChain> spoTriangles = findDescendingTriangles(hexastore, spo);
	toReturn.insert(toReturn.end(), opsTriangles.begin(), opsTriangles.end());
	toReturn.insert(toReturn.end(), spoTriangles.begin(), spoTriangles.end());
	return toReturn;	
}

std::vector<QueryChain> findDescendingTriangles(Hexastore& hexastore, RootType rootType)
{
	std::vector<QueryChain> toReturn;

	for (auto& triangleRoot : hexastore.roots[rootType].data)
	{
		HexastoreDataType* topNode = triangleRoot.first;
		descendNode<3>()(hexastore, toReturn, topNode, topNode, rootType);
	}

	return toReturn;
}
