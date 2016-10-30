#ifndef _HEXASTORE_SUBQUERY_ITERATOR_H_
#define _HEXASTORE_SUBQUERY_ITERATOR_H_

template <class Functor, class ...Args>
class SubQueryIterator
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

		const Hexastore& hexastore;

		const MiddleNode* middleNode;

		MiddleNode::const_iterator middleIterator;

		BottomNode::const_iterator bottomIterator;		

		Functor& functor;

		SubQueryIterator<Args...> subQueryIterator;	

};

template <class Functor, class ...Args>
		SubQueryIterator<Functor, Args...>::SubQueryIterator(const Hexastore& hexastore, const MiddleNode& root, Functor& functor, Args... args) :
			hexastore(hexastore),
			functor(functor),
			middleIterator(middleNode->begin()),
			bottomIterator(middleIterator->second.begin()),
			middleNode(&root),
			subQueryIterator(hexastore, hexastore.roots[spo].data.at(*bottomIterator), args...)
	{
	}

template <class Functor, class ...Args>
		bool SubQueryIterator<Functor, Args...>::reset(const MiddleNode& newMiddle)
		{
			// Setting the iterator to point to the start of the node iteration.
			middleNode = &newMiddle;
			middleIterator = middleNode->begin();
			bottomIterator = middleIterator->second.begin();

			if (hexastore.roots[spo].data.count(*bottomIterator) == 1)
			{
				return subQueryIterator.reset(hexastore.roots[spo].data.at(*bottomIterator));
			}
			else
			{
				return false;
			}
		}

template <class Functor, class ...Args>
		void SubQueryIterator<Functor, Args...>::recursiveChainBuilding(QueryChain& querySoFar)
		{
			querySoFar.insert(middleIterator->first, *bottomIterator);
			subQueryIterator.recursiveChainBuilding(querySoFar);
		}

template <class Functor, class ...Args>
		bool SubQueryIterator<Functor, Args...>::incrementSubQueryIterator(QueryChain& querySoFar)
		{
			querySoFar.insert(middleIterator->first, *bottomIterator);
			bool incrementSuccessful = subQueryIterator.increment(querySoFar);
			querySoFar.pop_back();
			querySoFar.pop_back();
			return incrementSuccessful;
		}

template <class Functor, class ...Args>
		bool SubQueryIterator<Functor, Args...>::increment(QueryChain& querySoFar)
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

template <class Functor, class ...Args>
		bool SubQueryIterator<Functor, Args...>::incrementNecessary(QueryChain& querySoFar)
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

template <class Functor, class ...Args>
		bool SubQueryIterator<Functor, Args...>::incrementIterators()
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
				return subQueryIterator.reset(hexastore.roots[spo].data.at(*bottomIterator));
			}
			else
			{
				return false;
			}
		}


template <class Functor>
class SubQueryIterator<Functor>
{

	public:

		SubQueryIterator(const Hexastore& hexastore, const MiddleNode& middleNode, Functor& functor);

		void recursiveChainBuilding(QueryChain& querySoFar);

		bool increment(QueryChain& querySoFar);

		bool incrementNecessary(QueryChain& querySoFar);

		bool reset(const MiddleNode& newMiddle);

	private:

		bool incrementIterators();

		const Hexastore& hexastore;

		const MiddleNode* middleNode;

		MiddleNode::const_iterator middleIterator;

		BottomNode::const_iterator bottomIterator;		

		Functor& functor;

};

template <class Functor>
		SubQueryIterator<Functor>::SubQueryIterator(const Hexastore& hexastore, const MiddleNode& middleNode, Functor& functor) :
			hexastore(hexastore),
			middleIterator(middleNode.begin()),
			bottomIterator(middleIterator->second.begin()),
			middleNode(&middleNode),
			functor(functor)
	{
		middleIterator = middleNode.begin();
		bottomIterator = middleIterator->second.begin();
	}

template <class Functor>
		void SubQueryIterator<Functor>::recursiveChainBuilding(QueryChain& querySoFar)
		{
			querySoFar.insert(middleIterator->first, *bottomIterator);
		}

template <class Functor>
		bool SubQueryIterator<Functor>::increment(QueryChain& querySoFar)
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

template <class Functor>
		bool SubQueryIterator<Functor>::incrementNecessary(QueryChain& querySoFar)
		{
			querySoFar.insert(middleIterator->first, *bottomIterator);


			bool matchesFunctor = Functor()(querySoFar);

			querySoFar.pop_back();
			querySoFar.pop_back();


			return (!matchesFunctor);
		}

template <class Functor>
		bool SubQueryIterator<Functor>::reset(const MiddleNode& newMiddle)
		{
			middleNode = &newMiddle;
			middleIterator = middleNode->begin();
			bottomIterator = middleIterator->second.begin();
			return true;
		}

template <class Functor>
		bool SubQueryIterator<Functor>::incrementIterators()
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

#endif
