#ifndef _HEXASTORE_QUERY_ITERATOR_H_
#define _HEXASTORE_QUERY_ITERATOR_H_

#include <hexastore/internals.h>
#include <hexastore/hexastore.h>

template <class Functor, class ...Args>
class SubQueryIterator
{

	public:

		SubQueryIterator(Hexastore& hexastore, const MiddleNode& root, Functor& functor, Args... args) :
			hexastore(hexastore),
			functor(functor),
			middleIterator(middleNode->begin()),
			bottomIterator(middleIterator->second.begin()),
			middleNode(&root),
			subQueryIterator(hexastore, hexastore.roots[spo].data[*bottomIterator], args...)
	{
	}

		
		// Reset the iterator so that it points to a new MiddleNode for iterator.
		// This behaves the same as the constructor. The reason I did this instead of reusing the constructor
		// is that there is no need to reset the hexastore and functor references.
		bool reset(const MiddleNode& newMiddle)
		{
			// Setting the iterator to point to the start of the node iteration.
			middleNode = &newMiddle;
			middleIterator = middleNode->begin();
			bottomIterator = middleIterator->second.begin();

			if (hexastore.roots[spo].data.count(*bottomIterator) == 1)
			{
				return subQueryIterator.reset(hexastore.roots[spo].data[*bottomIterator]);
			}
			else
			{
				return false;
			}
		}

		void recursiveChainBuilding(QueryChain& querySoFar)
		{
			querySoFar.insert(middleIterator->first, *bottomIterator);
			subQueryIterator.recursiveChainBuilding(querySoFar);
		}

		bool incrementSubQueryIterator(QueryChain& querySoFar)
		{
			querySoFar.insert(middleIterator->first, *bottomIterator);
			bool incrementSuccessful = subQueryIterator.increment(querySoFar);
			querySoFar.pop_back();
			querySoFar.pop_back();
			return incrementSuccessful;
		}

		bool increment(QueryChain& querySoFar)
		{
			do
			{
				// Increment the sub-iterator until it can no longer be incremented
				// because there are no more elements, or a valid state was found.
				while (incrementSubQueryIterator(querySoFar))
				{
					if (!incrementNecessary(querySoFar))
					{
						// Valid state was found, so the increment was successful.
						return true;
					}
				}

				// The sub-iterator had no more valid states, so adjust the sub-iterator
				// to a new root that can be searched. The new sub-iterator will automatically
				// be pointing to a first element, but this element may not satisfy the functors.
				if(!incrementIterators())
				{
					// The end of the available sub-iterators has been reached, so 
					// increment has surely failed.
					return false;
				}

			// Must check that the new iterator points to an element that satisfies the functors,
			// and if not then that sub-iterator must be incremented until it does.
			} while (incrementNecessary(querySoFar));

			return true;
		}

		bool incrementNecessary(QueryChain& querySoFar)
		{
			querySoFar.insert(middleIterator->first, *bottomIterator);
			bool matchesFunctor = Functor()(querySoFar);

			if (!matchesFunctor)
			{
				querySoFar.pop_back();
				querySoFar.pop_back();
				return true;
			}
			else
			{
				bool subQueryResult = subQueryIterator.incrementNecessary(querySoFar);
				querySoFar.pop_back();
				querySoFar.pop_back();
				return subQueryResult;
			}
		}

	private:

		bool incrementIterators()
		{
	
			++bottomIterator;

			if (bottomIterator == middleIterator->second.end())
			{
				++middleIterator;

				if (middleIterator == middleNode->end())
				{
					return false;
				}
				bottomIterator = middleIterator->second.begin();
			}
			if (hexastore.roots[spo].data.count(*bottomIterator) == 1)
			{
				return subQueryIterator.reset(hexastore.roots[spo].data[*bottomIterator]);
			}
			else
			{
				return false;
			}
		}

		Hexastore& hexastore;

		const MiddleNode* middleNode;

		MiddleNode::const_iterator middleIterator;

		BottomNode::const_iterator bottomIterator;		

		Functor& functor;

		SubQueryIterator<Args...> subQueryIterator;	

};

