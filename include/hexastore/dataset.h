#ifndef _HEXASTORE_DATASET_H_
#define _HEXASTORE_DATASET_H_

#include <hexastore/fundamental_types.h>

#include <vector>

typedef std::vector<HexastoreDataType> Dataset;

Dataset buildDataset(std::vector<HexastoreValueType>& nameData, int length);

#endif
