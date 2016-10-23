#include <hexastore/hexastore.h>
#include <hexastore/datastore.h>
#include <hexastore/utility.h>
#include <hexastore/datatypes.h>
#include <hexastore/output.h>
#include <hexastore/triangle_detection.h>
#include <hexastore/relationships.h>
#include <hexastore/query_chain.h>

#include <string>

#include <iostream>

int main(int argc, char *argv[])
{

	const int datasetSize = 4;
	std::vector<HexastoreValueType> nameData = readNameCSV("../data/names.csv");

	Hexastore hexastore;
	DataSet people = createPersonDataset(nameData, datasetSize);

	std::cout << "Dataset: " << std::endl;
	for (auto data : people)
	{
		std::cout << *data << std::endl;

	}
	std::cout << "------------------------------" << std::endl;


	for (int i = 0; i < people.size(); i++)
	{
		int nextIndex = (i + 1) % people.size();
		hexastore.insert(people[nextIndex], getFriend(), people[i]);
	}

	hexastore.insert(people[0], getFriend(), people[2]);

	std::cout << "Relationships: " << std::endl;
	for (auto person : people)
	{
		std::vector<QueryChain> chains = hexastore.getConnectedVertices(person, spo);
		for (auto chain : chains)
		{
			QueryChain tempChain;
			tempChain.insert(person);
			tempChain.extend(chain);
			std::cout << tempChain << std::endl;
		}
	}

	std::cout << "Result: " << std::endl;

	//std::vector<QueryChain> directedTriangles = hexastore.getConnectedVertices(people[2], spo);
	std::vector<QueryChain> directedTriangles = findAllDirectedTriangles(hexastore); 

	//directedTriangles[1]->extend(directedTriangles[0])->extend(directedTriangles[1]);

	for (auto& query : directedTriangles)
	{
		std::cout << query << std::endl;
	}

	return 0;
}
