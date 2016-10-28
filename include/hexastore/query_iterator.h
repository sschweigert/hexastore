#ifndef _HEXASTORE_QUERY_ITERATOR_H_
#define _HEXASTORE_QUERY_ITERATOR_H_

#include <hexastore/internals.h>
#include <hexastore/hexastore.h>


template <class Functor, class ...Args>
class SubQueryIterator
{

	public:

		SubQueryIterator(Hexastore& hexastore, Functor& functor, Args... args) :
			hexastore(hexastore),
			functor(functor),
			subIterator(hexastore, args...)
	{
		rootIterator = hexastore.roots[spo].begin();
		middleIterator = rootIterator->second.begin();
		bottomIterator = middleIterator->second.begin();
	}

		bool hasNext()
		{
			return (rootIterator == hexastore.roots[spo].end());
		}

		void recursiveChainBuilding(QueryChain& querySoFar)
		{
			querySoFar.insert(*middleIterator, *bottomIterator);
			subIterator.recursiveChainBuilding(querySoFar);
		}

		bool increment(QueryChain& querySoFar)
		{
			if(!incrementIterators())
				return false;

			querySoFar.insert(middleIterator->first, *bottomIterator);
				while (!Functor()(querySoFar))
				{
					querySoFar.pop_back();
					querySoFar.pop_back();

					if(incrementIterators())
						return false;

					querySoFar.insert(middleIterator->first, *bottomIterator);
				}

			return subIterator.increment(querySoFar);
		}

	private:

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


		RootNode::const_iterator rootIterator;

		MiddleNode::const_iterator middleIterator;

		BottomNode::const_iterator bottomIterator;		

		Hexastore& hexastore;

		Functor& functor;

		SubQueryIterator<Args...> subIterator;	

};

template <class Functor>
class SubQueryIterator<Functor>
{

	public:

		SubQueryIterator(Hexastore& hexastore, Functor& functor) :
			hexastore(hexastore),
			functor(functor)
	{
		rootIterator = hexastore.roots[spo].begin();
		middleIterator = rootIterator->second.begin();
		bottomIterator = middleIterator->second.begin();
	}

		bool hasNext()
		{
			return (rootIterator == hexastore.roots[spo].end());
		}

		void recursiveChainBuilding(QueryChain& querySoFar)
		{
			querySoFar.insert(*middleIterator, *bottomIterator);
		}

		bool increment(QueryChain& querySoFar)
		{
			if(!incrementIterators())
				return false;

			querySoFar.insert(middleIterator->first, *bottomIterator);
				while (!Functor()(querySoFar))
				{
					querySoFar.pop_back();
					querySoFar.pop_back();

					if(incrementIterators())
						return false;

					querySoFar.insert(middleIterator->first, *bottomIterator);
				}

			return true;
		}

	private:

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

		RootNode::const_iterator rootIterator;

		MiddleNode::const_iterator middleIterator;

		BottomNode::const_iterator bottomIterator;		

		Hexastore& hexastore;

		Functor& functor;

};

template <class Functor, class ...Args>
class QueryIterator
{

	public:

		QueryIterator(Hexastore& hexastore, Functor& functor, Args... args) :
			subQueryIterator(hexastore, functor, args...)
	{
		increment();
	}


		// Returns the next element in the iteration.
		QueryChain next()
		{
			QueryChain toReturn;
			subQueryIterator.recursiveChainBuilding(toReturn);

			increment();

			return toReturn;
		}

		bool hasNext()
		{
			return subQueryIterator.hasNext();
		}

	private:

		void increment()
		{
			QueryChain querySoFar;
			subQueryIterator.increment(querySoFar);
		}

		SubQueryIterator<Functor, Args...> subQueryIterator;

};

#endif
