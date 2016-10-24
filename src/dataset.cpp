#include <hexastore/dataset.h>


DataSet::~DataSet()
{
	for (auto& value : *this)
	{
		delete value;
	}
}

DataSet::DataSet(std::vector<HexastoreValueType>& nameData, int length)
{
	for (int i = 0; i < length; i++)
	{
		int normalizedIndex = i % nameData.size();		

		HexastoreDataType* newEntry = new HexastoreDataType(&nameData[normalizedIndex]);

		push_back(newEntry);
	}
}
