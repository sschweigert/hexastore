#include <hexastore/triangle_detection.h>

#include <hexastore/hexastore.h>
#include <hexastore/generic_query.hpp>
#include <hexastore/query_template_functions.hpp>

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
