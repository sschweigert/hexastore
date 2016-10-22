#ifndef _HEXASTORE_QUERY_CHAIN_H_
#define _HEXASTORE_QUERY_CHAIN_H_

#include <hexastore/fundamental_types.h>

class QueryChain
{

	public:

		typedef std::vector<HexastoreDataType*> Collection;

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

	private:

		Collection data;	

};

#endif
