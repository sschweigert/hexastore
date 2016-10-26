#include <hexastore/dataset.h>

DataSet::DataSet(std::vector<HexastoreValueType>& nameData, int length)
{
	data.reserve(length);
	for (int i = 0; i < length; i++)
	{
		int normalizedIndex = i % nameData.size();		

		HexastoreDataType newEntry(&nameData[normalizedIndex]);

		data.push_back(newEntry);
	}
}

HexastoreDataType* DataSet::operator[](std::size_t idx)
{
	return &data[idx];
}

std::size_t DataSet::size() const
{
	return data.size();
}
