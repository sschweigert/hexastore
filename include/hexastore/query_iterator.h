#ifndef _HEXASTORE_QUERY_ITERATOR_H_
#define _HEXASTORE_QUERY_ITERATOR_H_

#include <hexastore/internals.h>
#include <hexastore/hexastore.h>
#include <hexastore/output.h>

#include <iostream>

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

		
		bool reset(const MiddleNode& newMiddle)
		{
			middleNode = &newMiddle;
			middleIterator = middleNode->begin();
			bottomIterator = middleIterator->second.begin();

			//
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
				while (incrementSubQueryIterator(querySoFar))
				{
					if (!incrementNecessary(querySoFar))
					{
						return true;
					}
				}

				if(!incrementIterators())
				{
					return false;
				}

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

template <class Functor, class ...Args>
class QueryIterator
{

	public:

		QueryIterator(Hexastore& hexastore, Functor& functor, Args... args) :
			rootIterator(hexastore.roots[spo].begin()),
			subQueryIterator(hexastore, rootIterator->second, functor, args...),
			hexastore(hexastore)
	{
		if (incrementNecessary())
			increment();
	}


		// Returns the next element in the iteration.
		QueryChain next()
		{
			QueryChain toReturn(rootIterator->first);
			subQueryIterator.recursiveChainBuilding(toReturn);
	

			increment();

			return toReturn;
		}

		bool hasNext()
		{
			return rootIterator != hexastore.roots[spo].end();
		}

	private:

		bool incrementNecessary()
		{
			QueryChain querySoFar;
			querySoFar.insert(rootIterator->first);
			return subQueryIterator.incrementNecessary(querySoFar);
		}

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

		RootNode::const_iterator rootIterator;

		SubQueryIterator<Functor, Args...> subQueryIterator;


};

#endif
