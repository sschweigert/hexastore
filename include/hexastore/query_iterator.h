#ifndef _HEXASTORE_QUERY_ITERATOR_H_
#define _HEXASTORE_QUERY_ITERATOR_H_

#include <hexastore/internals.h>
#include <hexastore/hexastore.h>
#include <hexastore/subquery_iterator.h>

// An iterator which operates on a hexastore and increments according to a list of Functor objects.
//
// This class maintains a chain of subQueryIterator objects each of which contributes two elements
// to the returned QueryChain object. Each subQueryIterator in the chain has its own Functor which controls
// how it is incremented.
//
// When the iterator is incremented (with calls to next()) this will increment the lowest subQueryIterator
// first. Then, the iterator will check to see if the functors along the chain return true. If not,
// it is incremented again. This continues until a valid chain is found, which is what is returned with
// through calls to next(). If a SubQueryIterator reaches the end of its available data, this will 
// propagate upwards in the chain before the state is tested using the Functors.
//
// You may wonder why I did not use the C++ idiom for iterators: (begin(), end()). I generally prefer to
// define my iterators using the begin() -> end() style, but in this case that would have made the design
// more difficult. It requires that I define a state representing the end() iterator which can be tested
// for equality so that the client can know when to stop iterating. In this class, that is somewhat difficult 
// to define due to the nature of the structure, so I chose more of a Java-style iterator interface.
template <class Functor, class ...Args>
class QueryIterator
{

	public:

		// Construct a QueryIterator operating on a hexastore, parameterized with a variadic
		// list of functors. The number of functors specified will dictate the length of the chain.
		// For example QueryIterator<AcceptAll, AcceptAll, AcceptAll> will return all the chains
		// of length 3 within the hexastore.
		QueryIterator(const Hexastore& hexastore, Functor& functor, Args... args);

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

		const Hexastore& hexastore;

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
