#include <hexastore/triangle_detection.h>

#include <hexastore/hexastore.h>

std::vector<QueryNode*> findAllDirectedTriangles(Hexastore& store)
{
	std::vector<QueryNode*> toReturn;

	for (auto& triangleRoot : store.roots[spo].data)
	{
		HexastoreDataType* topNode = triangleRoot.first;
		MiddleNode& middleNode = triangleRoot.second;
				
		std::vector<QueryNode*> connectionsToSecond = store.getConnectedVertices(topNode, spo);

		for (QueryNode* connectionToSecond : connectionsToSecond)
		{
			HexastoreDataType* connectedSecondNode = connectionToSecond->next->record;
			if (connectedSecondNode->index < topNode->index)
				continue;

			std::vector<QueryNode*> connectionsToThird = store.getConnectedVertices(connectedSecondNode, spo);

			for (QueryNode* connectionToThird : connectionsToThird)
			{
				HexastoreDataType* connectedThirdNode = connectionToThird->next->record;
				if (connectedThirdNode->index < connectedSecondNode->index || connectedThirdNode->index == topNode->index)
				{
				}
				else
				{
					QueryNode* newQuery = new QueryNode(topNode);					
					newQuery->extend(connectionToSecond);
					newQuery->extend(connectionToThird);
					toReturn.push_back(newQuery);
				}
			}
	
		}
	}

	return toReturn;
}
