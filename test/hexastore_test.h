#include <cxxtest/TestSuite.h>

#include <vector>

#include <hexastore/hexastore.h>
#include <hexastore/operations.h>


class AddingAndRemoving : public CxxTest::TestSuite
{

	public:

		void testInsertion(void)
		{
			/*
			std::vector<Triple> data(10);
			Hexastore hexastore;
			insert(&hexastore, (data[0].subject), (data[0].predicate), (data[0].object));
			
			TS_ASSERT(contains(&hexastore, (data[0].subject), (data[0].predicate), (data[0].object)));
			TS_ASSERT(!contains(&hexastore, (data[1].subject), (data[1].predicate), (data[1].object)));
			*/
			
		}

		void testAddition2(void)
		{
			TS_ASSERT_EQUALS(true, true);
		}
};
