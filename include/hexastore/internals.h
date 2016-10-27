#ifndef _HEXASTORE_INTERNALS_H_
#define _HEXASTORE_INTERNALS_H_

#include <map>
#include <set>

// This file contains the internals of the hexastore. It contains classes defining the levels
// within the hexastore. Each level in the database contains a few basic functions, like
// insertion/removal and checking if elements exist in the object.

// Member functions within the Hexastore class will usually call functions on its RootNode
// objects. These function calls propagate from the RootNode to the MiddleNode and
// finally the BottomNode.

// Each class also contains a templated function used to query the data within the hexastore.
// This was originally two different functions, but I used templates to combine them
// into one generic function that takes in functors as template argument.


struct BottomNode 
{

	public:

		bool contains(HexastoreDataType* bottom);

		void insert(HexastoreDataType* bottom);

		bool remove(HexastoreDataType* bottom);

		template <class Algorithm, class ...Args>
			void insertConnections(std::vector<QueryChain>& toAdd, HexastoreDataType* middle, Args... args)
			{
				Algorithm()(data, toAdd, middle, args...);
			}

	private:

		std::set<HexastoreDataType*> data;


};

struct MiddleNode 
{

	public:

		bool contains(HexastoreDataType* middle, HexastoreDataType* bottom);

		void insert(HexastoreDataType* middle, HexastoreDataType* bottom);

		bool remove(HexastoreDataType* middle, HexastoreDataType* bottom);

		template <class Algorithm, class ...Args>
			void insertConnections(std::vector<QueryChain>& toAdd, Args... args)
			{
				for (auto& connection : data)
				{
					HexastoreDataType* edge = connection.first;
					BottomNode& bottomNode = connection.second;
					bottomNode.insertConnections<Algorithm>(toAdd, edge, args...);
				}
			}

	private:

		std::map<HexastoreDataType*, BottomNode> data;	

};

struct RootNode
{

	public:

		bool contains(HexastoreDataType* top, HexastoreDataType* middle, HexastoreDataType* bottom);

		void insert(HexastoreDataType* top, HexastoreDataType* middle, HexastoreDataType* bottom);

		bool remove(HexastoreDataType* top, HexastoreDataType* middle, HexastoreDataType* bottom);

		template <class Algorithm, class ...Args>
			std::vector<QueryChain> getConnections(HexastoreDataType* root, Args... args)
			{
				std::vector<QueryChain> toReturn;

				if (data.count(root) == 1)
				{
					MiddleNode& middleNode = data[root];
					middleNode.insertConnections<Algorithm>(toReturn, args...);
				}

				return toReturn;
			}


		std::map<HexastoreDataType*, MiddleNode> data;



};

// Retrieves the QueryChains where the bottom node matches a given value.
struct InsertSpecific
{

	void operator()(std::set<HexastoreDataType*>& data, std::vector<QueryChain>& toAdd, HexastoreDataType* middle, HexastoreDataType* specificVal)
	{
		if (data.count(specificVal) == 1)
		{
			QueryChain newNode(middle, specificVal); 
			toAdd.push_back(newNode);
		}
	}

};

// Retrieves the QueryChains generated from all the connections to the bottom nodes.
struct InsertAll
{

	void operator()(std::set<HexastoreDataType*>& data, std::vector<QueryChain>& toAdd, HexastoreDataType* middle)
	{
		for (auto& bottom : data)
		{
			QueryChain newChain(middle, bottom);
			toAdd.push_back(newChain);
		}
	}

};

#endif
