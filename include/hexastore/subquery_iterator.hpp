#ifndef _HEXASTORE_SUBQUERY_ITERATOR_HPP_
#define _HEXASTORE_SUBQUERY_ITERATOR_HPP_

template <class Functor>
SubQueryIteratorBase<Functor>::SubQueryIteratorBase(const Hexastore& hexastore, const MiddleNode& middleNode, Functor& functor) :
	hexastore(hexastore),
	middleIterator(middleNode.begin()),
	bottomIterator(middleIterator->second.begin()),
	middleNodePtr(&middleNode),
	functor(functor)
{}


template <class Functor>
SubQueryIterator<Functor>::SubQueryIterator(const Hexastore& hexastore, const MiddleNode& middleNode, Functor& functor) :
SubQueryIteratorBase<Functor>(hexastore, middleNode, functor)
{
}

template <class Functor, class ...Args>
SubQueryIterator<Functor, Args...>::SubQueryIterator(const Hexastore& hexastore, const MiddleNode& middleNode, Functor& functor, Args... args) :
SubQueryIteratorBase<Functor>(hexastore, middleNode, functor),
	subQueryIterator(hexastore, hexastore.roots[spo].data.at(*SubQueryIteratorBase<Functor>::bottomIterator), args...)
{
}

	template <class Functor, class ...Args>
void SubQueryIterator<Functor, Args...>::recursiveChainBuilding(QueryChain& querySoFar)
{
	querySoFar.insert(SubQueryIteratorBase<Functor>::middleIterator->first, *SubQueryIteratorBase<Functor>::bottomIterator);
	subQueryIterator.recursiveChainBuilding(querySoFar);
}


	template <class Functor>
void SubQueryIterator<Functor>::recursiveChainBuilding(QueryChain& querySoFar)
{
	querySoFar.insert(SubQueryIteratorBase<Functor>::middleIterator->first, *SubQueryIteratorBase<Functor>::bottomIterator);
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
		// be pointing to a first element, but this element may not satisfy the SubQueryIteratorBase<Functor>::functors.
		if(!incrementIterators())
		{
			// The end of the available sub-iterators has been reached, so 
			// increment has surely failed.
			return false;
		}

		// Must check that the new iterator points to an element that satisfies the SubQueryIteratorBase<Functor>::functors,
		// and if not then that sub-iterator must be incremented until it does.
	} while (incrementNecessary(querySoFar));

	return true;
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

	template <class Functor, class ...Args>
bool SubQueryIterator<Functor, Args...>::incrementNecessary(QueryChain& querySoFar)
{
	querySoFar.insert(SubQueryIteratorBase<Functor>::middleIterator->first, *SubQueryIteratorBase<Functor>::bottomIterator);
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

	template <class Functor>
bool SubQueryIterator<Functor>::incrementNecessary(QueryChain& querySoFar)
{
	querySoFar.insert(SubQueryIteratorBase<Functor>::middleIterator->first, *SubQueryIteratorBase<Functor>::bottomIterator);


	bool matchesFunctor = Functor()(querySoFar);

	querySoFar.pop_back();
	querySoFar.pop_back();


	return (!matchesFunctor);
}


	template <class Functor, class ...Args>
bool SubQueryIterator<Functor, Args...>::reset(const MiddleNode& newMiddle)
{
	// Setting the iterator to point to the start of the node iteration.
	SubQueryIteratorBase<Functor>::middleNodePtr = &newMiddle;
	SubQueryIteratorBase<Functor>::middleIterator = SubQueryIteratorBase<Functor>::middleNodePtr->begin();
	SubQueryIteratorBase<Functor>::bottomIterator = SubQueryIteratorBase<Functor>::middleIterator->second.begin();

	if (SubQueryIteratorBase<Functor>::hexastore.roots[spo].data.count(*SubQueryIteratorBase<Functor>::bottomIterator) == 1)
	{
		return subQueryIterator.reset(SubQueryIteratorBase<Functor>::hexastore.roots[spo].data.at(*SubQueryIteratorBase<Functor>::bottomIterator));
	}
	else
	{
		return false;
	}
}


	template <class Functor>
bool SubQueryIterator<Functor>::reset(const MiddleNode& newMiddle)
{
	SubQueryIteratorBase<Functor>::middleNodePtr = &newMiddle;
	SubQueryIteratorBase<Functor>::middleIterator = SubQueryIteratorBase<Functor>::middleNodePtr->begin();
	SubQueryIteratorBase<Functor>::bottomIterator = SubQueryIteratorBase<Functor>::middleIterator->second.begin();
	return true;
}

	template <class Functor>
bool SubQueryIterator<Functor>::incrementIterators()
{
	++SubQueryIteratorBase<Functor>::bottomIterator;

	if (SubQueryIteratorBase<Functor>::bottomIterator == SubQueryIteratorBase<Functor>::middleIterator->second.end())
	{
		++SubQueryIteratorBase<Functor>::middleIterator;

		if (SubQueryIteratorBase<Functor>::middleIterator == SubQueryIteratorBase<Functor>::middleNodePtr->end())
		{
			return false;
		}
		SubQueryIteratorBase<Functor>::bottomIterator = SubQueryIteratorBase<Functor>::middleIterator->second.begin();
	}
	return true;
}

	template <class Functor, class ...Args>
bool SubQueryIterator<Functor, Args...>::incrementIterators()
{

	++SubQueryIteratorBase<Functor>::bottomIterator;

	if (SubQueryIteratorBase<Functor>::bottomIterator == SubQueryIteratorBase<Functor>::middleIterator->second.end())
	{
		++SubQueryIteratorBase<Functor>::middleIterator;

		if (SubQueryIteratorBase<Functor>::middleIterator == SubQueryIteratorBase<Functor>::middleNodePtr->end())
		{
			return false;
		}
		SubQueryIteratorBase<Functor>::bottomIterator = SubQueryIteratorBase<Functor>::middleIterator->second.begin();
	}
	if (SubQueryIteratorBase<Functor>::hexastore.roots[spo].data.count(*SubQueryIteratorBase<Functor>::bottomIterator) == 1)
	{
		return subQueryIterator.reset(SubQueryIteratorBase<Functor>::hexastore.roots[spo].data.at(*SubQueryIteratorBase<Functor>::bottomIterator));
	}
	else
	{
		return false;
	}
}

	template <class Functor, class ...Args>
bool SubQueryIterator<Functor, Args...>::incrementSubQueryIterator(QueryChain& querySoFar)
{
	querySoFar.insert(SubQueryIteratorBase<Functor>::middleIterator->first, *SubQueryIteratorBase<Functor>::bottomIterator);
	bool incrementSuccessful = subQueryIterator.increment(querySoFar);
	querySoFar.pop_back();
	querySoFar.pop_back();
	return incrementSuccessful;
}

#endif
