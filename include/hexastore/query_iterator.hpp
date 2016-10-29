#ifndef _HEXASTORE_QUERY_ITERATOR_HPP_
#define _HEXASTORE_QUERY_ITERATOR_HPP_

	template <class Functor, class ...Args>
		QueryIterator<Functor, Args...>::QueryIterator(Hexastore& hexastore, Functor& functor, Args... args) :
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
	template <class Functor, class ...Args>
QueryChain QueryIterator<Functor, Args...>::next()
{
	QueryChain toReturn(rootIterator->first);
	subQueryIterator.recursiveChainBuilding(toReturn);

	// Increment the iterator so it points to the next element
	increment();

	return toReturn;
}

	template <class Functor, class ...Args>
bool QueryIterator<Functor, Args...>::hasNext()
{
	return rootIterator != hexastore.roots[spo].end();
}

	template <class Functor, class ...Args>
bool QueryIterator<Functor, Args...>::incrementNecessary()
{
	QueryChain querySoFar;
	querySoFar.insert(rootIterator->first);
	return subQueryIterator.incrementNecessary(querySoFar);
}

	template <class Functor, class ...Args>
void QueryIterator<Functor, Args...>::increment()
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

#endif
