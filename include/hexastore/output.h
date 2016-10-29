#ifndef _HEXASTORE_OUTPUT_H_
#define _HEXASTORE_OUTPUT_H_

#include <iostream>

#include <hexastore/datatypes.h>
#include <hexastore/query_chain.h>
#include <hexastore/hexastore.h>

// This file just contains operator<< functions designed to make output easier 
// for debugging/testing purposes

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

std::ostream& operator<<(std::ostream& os, const Hexastore& hexastore);

#endif
