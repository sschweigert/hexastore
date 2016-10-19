#ifndef _HEXASTORE_CSV_H_
#define _HEXASTORE_CSV_H_

/* A array of C strings with length attached */
struct NameStore 
{

	char** names;

	int numberOfNames;

};

/* Reads csv file and returns the lines as array of C string */
NameStore read_name_store(char* filename);

/* Frees NameStore memory */
void free_name_store(NameStore toDelete);

#endif
