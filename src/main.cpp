#include <hexastore/hexastore.h>
#include <hexastore/operations.h>
#include <hexastore/csv.h>
#include <hexastore/utility.h>

#define HEXASTORE_DATASET_SIZE 400000

HexastoreDataType* createDataset(NameStore nameData, int length)
{
	HexastoreDataType* toReturn;

	toReturn = (HexastoreDataType*)malloc(length * sizeof(HexastoreDataType));	
	for (int i = 0; i < length; i++)
	{
		int normalizedValue = i % nameData.numberOfNames;		
		toReturn[i].value = nameData.names[normalizedValue];
		toReturn[i].index = i;
	}

	return toReturn;
}

int main(int argc, char *argv[])
{
	NameStore nameData = read_name_store("../data/names.csv");
	HexastoreDataType* hexastoreData = createDataset(nameData, HEXASTORE_DATASET_SIZE);
	free(hexastoreData);
	free_name_store(nameData);
	return 0;
}
