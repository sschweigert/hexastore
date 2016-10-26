#ifndef _HEXASTORE_QUERY_TEMPLATE_FUNCTIONS_H_
#define _HEXASTORE_QUERY_TEMPLATE_FUNCTIONS_H_


struct DescendingNode
{

	static std::vector<QueryChain> getLeads(Hexastore& hexastore, QueryChain& querySoFar, RootType connectionType)
	{
		return hexastore.getConnectedVertices(querySoFar.back(), connectionType);
	}

	static bool acceptLead(QueryChain& lead, QueryChain& querySoFar)
	{
		HexastoreDataType* leadNode = lead.back();
		HexastoreDataType* prevNode = querySoFar.back();;

		return (*leadNode < *prevNode);
	}	

};

struct AscendingNode 
{

	static std::vector<QueryChain> getLeads(Hexastore& hexastore, QueryChain& querySoFar, RootType connectionType)
	{
		return hexastore.getConnectedVertices(querySoFar.back(), connectionType);
	}

	static bool acceptLead(QueryChain& lead, QueryChain& querySoFar)
	{
		HexastoreDataType* leadNode = lead.back();
		HexastoreDataType* prevNode = querySoFar.back();;

		return (*leadNode > *prevNode);
	}	

};

struct ReturnToRoot 
{

	static std::vector<QueryChain> getLeads(Hexastore& hexastore, QueryChain& querySoFar, RootType connectionType)
	{
		return hexastore.getConnections(querySoFar.back(), querySoFar.front(), connectionType);
	}

	static bool acceptLead(QueryChain& lead, QueryChain& querySoFar)
	{
		return true;
	}	

};

struct NotRoot 
{

	static std::vector<QueryChain> getLeads(Hexastore& hexastore, QueryChain& querySoFar, RootType connectionType)
	{
		return hexastore.getConnectedVertices(querySoFar.back(), connectionType);
	}

	static bool acceptLead(QueryChain& lead, QueryChain& querySoFar)
	{
		return (lead.back() != querySoFar.front());
	}	

};

// This only exists as a placeholder in the template. I specialize it to push the
// query being built onto the vector of query chains.
// It is meant to be the last item in the template list, and it finishes the chain.
struct Push 
{};

// Specialization of runQuery which pushes 
template <>
inline void runQuery<Push>(Hexastore& hexastore, std::vector<QueryChain>& buildingChain, QueryChain& querySoFar, RootType connectionType)
{
	buildingChain.push_back(querySoFar);
}

#endif
