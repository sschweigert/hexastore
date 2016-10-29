#ifndef _HEXASTORE_QUERY_ITERATOR_H_
#define _HEXASTORE_QUERY_ITERATOR_H_

#include <hexastore/internals.h>
#include <hexastore/hexastore.h>
#include <hexastore/subquery_iterator.h>

// An iterator which operates on a hexastore and increments according to
template <class Functor, class ...Args>
class QueryIterator
{

	public:

		QueryIterator(Hexastore& hexastore, Functor& functor, Args... args);

		// Return the next element in the iteration and increments the iterator.
		QueryChain next();

		// If true, a value can be obtained from the iterator.
		bool hasNext();

	private:

		// Check if the current state of the iterator satisfies all of its functors.
		bool incrementNecessary();

		// Increment the iterator to the next valid state. If there's no valid state the 
		// iterator will end up in a state where hasNext() evaluates to false.
		void increment();

		Hexastore& hexastore;

		// Iterates over the root nodes in the hexastore and provides the first entry in 
		// the returned QueryChains.
		RootNode::const_iterator rootIterator;

		// A recursive structure in which each element generates the next two elements in
		// the returned QueryChains.
		SubQueryIterator<Functor, Args...> subQueryIterator;


};

// Implementation
#include <hexastore/query_iterator.hpp>

#endif
