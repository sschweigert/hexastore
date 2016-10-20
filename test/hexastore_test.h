#include <cxxtest/TestSuite.h>

#include <vector>

#include <hexastore/hexastore.h>
#include <hexastore/datastore.h>
#include <hexastore/triangle_detection.h>

class AddingAndRemoving : public CxxTest::TestSuite
{

	public:

		AddingAndRemoving()
		{
			names = readNameCSV("../data/names.csv");
		}
	
		void testInsertionRemoval(void)
		{
			Hexastore hexastore;
			std::vector<HexastoreDataType*> people = createPersonDataset(names, 1000);

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

		void testDirectedTriangleDetection(void)
		{
			const int storeSize = 1000;
			Hexastore hexastore;
			std::vector<HexastoreDataType*> people = createPersonDataset(names, storeSize);
			
			for (int i = 0; i < 1000; i++)
			{
				int nextIndex1 = (i + 1) % 100;
				int nextIndex2 = (i + 2) % 100;
				hexastore.insert(people[i], people[nextIndex1], people[nextIndex2]);
			}

			hexastore.insert(people[2], people[1], people[0]);
			
			std::vector<QueryNode*> directedTriangles = findAllDirectedTriangles(hexastore);

			TS_ASSERT(directedTriangles.size() == 1);
		}

	private:

		std::vector<HexastoreValueType> names;
};
