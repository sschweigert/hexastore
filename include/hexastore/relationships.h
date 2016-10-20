#ifndef _HEAXASTORE_RELATIONSHIPS_H_
#define _HEAXASTORE_RELATIONSHIPS_H_

#include <hexastore/datatypes.h>

inline HexastoreDataType* getFriend()
{
	static HexastoreValueType value = "Friend";
	static HexastoreDataType data = HexastoreDataType(&value);
	return &data;
}

#endif
