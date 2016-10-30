#include <hexastore/output.h>


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

	os << queryIterator;
	return os;
}
