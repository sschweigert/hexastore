#include <hexastore/triangle_detection.h>

#include <hexastore/hexastore.h>

std::vector<QueryChain> findAllDirectedTriangles(Hexastore& store)
{
	std::vector<QueryChain> toReturn;
	std::vector<QueryChain> opsTriangles = findDescendingTriangles(store, ops);
	std::vector<QueryChain> spoTriangles = findDescendingTriangles(store, spo);
	toReturn.insert(toReturn.end(), opsTriangles.begin(), opsTriangles.end());
	toReturn.insert(toReturn.end(), spoTriangles.begin(), spoTriangles.end());
	return toReturn;	
}

std::vector<QueryChain> findDescendingTriangles(Hexastore& store, RootType rootType)
{
	std::vector<QueryChain> toReturn;

	for (auto& triangleRoot : store.roots[rootType].data)
	{
		HexastoreDataType* topNode = triangleRoot.first;
		MiddleNode& middleNode = triangleRoot.second;

		std::vector<QueryChain> connectionsToSecond = store.getConnectedVertices(topNode, rootType);

		for (QueryChain& connectionToSecond : connectionsToSecond)
		{
			if (*(connectionToSecond.back()) <= *topNode)
				continue;

			std::vector<QueryChain> connectionsToThird = store.getConnectedVertices(connectionToSecond.back(), rootType);

			for (QueryChain connectionToThird : connectionsToThird)
			{

				if (*(connectionToThird.back()) <= *(connectionToSecond.back()))
					continue;

				std::vector<QueryChain> connectionsBackToRoot = store.getConnections(connectionToThird.back(), topNode, rootType);
				for (QueryChain finalConnection : connectionsBackToRoot)
				{
					QueryChain newChain;
					newChain.insert(topNode);					
					newChain.extend(connectionToSecond);
					newChain.extend(connectionToThird);
					newChain.extend(finalConnection);
					toReturn.push_back(newChain);
				}
			}

		}
	}

	return toReturn;
}
