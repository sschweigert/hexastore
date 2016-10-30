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

	// Add another relationship
	hexastore.insert(people[2], getFriend(), people[0]);

	std::cout << "Relationships: " << std::endl;
	std::cout << hexastore << "\n" << std::endl; 

	// Functors used for query/path generation
	AcceptAll acceptAll;
	Descending descending;
	Ascending ascending;
	NotTheRoot notTheRoot;

	QueryIterator<AcceptAll, AcceptAll> twoEdge(hexastore, acceptAll, acceptAll);
	QueryIterator<AcceptAll, AcceptAll, AcceptAll> threeEdge(hexastore, acceptAll, acceptAll, acceptAll);
	QueryIterator<AcceptAll, AcceptAll, AcceptAll, AcceptAll> fourEdge(hexastore, acceptAll, acceptAll, acceptAll, acceptAll);

	QueryIterator<Ascending, Ascending> twoAscending(hexastore, ascending, ascending);
	QueryIterator<Ascending, Ascending, Ascending> threeAscending(hexastore, ascending, ascending, ascending);
	QueryIterator<Ascending, Ascending, Ascending, Ascending> fourAscending(hexastore, ascending, ascending, ascending, ascending);

	auto directedTriangles = findDirectedTriangles(hexastore); 
	auto nonDirectedTriangles = findNonDirectedTriangles(hexastore); 

	std::cout << "Sub-paths with two edges:" << std::endl;
	std::cout << twoEdge << "\n" << std::endl;

	std::cout << "Sub-paths with three edges:" << std::endl;
	std::cout << threeEdge << "\n" << std::endl;

	std::cout << "Sub-paths with four edges:" << std::endl;
	std::cout << fourEdge << "\n" << std::endl;

	std::cout << "Ascending sub-paths with two edges:" << std::endl;
	std::cout << twoAscending << "\n" << std::endl;

	std::cout << "Ascending sub-paths with three edges:" << std::endl;
	std::cout << threeAscending << "\n" << std::endl;

	std::cout << "Ascending sub-paths with four edges:" << std::endl;
	std::cout << fourAscending << "\n" << std::endl;

	std::cout << "Directed triangles: " << std::endl;
	std::cout << directedTriangles << "\n" << std::endl;

	std::cout << "Non-directed triangles: " << std::endl;
	std::cout << nonDirectedTriangles << "\n" << std::endl;
	
	return 0;
}
