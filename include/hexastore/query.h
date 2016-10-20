#ifndef _HEXASTORE_QUERY_H_
#define _HEXASTORE_QUERY_H_

#include <vector>
#include <hexastore/fundamental_types.h>

struct QueryNode 
{

	HexastoreDataType* record;	

	QueryNode* next;

	explicit QueryNode(HexastoreDataType* record) :
		record(record),
		next(NULL)
	{
	}

	QueryNode* bottomNode()
	{
		QueryNode* current = this;
		while (current->next != NULL)
		{
			current = current->next;
		}
		return current;
	}

	QueryNode* extend(QueryNode* addition)
	{
		QueryNode* oldBottom = bottomNode();
		QueryNode* localCounter = oldBottom;

		while (addition != NULL)
		{
			localCounter = localCounter->next = new QueryNode(addition->record);	

			if (addition == oldBottom)
				break;

			addition = addition->next;
		}	
		return this;
	}

	~QueryNode()
	{
		if (next != NULL)
			delete next;
	}

	QueryNode(const QueryNode&) = delete;
	QueryNode& operator=(const QueryNode&) = delete;
};

inline QueryNode* buildQuery()
{
	return NULL;
}

	template <typename... Args>
QueryNode* buildQuery(HexastoreDataType* current, Args... args)
{
	QueryNode* newNode = new QueryNode(current);
	newNode->next = buildQuery(args...);
	return newNode;
}

inline void cleanUp(std::vector<QueryNode*> queryNodes)
{
	for (auto& node : queryNodes)
	{
		delete node;
	}
}

#endif
