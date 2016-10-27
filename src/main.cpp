#include <hexastore/hexastore.h>
#include <hexastore/datastore.h>
#include <hexastore/datatypes.h>
#include <hexastore/output.h>
#include <hexastore/triangle_detection.h>
#include <hexastore/relationships.h>

#include <string>

#include <iostream>

int main(int argc, char *argv[])
{

	const int datasetSize = 4;
	std::vector<HexastoreValueType> nameData = readNameCSV("../data/names.csv");

	Hexastore hexastore;
	DataSet people(nameData, datasetSize);

	std::cout << "Dataset: " << std::endl;
	//std::cout << people << std::endl;
	std::cout << "------------------------------" << std::endl;

	for (int i = 0; i < people.size(); i++)
	{
		int nextIndex = (i + 1) % people.size();
		hexastore.insert(people[i], getFriend(), people[nextIndex]);
	}

	hexastore.insert(people[0], getFriend(), people[2]);

	std::cout << "Relationships: " << std::endl;
	/*
	for (auto person : people)
	{
		std::vector<QueryChain> chains = hexastore.getConnectedVertices(person, spo);
		for (auto chain : chains)
		{
			QueryChain tempChain;
			tempChain.insert(person);
			tempChain.insert(chain);
			std::cout << tempChain << std::endl;
		}
	}
	*/

	std::cout << "Result: " << std::endl;

	auto directedTriangles = findNonDirectedTriangles(hexastore); 

	std::cout << directedTriangles << std::endl;
	
	return 0;
}
