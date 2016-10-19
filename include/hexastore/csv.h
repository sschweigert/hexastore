#ifndef _HEXASTORE_CSV_H_
#define _HEXASTORE_CSV_H_

#include <hexastore/datatypes.h>

#include <vector>
#include <string>

typedef std::vector<HexastoreValueType> NameStore;

/* Reads csv file and returns the lines as array of C string */
NameStore read_name_store(char* filename);

#endif
