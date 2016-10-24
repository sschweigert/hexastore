#include <hexastore/triangle_detection.h>

#include <hexastore/hexastore.h>

std::vector<QueryChain> findDirectedTriangles(Hexastore& hexastore)
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

std::vector<QueryResult> findNonDirectedTriangles(Hexastore& hexastore)
{
	std::vector<QueryResult> toReturn;

	RootType rootType = spo;
	
	for (auto& triangleRoot : hexastore.roots[rootType].data)
	{
		HexastoreDataType* topNode = triangleRoot.first;
		
		std::vector<QueryChain> twoConnections;

		QueryChain buildingQuery;
		buildingQuery.insert(topNode);
		runQuery<NotRoot, NotRoot, Push>(hexastore, twoConnections, buildingQuery, rootType);

		for (auto& connection : twoConnections)
		{
			std::vector<QueryChain> connectionsToDouble = hexastore.getConnections(topNode, connection.back(), rootType);
			for (auto& triangleClosingChain : connectionsToDouble)
			{
				QueryResult newResult;
				newResult.push_back(connection);
				newResult.push_back(triangleClosingChain);
			}
		}
	}

	return toReturn;
}
