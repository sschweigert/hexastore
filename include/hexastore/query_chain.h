#ifndef _HEXASTORE_QUERY_CHAIN_H_
#define _HEXASTORE_QUERY_CHAIN_H_

#include <hexastore/fundamental_types.h>

#include <vector>
#include <iostream>
#include <algorithm>

// A directed list of records
class QueryChain
{

	public:

		typedef std::vector<HexastoreDataType*> Collection;

		typedef Collection::const_iterator const_iterator;

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

		const_iterator begin() const
		{
			return data.begin();
		}

		const_iterator end() const
		{
			return data.end();
		}

		Collection::const_reference back() const
		{
			return data.back();	
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

		bool operator==(const QueryChain& other) const
		{
			return data == other.data;
		}


	public:

		Collection data;	

};

typedef std::vector<QueryChain> QueryResult;

// Check if a QueryChain is cyclic (end is same as start)
bool cyclic(const QueryChain& chain);

// Check if two cyclical QueryChains are equivalent. For example, 0 -> 1 -> 0 is equivalent to 1 -> 0 -> 1
// Currently this is used in unit testing to check the results
bool cyclicEquivalent(const QueryChain& first, const QueryChain& other);

// A helper function for cyclicEquivalent. 
bool checkIterators(std::vector<QueryChain::const_iterator>& otherIterators, QueryChain::const_iterator localItr, const QueryChain& other);

#endif
