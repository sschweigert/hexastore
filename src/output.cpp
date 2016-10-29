#include <hexastore/output.h>

#include <hexastore/query_iterator.h>
#include <hexastore/query_iterator_functors.h>

std::ostream& operator<<(std::ostream& os, const HexastoreDataType& dataType)
{
	os << (dataType.index);
	return os;
}

std::ostream& operator<<(std::ostream& os, const QueryChain& dataType)
{
	bool first = true;
	for (auto data : dataType)
	{
		if (!first) os << " -> ";

		os << *data;
		
		first = false;
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, const Hexastore& hexastore)
{
	AcceptAll functor;
	QueryIterator<AcceptAll> queryIterator(hexastore, functor);	

	while (queryIterator.hasNext())
	{
		std::cout << queryIterator.next();

		if (queryIterator.hasNext())
			std::cout << std::endl;

	}
	return os;
}
