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

		void reset(const MiddleNode& newMiddle)
		{
			middleNode = &newMiddle;
			middleIterator = middleNode->begin();
			bottomIterator = middleIterator->second.begin();
			subQueryIterator.reset(hexastore.roots[spo].data[*bottomIterator]);
		}

		void recursiveChainBuilding(QueryChain& querySoFar)
		{
			querySoFar.insert(middleIterator->first, *bottomIterator);
			subQueryIterator.recursiveChainBuilding(querySoFar);
		}

		bool increment(QueryChain& querySoFar)
		{
			do
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

				subQueryIterator.reset(hexastore.roots[spo].data[*bottomIterator]);
			} while (incrementNecessary(querySoFar) && !subQueryIterator.increment(querySoFar));

			return true;
		}

		bool incrementNecessary(QueryChain& querySoFar)
		{
			querySoFar.insert(middleIterator->first, *bottomIterator);
			if (!Functor()(querySoFar))
				return true;
			else
				return subQueryIterator.incrementNecessary(querySoFar);
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
			if(!incrementIterators())
			{
				return false;
			}
	

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

		bool incrementNecessary(QueryChain& querySoFar)
		{
			querySoFar.insert(middleIterator->first, *bottomIterator);
			return (!Functor()(querySoFar));
		}

		void reset(const MiddleNode& newMiddle)
		{
			middleNode = &newMiddle;
			middleIterator = middleNode->begin();
			bottomIterator = middleIterator->second.begin();
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
					subQueryIterator.reset(rootIterator->second);
					if (!incrementNecessary())
						break;
				}
			}
		}

		Hexastore& hexastore;

		RootNode::const_iterator rootIterator;

		SubQueryIterator<Functor, Args...> subQueryIterator;


};

#endif
