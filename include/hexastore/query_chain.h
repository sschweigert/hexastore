#ifndef _HEXASTORE_QUERY_CHAIN_H_
#define _HEXASTORE_QUERY_CHAIN_H_

#include <hexastore/fundamental_types.h>

#include <iostream>
#include <algorithm>

class QueryChain
{

	public:

		typedef std::vector<HexastoreDataType*> Collection;

		QueryChain()
		{}

		template <class Val, class ...Args>
		QueryChain(Val value, Args... args)
		{
			insert(value, args...);
		}
	
		void extend(const QueryChain& toAdd)
		{
			data.insert(data.end(), toAdd.begin(), toAdd.end());	
		}

		template <class ...Args>
		void extend(const QueryChain& toAdd, Args... args)
		{
			extend(toAdd);
			extend(args...);
		}

		void insert(HexastoreDataType* entry)
		{
			data.push_back(entry);
		}

		template <class ...Args>
		void insert(HexastoreDataType* entry, Args... args)
		{
			insert(entry);
			insert(args...);
		}

		Collection::const_iterator begin() const
		{
			return data.begin();
		}

		Collection::const_iterator end() const
		{
			return data.end();
		}

		Collection::const_reference back() const
		{
			return data.back();	
		}
		
		bool cyclic() const
		{
			return (data.front() == data.back());
		}

		Collection::size_type size() const
		{
			return data.size();
		}

		Collection::const_reference front() const
		{
			return data.front();
		}

		void pop_back()
		{
			data.pop_back();
		}

		bool cyclicEquivalent(const QueryChain& other)
		{
			// They are actually cycles
			if (!cyclic() || !other.cyclic())
				return false;

			// They are the same size
			if (size() != other.size())
				return false;

			std::vector<Collection::const_iterator> otherIterators;

			// Iterate up to the second last element. The last one will be a repeat of the first, 
			// as was verified previously
			for (auto localItr = begin(); localItr != (end() - 1); ++localItr)
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

				// Start up a new iterator 
				if (*localItr == other.front())
				{
					otherIterators.push_back(other.begin());
				}

			}	

			for (auto localItr = begin(); localItr != (end() - 1); ++localItr)
			{
				// Quit early if possible
				if (otherIterators.size() == 0)
		 		{
					break;
				}
				
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
			}
			return false;
		}

	private:

		Collection data;	

};

#endif
