#ifndef _HEXASTORE_DATASET_H_
#define _HEXASTORE_DATASET_H_

#include <hexastore/fundamental_types.h>

#include <vector>

class DataSet : public std::vector<HexastoreDataType*>
{

	public:

		~DataSet()
		{
			for (auto& value : *this)
			{
				delete value;
			}
		}

};

#endif
