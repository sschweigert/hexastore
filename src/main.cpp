#include <hexastore/hexastore.h>
#include <hexastore/csv.h>
#include <hexastore/utility.h>
#include <hexastore/datatypes.h>

#define HEXASTORE_DATASET_SIZE 400000

std::vector<HexastoreDataType> createPersonDataset(NameStore nameData, int length)
{
	std::vector<HexastoreDataType> toReturn(length);

	for (int i = 0; i < length; i++)
	{
		int normalizedIndex = i % nameData.size();		

		toReturn[i].value = &nameData[normalizedIndex];
		toReturn[i].index = i;
	}

	return toReturn;
}

int main(int argc, char *argv[])
{
	NameStore nameData = read_name_store("../data/names.csv");
	std::vector<HexastoreDataType> hexastoreData = createPersonDataset(nameData, HEXASTORE_DATASET_SIZE);
	
	return 0;
}
