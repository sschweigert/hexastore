#ifndef _HEXASTORE_QUERY_ITERATOR_FUNCTORS_H_
#define _HEXASTORE_QUERY_ITERATOR_FUNCTORS_H_

struct Descending
{
	bool operator()(QueryChain& querySoFar)
	{
		return querySoFar.back() < querySoFar.offsetBack(2);
	}
};

struct Ascending 
{
	bool operator()(QueryChain& querySoFar)
	{
		return querySoFar.back() > querySoFar.offsetBack(2);
	}
};

struct Return
{
	bool operator()(QueryChain& querySoFar)
	{
		return querySoFar.back() == querySoFar.front();
	}
};

#endif
