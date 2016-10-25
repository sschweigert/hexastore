#include <hexastore/dataset.h>

Dataset buildDataset(std::vector<HexastoreValueType>& nameData, int length)
{
	std::vector<HexastoreDataType> toReturn;
	toReturn.reserve(length);

	for (int i = 0; i < length; i++)
	{
		int normalizedIndex = i % nameData.size();		

		HexastoreDataType newEntry(&nameData[normalizedIndex]);

		toReturn.push_back(newEntry);
	}
	return toReturn;
}
