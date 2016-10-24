#include <hexastore/datastore.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <string>
#include <iostream>

std::vector<HexastoreValueType> readNameCSV(std::string filename)
{
	std::vector<HexastoreValueType> toReturn;
	FILE* stream = fopen(filename.c_str(), "r");

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
