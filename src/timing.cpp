#include <hexastore/hexastore.h>
#include <hexastore/datastore.h>
#include <hexastore/datatypes.h>
#include <hexastore/output.h>
#include <hexastore/triangle_detection.h>
#include <hexastore/relationships.h>

#include <string>

#include <iostream>
#include <ctime>

// This file needs to be refactored.

void horizontalLine()
{
	std::cout << "-------------------------------" << std::endl;
}

int main(int argc, char *argv[])
{

	const int datasetSize = 600000;
	std::vector<HexastoreValueType> nameData = readNameCSV("../data/names.csv");

	Hexastore hexastore;
	DataSet people(nameData, datasetSize);

	horizontalLine();
	std::cout << "Element insertion:" << std::endl;

	std::clock_t start;
	start = std::clock();

	for (int i = 0; i < people.size(); i++)
	{
		int nextIndex = (i + 1) % people.size();
		int nextNextIndex = (i + 2) % people.size();
		hexastore.insert(people[i], getFriend(), people[nextIndex]);
		hexastore.insert(people[nextIndex], getFriend(), people[i]);
		hexastore.insert(people[i], getFriend(), people[nextNextIndex]);
	}

	std::cout << "Steps taken to insert elements: " << (std::clock() - start) << std::endl;

	horizontalLine();
	std::cout << "Triangle detection:" << std::endl;
	
	start = std::clock();
	std::vector<QueryChain> directedTriangles = findDirectedTriangles(hexastore); 
	std::cout << "Steps taken to find triangles: " << (std::clock() - start) << std::endl;
	std::cout << directedTriangles.size() << " triangles found. " << std::endl;

	return 0;
}