template <class Functor>
class SubQueryIterator<Functor>
{

	public:

		SubQueryIterator(Hexastore& hexastore, const MiddleNode& middleNode, Functor& functor) :
			hexastore(hexastore),
			middleIterator(middleNode.begin()),
			bottomIterator(middleIterator->second.begin()),
			middleNode(&middleNode),
			functor(functor)
	{
		middleIterator = middleNode.begin();
		bottomIterator = middleIterator->second.begin();
	}

		void recursiveChainBuilding(QueryChain& querySoFar)
		{
			querySoFar.insert(middleIterator->first, *bottomIterator);
		}

		bool increment(QueryChain& querySoFar)
		{
			do
			{
				if(!incrementIterators())
				{
					return false;
				}

			}
			while (incrementNecessary(querySoFar));

			return true;
		}

		bool incrementNecessary(QueryChain& querySoFar)
		{
			querySoFar.insert(middleIterator->first, *bottomIterator);


			bool matchesFunctor = Functor()(querySoFar);

			querySoFar.pop_back();
			querySoFar.pop_back();


			return (!matchesFunctor);
		}

		bool reset(const MiddleNode& newMiddle)
		{
			middleNode = &newMiddle;
			middleIterator = middleNode->begin();
			bottomIterator = middleIterator->second.begin();
			return true;
		}

	private:

		bool incrementIterators()
		{
			++bottomIterator;

			if (bottomIterator == middleIterator->second.end())
			{
				++middleIterator;

				if (middleIterator == middleNode->end())
				{
					return false;
				}
				bottomIterator = middleIterator->second.begin();
			}
			return true;
		}

		Hexastore& hexastore;

		const MiddleNode* middleNode;

		MiddleNode::const_iterator middleIterator;

		BottomNode::const_iterator bottomIterator;		

		Functor& functor;

};

// An iterator which operates on a hexastore and increments according to
template <class Functor, class ...Args>
class QueryIterator
{

	public:

		QueryIterator(Hexastore& hexastore, Functor& functor, Args... args) :
			rootIterator(hexastore.roots[spo].begin()),
			subQueryIterator(hexastore, rootIterator->second, functor, args...),
			hexastore(hexastore)
	{
		// It's possible that the iterator points to an invalid entry at the start.
		// If that's the case, then it must be incremented before the client uses it.
		if (incrementNecessary())
			increment();
	}


		// Return the next element in the iteration and increments the iterator.
		QueryChain next()
		{
			QueryChain toReturn(rootIterator->first);
			subQueryIterator.recursiveChainBuilding(toReturn);
	
			// Increment the iterator so it points to the next element
			increment();

			return toReturn;
		}

		// If true, a value can be obtained from the iterator.
		bool hasNext()
		{
			return rootIterator != hexastore.roots[spo].end();
		}

	private:

		// Check if the current state of the iterator satisfies all of its functors.
		bool incrementNecessary()
		{
			QueryChain querySoFar;
			querySoFar.insert(rootIterator->first);
			return subQueryIterator.incrementNecessary(querySoFar);
		}

		// Increment the iterator to the next valid state. If there's no valid state the 
		// iterator will end up in a state where hasNext() evaluates to false.
		void increment()
		{
			bool validStateFound = false;
	
			while (rootIterator != hexastore.roots[spo].end() && !validStateFound)
			{
				
				QueryChain querySoFar(rootIterator->first);
				validStateFound = subQueryIterator.increment(querySoFar);
				if (!validStateFound)
				{
					++rootIterator;

					if (!hasNext())
						return;
	
					bool rootHasPath = subQueryIterator.reset(rootIterator->second);
					if (rootHasPath && !incrementNecessary())
					{
						return;
					}
				}
			}
		}

		Hexastore& hexastore;

		// Iterates over the root nodes in the hexastore and provides the first entry in 
		// the returned QueryChains.
		RootNode::const_iterator rootIterator;

		// A recursive structure in which each element generates the next two elements in
		// the returned QueryChains.
		SubQueryIterator<Functor, Args...> subQueryIterator;


};

#endif
