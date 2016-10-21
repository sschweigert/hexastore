#ifndef _HEXASTORE_FUNAMENTAL_TYPES_H_
#define _HEXASTORE_FUNAMENTAL_TYPES_H_

#include <iostream>
#include <string>

// The concrete data associated with
typedef std::string HexastoreValueType;

// Defines structured entry in the hexastore
struct HexastoreDataType
{

	// I added this unique index because my triangle algorithm requires an explcit
	// ordering of the nodes in the store. This helps eliminate duplicate triangles
	// with rearranged ordering: 
	// [A -> B -> C -> A] vs 
	// [B -> C -> A -> B] vs 
	// [C -> A -> B -> C]  
	//
	// I am able to filter out these duplicates by only considering child nodes with
	// larger index than parents. For any given triangle there will be only one
	// such ordering in one direction.
	//
	// I initially considered using the HexastoreValueType pointer as this index, but 
	// I found that pointer comparison operators only work if the pointers are part of 
	// the same array or class, as per the C++ specification. Many compilers still support
	// comparison operators for all pointers, even those not in the same array, but I
	// wanted to be safe. The index is not needed if the HexastoreDataType
	// objects are all allocated in the same array before being loaded into the hexastore.
	int index;

	// Each entry in the hexastore contains a pointer to the data of the entry. This means
	// that multiple hexastore entries can point to the same data, with the hexastore
	// treating each one as a unique entry. 
	HexastoreValueType* value;

	HexastoreDataType(HexastoreValueType* value);

	static int indexCounter;

	bool operator<(const HexastoreDataType& other)
	{
		return index < other.index;
	}

	bool operator>(const HexastoreDataType& other)
	{
		return index > other.index;
	}
	
	bool operator<=(const HexastoreDataType& other)
	{
		return index <= other.index;	
	}

	bool operator>=(const HexastoreDataType& other)
	{
		return index >= other.index;	
	}

	bool operator==(const HexastoreDataType& other)
	{
		index == other.index;
	}

};

#endif
