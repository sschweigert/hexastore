#ifndef _HEXASTORE_OUTPUT_H_
#define _HEXASTORE_OUTPUT_H_

#include <iostream>

#include <hexastore/datatypes.h>

std::ostream& operator<<(std::ostream& os, const QueryNode& query);

std::ostream& operator<<(std::ostream& os, const HexastoreDataType& dataType);


#endif
