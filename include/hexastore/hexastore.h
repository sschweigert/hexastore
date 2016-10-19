#ifndef _HEXASTORE_HEXASTORE_H_
#define _HEXASTORE_HEXASTORE_H_

#include <map>
#include <set>

/* A pointer to a C string */
typedef char* HexastoreValueType;

struct HexastoreDataType
{

	int index;
	
	HexastoreValueType value;

};

typedef enum { spo, sop, pso, pos, osp, ops } RootType;

struct BottomNode 
{

	std::set<HexastoreDataType*> data;

};

struct MiddleNode 
{

	std::map<HexastoreDataType*, BottomNode> data;	

};

struct RootNode
{

	std::map<HexastoreDataType*, MiddleNode> data;

};

struct Triple
{

	HexastoreDataType* subject;

	HexastoreDataType* predicate;

	HexastoreDataType* object;

};

struct Hexastore
{

	RootNode roots[6];	

};

#endif
