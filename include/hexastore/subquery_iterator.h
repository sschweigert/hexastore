#ifndef _HEXASTORE_SUBQUERY_ITERATOR_H_
#define _HEXASTORE_SUBQUERY_ITERATOR_H_

template <class Functor>
class SubQueryIteratorBase 
{
	public:

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
		
		// Reset the iterator so that it points to a new MiddleNode for iterator.
		// This behaves the same as the constructor. The reason I did this instead of reusing the constructor
		// is that there is no need to reset the hexastore and functor references.
		bool reset(const MiddleNode& newMiddle);

		void recursiveChainBuilding(QueryChain& querySoFar);

		bool incrementSubQueryIterator(QueryChain& querySoFar);

		bool increment(QueryChain& querySoFar);

		bool incrementNecessary(QueryChain& querySoFar);

	private:

		bool incrementIterators();

		SubQueryIterator<Args...> subQueryIterator;	

};

template <class Functor>
class SubQueryIterator<Functor> : protected SubQueryIteratorBase<Functor>
{

	public:

		SubQueryIterator(const Hexastore& hexastore, const MiddleNode& middleNode, Functor& functor);

		void recursiveChainBuilding(QueryChain& querySoFar);

		bool increment(QueryChain& querySoFar);

		bool incrementNecessary(QueryChain& querySoFar);

		bool reset(const MiddleNode& newMiddle);

	private:

		bool incrementIterators();

};

// Implementation
#include <hexastore/subquery_iterator.hpp>


#endif
