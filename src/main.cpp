#include <hexastore/hexastore.h>
#include <hexastore/datastore.h>
#include <hexastore/utility.h>
#include <hexastore/datatypes.h>
#include <hexastore/output.h>
#include <hexastore/triangle_detection.h>
#include <hexastore/relationships.h>

#include <iostream>

void cleanUp(std::vector<HexastoreDataType*>& data)
{
	for (auto& value : data)
	{
		delete value;
	}
}

int main(int argc, char *argv[])
{
	const int datasetSize = 4;
	std::vector<HexastoreValueType> nameData = readNameCSV("../data/names.csv");

	Hexastore hexastore;
	std::vector<HexastoreDataType*> people = createPersonDataset(nameData, datasetSize);

	for (auto& data : people)
	{
		std::cout << *data << std::endl;

	}

	for (int i = 0; i < people.size(); i++)
	{
		int nextIndex1 = (i + 1) % people.size();
		hexastore.insert(people[i], getFriend(), people[nextIndex1]);
	}

	hexastore.insert(people[2], getFriend(), people[0]);

	//std::vector<QueryNode*> directedTriangles = hexastore.getConnectedVertices(people[2], spo);
	std::vector<QueryNode*> directedTriangles = findAllDirectedTriangles(hexastore); 

	//directedTriangles[1]->extend(directedTriangles[0])->extend(directedTriangles[1]);
	
	for (auto& query : directedTriangles)
	{
		std::cout << *query << std::endl;
	}

	cleanUp(directedTriangles);
	cleanUp(people);
	return 0;
}
