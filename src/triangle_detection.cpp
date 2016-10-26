#include <hexastore/triangle_detection.h>

#include <hexastore/hexastore.h>
#include <hexastore/query_template_functions.hpp>

// This algorithm is based on the idea that triangles are either ascending or descending
// ie. 0 -> 1 -> 2 -> 0   or   2 -> 1 -> 0 -> 2

std::vector<QueryChain> findDirectedTriangles(Hexastore& hexastore)
{
	std::vector<QueryChain> toReturn;

	RootType rootType = spo;

	hexastore.runHexastoreQuery<DescendingNode, DescendingNode, ReturnToRoot>(toReturn, rootType);
	hexastore.runHexastoreQuery<AscendingNode, AscendingNode, ReturnToRoot>(toReturn, rootType);
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
		hexastore.runQuery<NotRoot, NotRoot, Push>(twoConnections, buildingQuery, rootType);

		for (auto& connection : twoConnections)
		{
			std::vector<QueryChain> connectionsToDouble = hexastore.getConnections(topNode, connection.back(), rootType);
			for (auto& triangleClosingChain : connectionsToDouble)
			{
				// This is wasteful with resources and should be changed
				QueryChain closingChain;
				closingChain.insert(topNode);
				closingChain.extend(triangleClosingChain);

				QueryResult newResult;
				newResult.push_back(connection);
				newResult.push_back(closingChain);
				toReturn.push_back(newResult);
			}
		}
	}

	return toReturn;
}
