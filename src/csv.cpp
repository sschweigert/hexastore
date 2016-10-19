#include <hexastore/csv.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


NameStore read_name_store(char* filename)
{
	NameStore toReturn;
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
