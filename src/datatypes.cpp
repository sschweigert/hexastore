#include <hexastore/datatypes.h>

int HexastoreDataType::indexCounter = 0;

HexastoreDataType::HexastoreDataType(HexastoreValueType* value) :
value(value),
index(indexCounter)
{
	indexCounter++;	
}
