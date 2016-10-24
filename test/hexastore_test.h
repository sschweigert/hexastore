#include <cxxtest/TestSuite.h>

#include <vector>

#include <hexastore/query_chain.h>
#include <hexastore/hexastore.h>
#include <hexastore/datastore.h>
#include <hexastore/relationships.h>
#include <hexastore/triangle_detection.h>

class AddingAndRemoving : public CxxTest::TestSuite
{

	public:

		// Override function which setups the tests
		void setUp()
		{
			names = readNameCSV("../data/names.csv");
			people = createPersonDataset(names, 1000);
		}

		void testCyclicalQueryChain(void)
		{
			QueryChain first;
			first.insert(people[0]);
			first.insert(people[1]);
			first.insert(people[2]);
			first.insert(people[0]);

			QueryChain second;
			second.insert(people[1]);
			second.insert(people[2]);
			second.insert(people[0]);

			// Not equivalent, one element missing
			TS_ASSERT(!cyclicEquivalent(first, second));

			second.insert(people[1]);

			// Element is added so now they're equivalent
			TS_ASSERT(cyclicEquivalent(first, second));

			// Should be equivalent to itself, of course
			TS_ASSERT(cyclicEquivalent(first, first));

			QueryChain third;
			third.insert(people[0]);
			third.insert(people[1]);
			third.insert(people[4]);
			third.insert(people[0]);

			// Make sure no false positives
			TS_ASSERT(!cyclicEquivalent(first, third));

		}

		void testInsertionRemoval(void)
		{
			Hexastore hexastore;

			hexastore.insert(people[0], people[1], people[2]);
			hexastore.insert(people[1], people[2], people[3]);

			// Make sure we can detect the insertion
			TS_ASSERT(hexastore.contains(people[0], people[1], people[2]));

			// Changing the order, this element isn't in the store
			TS_ASSERT(!hexastore.contains(people[2], people[1], people[0]));

			hexastore.remove(people[0], people[1], people[2]);

			// Check to make sure it was correctly removed
			TS_ASSERT(!hexastore.contains(people[0], people[1], people[2]));

			// Make sure the second one wasn't accidently removed
			TS_ASSERT(hexastore.contains(people[1], people[2], people[3]));

		}

		void testForwardDirectedTriangleDetection(void)
		{
			Hexastore hexastore;
			// Everybody is a friend of the person next to them
			for (int i = 0; i < people.size(); i++)
			{
				int nextIndex = (i + 1) % people.size();
				hexastore.insert(people[i], getFriend(), people[nextIndex]);
			}

			// Add more loop back from 2 -> 0
			hexastore.insert(people[2], getFriend(), people[0]);

			std::vector<QueryChain> directedTriangles = findDirectedTriangles(hexastore); 

			QueryChain expectedResult(people[0], getFriend(), people[1], getFriend(), people[2], getFriend(), people[0]);
			TS_ASSERT(cyclicEquivalent(expectedResult, directedTriangles.front()));
			TS_ASSERT(directedTriangles.size() == 1);
		}

		void testReverseDirectedTriangleDetection(void)
		{
			Hexastore hexastore;
			// Everybody is a friend of the person next to them
			for (int i = 0; i < people.size(); i++)
			{
				int nextIndex = (i + 1) % 3;
				hexastore.insert(people[nextIndex], getFriend(), people[i]);
			}

			// Add more loop back from 2 -> 0
			hexastore.insert(people[0], getFriend(), people[2]);

			std::vector<QueryChain> directedTriangles = findDirectedTriangles(hexastore); 

			// Check that the triangle was detected
			QueryChain expectedResult(people[0], getFriend(), people[2], getFriend(), people[1], getFriend(), people[0]);
			TS_ASSERT(cyclicEquivalent(expectedResult, directedTriangles.front()));
	
			// No spurious triangles detected
			TS_ASSERT(directedTriangles.size() == 1);
		}

		void testThreeNodeDoubleTriangle(void)
		{
			Hexastore hexastore;
			// Everybody is a friend of the person next to them
			for (int i = 0; i < 3; i++)
			{
				int nextIndex = (i + 1) % 3;
				hexastore.insert(people[nextIndex], getFriend(), people[i]);
				hexastore.insert(people[i], getFriend(), people[nextIndex]);
			}

			std::vector<QueryChain> directedTriangles = findDirectedTriangles(hexastore); 

			// Check that the triangle was detected
			QueryChain expectedResult1(people[0], getFriend(), people[1], getFriend(), people[2], getFriend(), people[0]);
			QueryChain expectedResult2(people[0], getFriend(), people[2], getFriend(), people[1], getFriend(), people[0]);
			TS_ASSERT(cyclicEquivalent(expectedResult2, directedTriangles.front()));
			TS_ASSERT(cyclicEquivalent(expectedResult1, directedTriangles.back()));
	
			// No spurious triangles detected
			TS_ASSERT(directedTriangles.size() == 2);
		}


	private:

		std::vector<HexastoreDataType*> people;
		std::vector<HexastoreValueType> names;
};

			/*
			// Printing dataset
			   std::cout << "Dataset: " << std::endl;
			   for (auto data : people)
			   {
			   std::cout << *data << std::endl;

			   }
			   std::cout << "------------------------------" << std::endl;
			 */
			/*
				// Print vector of query chains
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
			 */


