#ifndef _HEXASTORE_DATATYPES_H_
#define _HEXASTORE_DATATYPES_H_

#include <string>
#include <vector>
#include <iostream>

// The concrete data associated with
typedef std::string HexastoreValueType;

// Defines structured entry in the hexastore
struct HexastoreDataType
{

	// I added this unique index because my triangle algorithm requires an explcit
	// ordering of the nodes in the store. This helps eliminate duplicate triangles
	// with rearranged ordering: 
	// [A -> B -> C -> A] vs 
	// [B -> C -> A -> B] vs 
	// [C -> A -> B -> C]  
	//
	// I am able to filter out these duplicates by only considering child nodes with
	// larger index than parents. For any given triangle there will be only one
	// such ordering in one direction.
	//
	// I initially considered using the HexastoreValueType pointer as this index, but 
	// I found that pointer comparison operators only work if the pointers are part of 
	// the same array or class, as per the C++ specification. Many compilers still support
	// comparison operators for all pointers, even those not in the same array, but I
	// wanted to be safe. The index is not needed if the HexastoreDataType
	// objects are all allocated in the same array before being loaded into the hexastore.
	int index;
	
	// Each entry in the hexastore contains a pointer to the data of the entry. This means
	// that multiple hexastore entries can point to the same data, with the hexastore
	// treating each one as a unique entry. 
	HexastoreValueType* value;

	HexastoreDataType(HexastoreValueType* value);

	static int indexCounter;

};

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
