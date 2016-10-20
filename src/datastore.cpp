#include <hexastore/datastore.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <string>
#include <iostream>

DataSet createPersonDataset(std::vector<HexastoreValueType>& nameData, int length)
{
	DataSet toReturn;

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
	while (fgets(line, 1024, stream))
	{
		num_lines++;
	}
	rewind(stream);

	while (fgets(line, 1024, stream))
	{
		std::string newString(line);
		newString = newString.erase(newString.length() - 2);
		toReturn.push_back(newString);
	}
	fclose(stream);

	return toReturn;	
}
