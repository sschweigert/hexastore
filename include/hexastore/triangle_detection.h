#ifndef _HEXASTORE_TRIANGLE_DETECTION_H_
#define _HEXASTORE_TRIANGLE_DETECTION_H_

#include <hexastore/hexastore.h>
#include <hexastore/datatypes.h>
#include <hexastore/query_chain.h>
#include <hexastore/output.h>

std::vector<QueryChain> findAllDirectedTriangles(Hexastore& hexastore);

std::vector<QueryChain> findDescendingTriangles(Hexastore& hexastore, RootType rootNode);


template <unsigned int N, class ...Args>
struct descendNode
{

	void operator()(Hexastore& hexastore, std::vector<QueryChain>& buildingChain, HexastoreDataType* nextLevelNode, HexastoreDataType* topNode, RootType connectionType, Args... args)
	{
		std::vector<QueryChain> connectionsToSecond = hexastore.getConnectedVertices(nextLevelNode, connectionType);

		for (QueryChain& connectionToSecond : connectionsToSecond)
		{
			HexastoreDataType* connectedNode = connectionToSecond.back();

			if (*connectedNode <= *nextLevelNode)
				continue;

			descendNode<N - 1, QueryChain&, Args...>()(hexastore, buildingChain, connectedNode, topNode, connectionType, connectionToSecond, args...);
		}
	}


};

template <class ...Args>
struct descendNode<1, Args...>
{

	void operator()(Hexastore& hexastore, std::vector<QueryChain>& buildingChain, HexastoreDataType* nextLevelNode, HexastoreDataType* topNode, RootType connectionType, Args... args)
	{
		std::vector<QueryChain> connectionsBackToRoot = hexastore.getConnections(nextLevelNode, topNode, connectionType);

		for (QueryChain finalConnection : connectionsBackToRoot)
		{
			QueryChain newChain;
			newChain.insert(topNode);					
			newChain.extend(args...);
			newChain.extend(finalConnection);
			buildingChain.push_back(newChain);
		}
	}
};

#endif
