#ifndef _HEXASTORE_QUERY_ITERATOR_H_
#define _HEXASTORE_QUERY_ITERATOR_H_

#include <hexastore/internals.h>
#include <hexastore/hexastore.h>

template <class Functor, class ...Args>
class QueryIterator
{

	public:

		QueryIterator(Hexastore& hexastore, Functor& functor, Args... args) :
		hexastore(hexastore),
		functor(functor),
		subIterator(hexastore, args...)
		{
			rootIterator = hexastore.roots[spo].begin();
			middleIterator = rootIterator->second.begin();
			bottomIterator = middleIterator->second.begin();

			increment();	
			
		}
	
		bool hasNext()
		{
			return (rootIterator == hexastore.roots[spo].end());
		}

		// Returns the next element in the iteration.
		QueryChain next()
		{
			QueryChain toReturn;
			recursiveChainBuilding(toReturn);

			increment();

			return toReturn;
		}

	private:

		void recursiveChainBuilding(QueryChain& querySoFar)
		{
			querySoFar.insert(*rootIterator, *middleIterator, *bottomIterator);
			subIterator.recursiveChainBuilding(querySoFar);
		}

		bool incrementIterators()
		{
			++bottomIterator;
		
			if (bottomIterator == middleIterator->second.end())
			{
				++middleIterator;

				if (middleIterator == rootIterator->second.end())
				{
					++rootIterator;
					if (rootIterator == hexastore.roots[spo].end())
					{
						return false;
					}	
					middleIterator = rootIterator->second.begin();
				}
				bottomIterator = middleIterator->second.begin();
			}
			return true;
		}

		bool increment(QueryChain& querySoFar)
		{
			if(!incrementIterators())
				return false;

			querySoFar.insert(middleIterator->first, *bottomIterator)
			while (!Functor(querySoFar))
			{
				querySoFar.pop_back();
				querySoFar.pop_back();

				if(incrementIterators())
					return false;

				querySoFar.insert(middleIterator->first, *bottomIterator)
			}
			
			return subIterator.increment(querySoFar);
		}

		void increment()
		{
			QueryChain querySoFar;
			increment(querySoFar);
		}

		RootNode::const_iterator rootIterator;
	
		MiddleNode::const_iterator middleIterator;
		
		BottomNode::const_iterator bottomIterator;		

		Hexastore& hexastore;
	
		Functor& functor;

		QueryIterator<Args...> subIterator;	

};

#endif
