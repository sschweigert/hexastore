#ifndef _HEXASTORE_QUERY_ITERATOR_FUNCTORS_H_
#define _HEXASTORE_QUERY_ITERATOR_FUNCTORS_H_

struct Descending
{
	bool operator()(const QueryChain& querySoFar)
	{
		return querySoFar.back() < querySoFar.offsetBack(2);
	}
};

struct Ascending 
{
	bool operator()(const QueryChain& querySoFar)
	{
		return querySoFar.back() > querySoFar.offsetBack(2);
	}
};

struct Return
{
	bool operator()(const QueryChain& querySoFar)
	{
		return querySoFar.back() == querySoFar.front();
	}
};

struct AcceptAll
{
	bool operator()(const QueryChain& querySoFar)
	{
		return true;
	}
};

#endif
