#ifndef _HEXASTORE_HEXASTORE_H_
#define _HEXASTORE_HEXASTORE_H_

#include <hexastore/datatypes.h>
#include <hexastore/internals.h>
#include <hexastore/query_chain.h>

#include <vector>

// Forward declare so I have more flexibility with ordering of this file
class BottomNode;
class MiddleNode;
class RootNode;

typedef enum { spo, sop, pso, pos, osp, ops } RootType;

struct Hexastore
{

	public:

		void remove(HexastoreDataType* subject, HexastoreDataType* predicate, HexastoreDataType* object);

		void insert(HexastoreDataType* subject, HexastoreDataType* predicate, HexastoreDataType* object);

		bool contains(HexastoreDataType* subject, HexastoreDataType* predicate, HexastoreDataType* object);

		std::vector<QueryChain> getConnectedVertices(HexastoreDataType* top, RootType rootType);

		std::vector<QueryChain> getConnections(HexastoreDataType* root, HexastoreDataType* bottom, RootType rootType);

	public:

		RootNode roots[6];	


};



#endif
