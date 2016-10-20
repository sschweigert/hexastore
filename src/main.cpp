#include <hexastore/hexastore.h>
#include <hexastore/datastore.h>
#include <hexastore/utility.h>
#include <hexastore/datatypes.h>
#include <hexastore/output.h>
#include <hexastore/triangle_detection.h>

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
	const int datasetSize = 4e1;
	std::vector<HexastoreValueType> nameData = readNameCSV("../data/names.csv");

	Hexastore hexastore;
	std::vector<HexastoreDataType*> people = createPersonDataset(nameData, datasetSize);

	for (int i = 0; i < datasetSize; i++)
	{
		int nextIndex1 = (i + 1) % datasetSize;
		int nextIndex2 = (i + 2) % datasetSize;
		hexastore.insert(people[i], people[nextIndex1], people[nextIndex2]);
	}

	hexastore.insert(people[2], people[1], people[0]);

	std::vector<QueryNode*> directedTriangles = hexastore.getConnectedVertices(people[2], spo);

	for (auto& query : directedTriangles)
	{
		std::cout << *query << std::endl;
	}

	cleanUp(people);
	return 0;
}
