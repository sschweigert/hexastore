#ifndef _HEXASTORE_OUTPUT_H_
#define _HEXASTORE_OUTPUT_H_

#include <iostream>

#include <hexastore/datatypes.h>
#include <hexastore/query_chain.h>

std::ostream& operator<<(std::ostream& os, const HexastoreDataType& dataType);

std::ostream& operator<<(std::ostream& os, const QueryChain& dataType);


#endif
