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

		AddingAndRemoving() :
		names(readNameCSV("../data/names.csv")),
		people(names, 1000)
		{}

		// Override function which setups the tests
		void setUp()
		{
			// Read names from a csv file
			//names = readNameCSV("../data/names.csv");
	
			// Make a data set of 1000 people to draw from
			//people = DataSet(names, 1000);
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

			bool one_is_front = cyclicEquivalent(expectedResult1, directedTriangles.front()) || 
						cyclicEquivalent(expectedResult2, directedTriangles.front());

			bool one_is_back = cyclicEquivalent(expectedResult1, directedTriangles.back()) || 
						cyclicEquivalent(expectedResult2, directedTriangles.back());
			TS_ASSERT(one_is_front);
			TS_ASSERT(one_is_back);
	
			// No spurious triangles detected
			TS_ASSERT(directedTriangles.size() == 2);
		}

		void testNonDirectedTriangles(void)
		{
			Hexastore hexastore;

			for (int i = 0; i < people.size(); i++)
			{
				int nextIndex = (i + 1) % people.size();
				hexastore.insert(people[i], getFriend(), people[nextIndex]);
			}

			hexastore.insert(people[0], getFriend(), people[2]);

			auto nonDirectedTriangles = findNonDirectedTriangles(hexastore); 

			QueryResult expectedResult;
			expectedResult.push_back(QueryChain(people[0], getFriend(), people[1], getFriend(), people[2]));
			expectedResult.push_back(QueryChain(people[0], getFriend(), people[2]));
			TS_ASSERT(nonDirectedTriangles[0] == expectedResult);

			// No spurious triangles detected
			TS_ASSERT(nonDirectedTriangles.size() == 1);
		}

	private:

		std::vector<HexastoreValueType> names;

		DataSet people;
};
