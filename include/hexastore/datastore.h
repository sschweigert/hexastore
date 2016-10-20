#ifndef _HEXASTORE_DATASTORE_H_
#define _HEXASTORE_DATASTORE_H_

#include <hexastore/datatypes.h>

#include <vector>
#include <string>

std::vector<HexastoreDataType*> createPersonDataset(std::vector<HexastoreValueType> nameData, int length);

/* Reads csv file and returns the lines as array of C string */
std::vector<HexastoreValueType> readNameCSV(char* filename);

#endif
