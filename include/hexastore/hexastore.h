#ifndef _HEXASTORE_HEXASTORE_H_
#define _HEXASTORE_HEXASTORE_H_

#include <hexastore/datatypes.h>
#include <hexastore/internals.h>

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

	private:

		RootNode roots[6];	


};



#endif
