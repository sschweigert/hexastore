#ifndef _HEXASTORE_OUTPUT_H_
#define _HEXASTORE_OUTPUT_H_

#include <iostream>

#include <hexastore/datatypes.h>
#include <hexastore/query_chain.h>

std::ostream& operator<<(std::ostream& os, const HexastoreDataType& dataType);

template <typename DataType>
std::ostream& operator<<(std::ostream& os, const std::vector<DataType>& collection)
{
	// Flag for commas
	bool first = true;

	os << "[";
	for (auto& element : collection)
	{
		if (!first)
		{
			os << ", " << std::endl;
		}

		os << element;	

		first = false;
	}
	os << "]";

	return os;
}

template <typename DataType>
std::ostream& operator<<(std::ostream& os, const std::vector<DataType*>& collection)
{
	// Flag for commas
	bool first = true;

	os << "[";
	for (auto& element : collection)
	{
		if (!first)
		{
			os << ", " << std::endl;
		}

		os << *element;	

		first = false;
	}
	os << "]";

	return os;
}

std::ostream& operator<<(std::ostream& os, const QueryChain& dataType);


#endif
