#ifndef _HEXASTORE_QUERY_CHAIN_H_
#define _HEXASTORE_QUERY_CHAIN_H_

#include <hexastore/fundamental_types.h>

#include <vector>
#include <iostream>
#include <algorithm>

// A directed list of records that is obtained as the result of a query.
// Each QueryChain represents a path through the nodes and edges of a graph
// that the hexastore models.
class QueryChain
{

	public:

		typedef std::vector<HexastoreDataType*> Collection;

		typedef Collection::const_iterator const_iterator;

		QueryChain()
		{}

		// Build a query chain with some initial values.
		template <class Val, class ...Args>
			QueryChain(Val value, Args... args)
			{
				insert(value, args...);
			}

		// Append another QueryChain onto this one.
		void extend(const QueryChain& toAdd)
		{
			data.insert(data.end(), toAdd.begin(), toAdd.end());	
		}

		// Append multiple QueryChain objects onto this one.
		template <class ...Args>
			void extend(const QueryChain& toAdd, Args... args)
			{
				extend(toAdd);
				extend(args...);
			}

		// Insert a new entry onto the end of the QueryChain
		void insert(HexastoreDataType* entry)
		{
			data.push_back(entry);
		}

		// Insert multiple entries onto the end of the QueryChain
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

		// Tests for equality between two QueryChains.
		// Note: this will only return true if the elements are exactly the same.
		// cyclicEquivalent(...) should be used if you want to compare two QueryChain
		// that are both cyclic, but with a different ordering in their representation.
		bool operator==(const QueryChain& other) const
		{
			return data == other.data;
		}

	public:

		Collection data;	

};

// A collection of QueryChain that is actually returned for queries.
// The collection is used instead of the single QueryChain because
// a query may need to return multiple non-connected paths through the graph.
typedef std::vector<QueryChain> QueryResult;

// Check if a QueryChain is cyclic (the end is same as start)
bool cyclic(const QueryChain& chain);

// Check if two cyclical QueryChains are equivalent. For example, 0 -> 1 -> 0 is equivalent to 1 -> 0 -> 1
// Currently this is used in unit testing to check the results of queries.
bool cyclicEquivalent(const QueryChain& first, const QueryChain& other);

// A helper function for cyclicEquivalent. 
// It increments all the iterators in the vector. Then, it checks to see if any of 
// them don't equal the value pointed to by localItr, and if so removes them.
// Finally, if any of the iterators in the vector reaches its end, the function will return
// true to signal that, meaning that one of the iterators has matched the pattern.
bool checkIterators(std::vector<QueryChain::const_iterator>& otherIterators, QueryChain::const_iterator localItr, const QueryChain& other);

#endif
