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

#endif
