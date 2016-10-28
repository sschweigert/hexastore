#ifndef _HEXASTORE_QUERY_ITERATOR_H_
#define _HEXASTORE_QUERY_ITERATOR_H_

#include <hexastore/hexastore.h>

template <class Functor, class ...Args>
class QueryIterator
{

	public:

		QueryIterator(Functor& functor, Args... args)
	
		bool hasNext();

		QueryChain next();

	private:

		QueryIterator<Args...> 	


};

#endif
