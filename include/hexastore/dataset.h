#ifndef _HEXASTORE_DATASET_H_
#define _HEXASTORE_DATASET_H_

#include <hexastore/fundamental_types.h>

#include <vector>

// Defines a collection of records. Could contain a list of people in a database, for example.
// Each record in the set is uniquely defined by a HexastoreDataType object stored in the vector.
// The contents of the vector cannot be changed after construction, so there is no concern that
// the pointers will change due to the vector resizing.
// The operator[] returns pointers to the elements in the dataset. These pointers are used with
// the hexastore. The hexastore does not manage the memory for the data, it just manipulates pointers
// to the memory allocated elsewhere.
class DataSet 
{

	public:

		// \param nameData A vector of data to use to fill the data set. The value will
		// repeat if nameData.size() < length, but the records will still be unique.
		// \param length The number of records to generate in the dataset
		DataSet(std::vector<HexastoreValueType>& nameData, int length);

		// \brief The size of the dataset
		std::size_t size() const;

		// \brief Obtain a pointer to an element in the DataSet
		HexastoreDataType* operator[](std::size_t idx);

	private:

		// Underlying storage mechanism
		std::vector<HexastoreDataType> data;

};

#endif
