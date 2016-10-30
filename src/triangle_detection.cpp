#include <hexastore/triangle_detection.h>

#include <hexastore/hexastore.h>
#include <hexastore/query_template_functions.hpp>
#include <hexastore/query_iterator.h>
#include <hexastore/query_iterator_functors.h>

// This algorithm is based on the idea that triangles are either ascending or descending
// ie. 0 -> 1 -> 2 -> 0   or   2 -> 1 -> 0 -> 2
// Any directed triangle can be expressed either as ascending from smallest node,
// or descending from the largest node.
// By only searching for triangles that start with these types of nodes, we can 
// avoid duplicates.
std::vector<QueryChain> findDirectedTriangles(Hexastore& hexastore)
{
	std::vector<QueryChain> toReturn;

	// Functors used to generate paths
	Ascending ascendingFunctor;
	Descending descendingFunctor;
	Return returnToRoot;

	std::vector<QueryChain> result;

	result = getQueryResults(hexastore, descendingFunctor, descendingFunctor, returnToRoot);
	toReturn.insert(toReturn.end(), result.begin(), result.end());

	result = getQueryResults(hexastore, ascendingFunctor, ascendingFunctor, returnToRoot);
	toReturn.insert(toReturn.end(), result.begin(), result.end());

	// This is the deprecated way. I want to do somet timing test to compare the two.
	// RootType rootType = spo;
	// Ascending order
	//hexastore.runQueryOnAllRoots<DescendingNode, DescendingNode, ReturnToRoot>(toReturn, rootType);

	// Descending order
	//hexastore.runQueryOnAllRoots<AscendingNode, AscendingNode, ReturnToRoot>(toReturn, rootType);

	return toReturn;	
}

// This algorithm is based on the idea that nonDirectedTriangles have one chain of two and
// one chain of one.   ie.   0 -> 1 -> 2 <- 0  
std::vector<QueryResult> findNonDirectedTriangles(Hexastore& hexastore)
{
	// This is currently implemented in the old deprecated way. I will switch it when I have time.


	std::vector<QueryResult> toReturn;

	RootType rootType = spo;
	
	for (auto& triangleRoot : hexastore.roots[rootType].data)
	{
		HexastoreDataType* topNode = triangleRoot.first;
		
		std::vector<QueryChain> twoConnections;

		QueryChain buildingQuery;
		buildingQuery.insert(topNode);

		// This query retrieve
		hexastore.runQuery<NotRoot, NotRoot>(twoConnections, buildingQuery, rootType);

		for (auto& connection : twoConnections)
		{
			std::vector<QueryChain> connectionsToDouble = hexastore.getConnections(topNode, connection.back(), rootType);
			for (auto& triangleClosingChain : connectionsToDouble)
			{
				// This is wasteful with resources and should be changed
				QueryChain closingChain(topNode, triangleClosingChain);

				QueryResult newResult;
				newResult.push_back(connection);
				newResult.push_back(closingChain);

				toReturn.push_back(newResult);
			}
		}
	}

	return toReturn;
}
