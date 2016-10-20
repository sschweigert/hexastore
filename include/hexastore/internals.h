#ifndef _HEXASTORE_INTERNALS_H_
#define _HEXASTORE_INTERNALS_H_

#include <map>
#include <set>

struct BottomNode 
{

	public:

		bool contains(HexastoreDataType* bottom);

		void insert(HexastoreDataType* bottom);

		bool remove(HexastoreDataType* bottom);

		std::set<HexastoreDataType*> data;


};

struct MiddleNode 
{

	public:

		bool contains(HexastoreDataType* middle, HexastoreDataType* bottom);

		void insert(HexastoreDataType* middle, HexastoreDataType* bottom);

		bool remove(HexastoreDataType* middle, HexastoreDataType* bottom);

		std::map<HexastoreDataType*, BottomNode> data;	

};

struct RootNode
{

	public:

		bool contains(HexastoreDataType* top, HexastoreDataType* middle, HexastoreDataType* bottom);

		void insert(HexastoreDataType* top, HexastoreDataType* middle, HexastoreDataType* bottom);

		bool remove(HexastoreDataType* top, HexastoreDataType* middle, HexastoreDataType* bottom);

		std::map<HexastoreDataType*, MiddleNode> data;


};


struct Triple
{

	HexastoreDataType* subject;

	HexastoreDataType* predicate;

	HexastoreDataType* object;

};

#endif
