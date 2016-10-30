#ifndef _HEXASTORE_SUBQUERY_ITERATOR_H_
#define _HEXASTORE_SUBQUERY_ITERATOR_H_


// Base class to group the common elements of the two SubQueryIterator classes so they don't need to be written twice.
// More member functions will be moved in here during refactoring.
template <class Functor>
class SubQueryIteratorBase 
{
	protected:

		SubQueryIteratorBase(const Hexastore& hexastore, const MiddleNode& middleNode, Functor& functor);

		const Hexastore& hexastore;

		const MiddleNode* middleNodePtr;

		MiddleNode::const_iterator middleIterator;

		BottomNode::const_iterator bottomIterator;		

		Functor& functor;

};

template <class Functor, class ...Args>
class SubQueryIterator : protected SubQueryIteratorBase<Functor>
{

	public:

		SubQueryIterator(const Hexastore& hexastore, const MiddleNode& root, Functor& functor, Args... args);

		// The main increment function. This function should be passed a QueryChain containing the chain built
		// so far. The function is called recursively, meaning that child iterators will have the chain elements from
		// this parent.
		// \return True if a valid state was found, false if there are no more elements within the SubQueryIterator.
		bool increment(QueryChain& querySoFar);
		
		// Reset the iterator so that it points to a new MiddleNode for iterator.
		// This behaves the same as the constructor. The reason I did this instead of reusing the constructor
		// is that there is no need to reset the hexastore and functor references.
		bool reset(const MiddleNode& newMiddle);

		// Add this iterator's *middleIterator and *bottomIterator values to a passed parameter.
		// Then, do the same for the child iterator, thereby building the entire chain.
		void recursiveChainBuilding(QueryChain& querySoFar);

		// Check if an increment is necessary due to an invalid state. This checks the current functor, and then
		// the child functor, and so on. The QueryChain is passed in and it is built while the functors are checked.
		// There is likely a more efficient way of doing this whereby the entire QueryChain need not be built.
		// \return True if the iterator is in an invalid state
		bool incrementNecessary(QueryChain& querySoFar);

	private:

		bool incrementSubQueryIterator(QueryChain& querySoFar);

		// Increment the node iterators inside this SubQueryIterator and resets the child SubQueryIterator so that
		// to the new *bottomIterator value.
		// \return False if the iterators cannot be incremented because there's no more elements left.
		bool incrementIterators();

		SubQueryIterator<Args...> subQueryIterator;	

};

// Specialization for the terminus case 
template <class Functor>
class SubQueryIterator<Functor> : protected SubQueryIteratorBase<Functor>
{

	public:

		bool increment(QueryChain& querySoFar);

		SubQueryIterator(const Hexastore& hexastore, const MiddleNode& middleNode, Functor& functor);

		void recursiveChainBuilding(QueryChain& querySoFar);

		bool incrementNecessary(QueryChain& querySoFar);

		bool reset(const MiddleNode& newMiddle);

	private:

		bool incrementIterators();

};

// Implementation
#include <hexastore/subquery_iterator.hpp>


#endif
