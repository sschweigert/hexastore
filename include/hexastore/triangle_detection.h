#ifndef _HEXASTORE_TRIANGLE_DETECTION_H_
#define _HEXASTORE_TRIANGLE_DETECTION_H_

#include <hexastore/hexastore.h>
#include <hexastore/datatypes.h>
#include <hexastore/query_chain.h>
#include <hexastore/output.h>

std::vector<QueryChain> findAllDirectedTriangles(Hexastore& hexastore);

struct DescendingNode
{

	static inline std::vector<QueryChain> getLeads(Hexastore& hexastore, QueryChain& querySoFar, RootType connectionType)
	{
		return hexastore.getConnectedVertices(querySoFar.back(), connectionType);
	}

	static inline bool acceptLead(QueryChain& lead, QueryChain& querySoFar)
	{
		HexastoreDataType* leadNode = lead.back();
		HexastoreDataType* prevNode = querySoFar.back();;

		return (*leadNode < *prevNode);
	}	

};

struct AscendingNode 
{

	static inline std::vector<QueryChain> getLeads(Hexastore& hexastore, QueryChain& querySoFar, RootType connectionType)
	{
		return hexastore.getConnectedVertices(querySoFar.back(), connectionType);
	}

	static inline bool acceptLead(QueryChain& lead, QueryChain& querySoFar)
	{
		HexastoreDataType* leadNode = lead.back();
		HexastoreDataType* prevNode = querySoFar.back();;

		return (*leadNode > *prevNode);
	}	

};

struct ReturnToRoot 
{

	static inline std::vector<QueryChain> getLeads(Hexastore& hexastore, QueryChain& querySoFar, RootType connectionType)
	{
		return hexastore.getConnections(querySoFar.back(), querySoFar.front(), connectionType);
	}

	static inline bool acceptLead(QueryChain& lead, QueryChain& querySoFar)
	{
		return true;
	}	

};

struct Push 
{};

template <class SearchStrategy, class ...Args>
void runQuery(Hexastore& hexastore, std::vector<QueryChain>& buildingChain, QueryChain& querySoFar, RootType connectionType)
{
	std::vector<QueryChain> leads = SearchStrategy::getLeads(hexastore, querySoFar, connectionType);
	
	for (QueryChain& lead : leads)
	{
		if (SearchStrategy::acceptLead(lead, querySoFar))
		{
			querySoFar.extend(lead);
			runQuery<Args...>(hexastore, buildingChain, querySoFar, connectionType);
			querySoFar.pop_back();
			querySoFar.pop_back();
		}
	}
}

template <>
inline void runQuery<Push>(Hexastore& hexastore, std::vector<QueryChain>& buildingChain, QueryChain& querySoFar, RootType connectionType)
{
	buildingChain.push_back(querySoFar);
}

template <unsigned int N, class ...Args>
struct descendNode
{

	void operator()(Hexastore& hexastore, std::vector<QueryChain>& buildingChain, HexastoreDataType* nextLevelNode, HexastoreDataType* topNode, RootType connectionType, Args... args)
	{
		std::vector<QueryChain> connections = hexastore.getConnectedVertices(nextLevelNode, connectionType);

		for (QueryChain& connection : connections)
		{
			HexastoreDataType* connectedNode = connection.back();

			if (*connectedNode <= *nextLevelNode)
				continue;

			descendNode<N - 1, QueryChain&, Args...>()(hexastore, buildingChain, connectedNode, topNode, connectionType, connection, args...);
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
