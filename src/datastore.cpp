#include <hexastore/datastore.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

std::vector<HexastoreDataType*> createPersonDataset(std::vector<HexastoreValueType> nameData, int length)
{
	std::vector<HexastoreDataType*> toReturn;

	for (int i = 0; i < length; i++)
	{
		int normalizedIndex = i % nameData.size();		

		HexastoreDataType* newEntry = new HexastoreDataType(&nameData[normalizedIndex]);

		toReturn.push_back(newEntry);
	}

	return toReturn;
}

std::vector<HexastoreValueType> readNameCSV(char* filename)
{
	std::vector<HexastoreValueType> toReturn;
	FILE* stream = fopen(filename, "r");

	char line[1024];
	int num_lines = 0;
	int current_line = 0;
	while (fgets(line, 1024, stream))
	{
		num_lines++;
	}
	rewind(stream);

	while (fgets(line, 1024, stream))
	{
		toReturn.push_back(std::string(line));
		current_line++;
	}
	fclose(stream);

	return toReturn;	
}
