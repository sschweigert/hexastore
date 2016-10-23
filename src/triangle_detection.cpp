#include <hexastore/triangle_detection.h>

#include <hexastore/hexastore.h>

std::vector<QueryChain> findAllDirectedTriangles(Hexastore& hexastore)
{
	std::vector<QueryChain> toReturn;

	RootType rootType = spo;

	for (auto& triangleRoot : hexastore.roots[rootType].data)
	{
		HexastoreDataType* topNode = triangleRoot.first;
		QueryChain buildingQuery;
		buildingQuery.insert(topNode);
		runQuery<DescendingNode, DescendingNode, ReturnToRoot, Push>(hexastore, toReturn, buildingQuery, rootType);
		runQuery<AscendingNode, AscendingNode, ReturnToRoot, Push>(hexastore, toReturn, buildingQuery, rootType);
	}
	return toReturn;	
}

/*
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
*/
