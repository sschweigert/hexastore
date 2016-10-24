#ifndef _HEXASTORE_DATASTORE_H_
#define _HEXASTORE_DATASTORE_H_

#include <hexastore/datatypes.h>
#include <hexastore/dataset.h>

#include <vector>
#include <string>

/* Reads csv file and returns the lines as array of C string */
std::vector<HexastoreValueType> readNameCSV(std::string filename);

#endif
