#include <iostream>
#include <set>
#include <vector>

#include <hexastore/hexastore.h>
#include <hexastore/operations.h>

/*
template <class Container>
void freeContainerPtrs(Container& container)
{
	for (auto& ptr : container)
	{
		delete ptr.subject;
		delete ptr.predicate;
		delete ptr.object;
	}
	container.clear();
}
*/

std::vector<Triple> createDataset()
{
	Triple firstTriple;	
	firstTriple.subject = std::string("Sebastian");
	firstTriple.predicate = std::string("Knows");
	firstTriple.object = std::string("Everything");

	Triple secondTriple;	
	secondTriple.subject = std::string("The Cat");
	secondTriple.predicate = std::string("Is in");
	secondTriple.object = std::string("The Bag");

	std::vector<Triple> triples;
	triples.push_back(firstTriple);
	triples.push_back(secondTriple);
	return triples;
}

int main(int argc, char *argv[])
{
	std::vector<Triple> triples = createDataset();
	Hexastore hexastore;
	
	hexastore.insert(triples[0]);

	std::cout << "Contains: " <<  hexastore.contains(triples[0]) << std::endl;
	std::cout << "Doesn't contain: " <<  hexastore.contains(triples[1]) << std::endl;
	return 0;
}
