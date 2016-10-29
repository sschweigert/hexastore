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
			std::cout << "Middle::reset(MiddleNode&)" << std::endl;
			middleNode = &newMiddle;
			middleIterator = middleNode->begin();
			bottomIterator = middleIterator->second.begin();
			std::cout << "Middle: New bottom: " << (*bottomIterator)->index << std::endl;
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
			std::cout << "Middle::increment(QueryChain&)" << std::endl;
			std::cout << "Middle: querySoFar: " << querySoFar << std::endl;
			do
			{
				std::cout << "Middle: Incrementing subIterator" << std::endl;
				while (incrementSubQueryIterator(querySoFar))
				{
					std::cout << "Middle: SubIterator successfully incremented" << std::endl;
					if (!incrementNecessary(querySoFar))
					{
						std::cout << "Middle: Increment still necessary" << std::endl;
						return true;
					}
				}

				if(!incrementIterators())
				{
					std::cout << "Middle: incrementing iterators failed." << std::endl;
					return false;
				}
				std::cout << "Middle: incrementing iterators succeeded. New bottom:" << (*bottomIterator)->index << std::endl;

				std::cout << "Middle: Checking if increment still necessary:" << std::endl;
			} while (incrementNecessary(querySoFar));

			std::cout << "Middle: Increment not necessary. Returning true" << std::endl;

			return true;
		}

		bool incrementNecessary(QueryChain& querySoFar)
		{
			std::cout << "Middle::incrementNecessary(QueryChain&)" << std::endl;
			querySoFar.insert(middleIterator->first, *bottomIterator);
			std::cout << "Middle: Checking " << querySoFar << std::endl;
			bool matchesFunctor = Functor()(querySoFar);

			if (!matchesFunctor)
			{
				querySoFar.pop_back();
				querySoFar.pop_back();
				std::cout << "Middle: Didn't match, restoring: " << querySoFar << std::endl;
				return true;
			}
			else
			{
				std::cout << "Middle: Matches, propagating: " << querySoFar << std::endl;
				bool subQueryResult = subQueryIterator.incrementNecessary(querySoFar);
				querySoFar.pop_back();
				querySoFar.pop_back();
				return subQueryResult;
			}
		}

	private:

		bool incrementIterators()
		{
			std::cout << "Middle::incrementIterators()" << std::endl;
	
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
			std::cout << "Middle: New bottom: " << (*bottomIterator)->index << std::endl;
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
			std::cout << "Bottom::increment(QueryChain&)" << std::endl;
			do
			{
				if(!incrementIterators())
				{
					std::cout << "Bottom: Incrementing iterators failed" << std::endl;
					return false;
				}
				std::cout << "Bottom: was able to increment iterators. New bottom: " << (*bottomIterator)->index << std::endl;

			}
			while (incrementNecessary(querySoFar));

			return true;
		}

		bool incrementNecessary(QueryChain& querySoFar)
		{
			std::cout << "Bottom::incrementNecessary(QueryChain&)" << std::endl;
			querySoFar.insert(middleIterator->first, *bottomIterator);

			std::cout << "Bottom: Checking: " << querySoFar << std::endl;

			bool matchesFunctor = Functor()(querySoFar);

			querySoFar.pop_back();
			querySoFar.pop_back();

			std::cout << "Bottom: Matches: " << matchesFunctor << std::endl;
			std::cout << "Bottom: Restored to: " << querySoFar << std::endl;

			return (!matchesFunctor);
		}

		bool reset(const MiddleNode& newMiddle)
		{
			std::cout << "Bottom::reset(MiddleNode&)" << std::endl;
			middleNode = &newMiddle;
			middleIterator = middleNode->begin();
			bottomIterator = middleIterator->second.begin();
			std::cout << "Bottom: Resetting botom to: " << (*bottomIterator)->index << std::endl;
			return true;
		}

	private:

		bool incrementIterators()
		{
			std::cout << "Bottom: Incrementing bottom itr: " << (*bottomIterator)->index << std::endl;
			++bottomIterator;

			std::cout << "Bottom: Checking bottomIterator at end" << std::endl;
			if (bottomIterator == middleIterator->second.end())
			{
				++middleIterator;

				std::cout << "Bottom: Checking middleIterator at end" << std::endl;
				if (middleIterator == middleNode->end())
				{
					std::cout << "Bottom:  Unable to increment iterators" << std::endl;
					return false;
				}
				bottomIterator = middleIterator->second.begin();
			}
			std::cout << "Bottom:  Able to increment iterators" << std::endl;
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
	
			std::cout << "----------------------" << std::endl;
			std::cout << "Next result: " << toReturn << std::endl;

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
	
			std::cout << "QueryIterator.increment()" << std::endl;
			std::cout << "Root is currently: " << rootIterator->first->index << std::endl;
			while (rootIterator != hexastore.roots[spo].end() && !validStateFound)
			{
				
				QueryChain querySoFar(rootIterator->first);
				validStateFound = subQueryIterator.increment(querySoFar);
				std::cout << "validStateFound: " << validStateFound << std::endl;
				if (!validStateFound)
				{
					++rootIterator;
					if (!hasNext())
						return;
	
					std::cout << "No valid states found for root, incrementing. Is now: " << rootIterator->first->index << std::endl;
					bool rootHasPath = subQueryIterator.reset(rootIterator->second);
					if (rootHasPath && !incrementNecessary())
					{
						std::cout << "begin() state resulted in valid root. Increment finished." << std::endl;
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
