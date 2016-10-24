#include <hexastore/query_chain.h>

bool cyclic(const QueryChain& chain)
{
	return chain.front() == chain.back();
}

// This function implemenets a simple pattern matching algorithm
// A vector of iterators is maintained.
// The first QueryChain is scanned. If the element matches other.front(),
// then front.begin() is added to the vector of iterators.
// Each element in this vector of iterators is incremented in each loop
// if it matches the next value in the first QueryChain.
// If any iterator in the vector reaches the end of its chain, then it has
// matched the full pattern of the other chain.
bool cyclicEquivalent(const QueryChain& first, const QueryChain& other)
{
	// Check that they are actually cycles
	if (!cyclic(first) || !cyclic(other))
		return false;

	// Check they are the same size
	if (first.size() != other.size())
		return false;

	std::vector<QueryChain::const_iterator> otherIterators;

	// Iterate up to the second last element. The last one will be a repeat of the first, 
	// as was verified previously
	for (auto localItr = first.begin(); localItr != (first.end() - 1); ++localItr)
	{

		if (checkIterators(otherIterators, localItr, other))
		{
			return true;
		}

		// Start up a new iterator 
		if (*localItr == other.front())
		{
			otherIterators.push_back(other.begin());
		}

	}	

	for (auto localItr = first.begin(); localItr != (first.end() - 1); ++localItr)
	{
		// Quit early if possible
		if (otherIterators.size() == 0)
		{
			break;
		}

		if (checkIterators(otherIterators, localItr, other))
		{
			return true;
		}

	}
	return false;
}

bool checkIterators(std::vector<QueryChain::const_iterator>& otherIterators, QueryChain::const_iterator localItr, const QueryChain& other)
{
	for (auto otherIteratorsItr = otherIterators.begin(); otherIteratorsItr != otherIterators.end();)
	{
		auto& otherItr = *otherIteratorsItr;

		// Increment iterators
		++otherItr;

		if (otherItr == (other.end() - 1))
		{
			return true;
		}

		if (*localItr != *otherItr)
		{
			otherIteratorsItr = otherIterators.erase(otherIteratorsItr);
		}
		else
		{
			// This is here so that it is possible to remove elements while iterating
			++otherIteratorsItr;
		}
	}
	return false;
}
