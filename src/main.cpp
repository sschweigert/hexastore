#include <hexastore/hexastore.h>
#include <hexastore/datastore.h>
#include <hexastore/datatypes.h>
#include <hexastore/output.h>
#include <hexastore/triangle_detection.h>
#include <hexastore/relationships.h>
#include <hexastore/query_iterator.h>
#include <hexastore/query_iterator_functors.h>

#include <string>

#include <iostream>

int main(int argc, char *argv[])
{
	const int datasetSize = 5;

	// Import some names from a .csv
	std::vector<HexastoreValueType> nameData = readNameCSV("../data/names.csv");

	// Create a dataset out of the names (records with unique indices, but not necessarily unique names)
	DataSet people(nameData, datasetSize);

	Hexastore hexastore;

	// Make each person a friend to the next person in the store.
	for (int i = 0; i < people.size(); i++)
	{
		int nextIndex = (i + 1) % people.size();
		hexastore.insert(people[i], getFriend(), people[nextIndex]);
	}

	hexastore.insert(people[2], getFriend(), people[0]);

	std::cout << "Relationships: " << std::endl;
	std::cout << hexastore << std::endl;

	AcceptAll acceptAll;
	Descending descending;
	Ascending ascending;

	//QueryIterator<AcceptAll> twoLength

	Descending one;
	Descending two;
	Descending three;
	Return returnToRoot;
	QueryIterator<Descending, Descending, Return> queryIterator(hexastore, one, one, returnToRoot);
	//QueryIterator<Descending, Descending, Descending> queryIterator(hexastore, one, two, three);
	//QueryIterator<Descending, Descending> queryIterator(hexastore, one, two);
	//QueryIterator<Descending> queryIterator(hexastore, one);
	
	int i = 0;
	while (queryIterator.hasNext())
	{
		queryIterator.next();
		i++;
	}
	std::cout << "Num found: " << i << std::endl;

	/*
	std::cout << "Result: " << std::endl;

	auto directedTriangles = findNonDirectedTriangles(hexastore); 

	std::cout << directedTriangles << std::endl;
	*/
	
	return 0;
}
