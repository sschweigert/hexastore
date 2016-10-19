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

	toReturn.names = (char**)malloc(num_lines * sizeof(char*));	
	toReturn.numberOfNames = num_lines;
	while (fgets(line, 1024, stream))
	{
		toReturn.names[current_line] = strdup(line);
		current_line++;
	}
	fclose(stream);

	return toReturn;	
}

void free_name_store(NameStore toDelete)
{
	int i;
	for (i = 0; i < toDelete.numberOfNames; i++)
	{
		free(toDelete.names[i]);
	}
	free(toDelete.names);
}
