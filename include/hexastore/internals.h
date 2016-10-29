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

		typedef std::set<HexastoreDataType*> collection;

		typedef collection::const_iterator const_iterator;

		bool contains(HexastoreDataType* bottom) const;

		void insert(HexastoreDataType* bottom);

		bool remove(HexastoreDataType* bottom);

		template <class Algorithm>
			void insertConnections(std::vector<QueryChain>& toAdd, HexastoreDataType* middle, Algorithm& algorithm) const
			{
				algorithm(data, toAdd, middle);
			}

		const_iterator begin() const
		{
			return data.begin();
		}

		const_iterator end() const
		{
			return data.end();
		}

	private:

		collection data;


};

struct MiddleNode 
{

	public:

		typedef std::map<HexastoreDataType*, BottomNode> collection;
	
		typedef collection::const_iterator const_iterator;

		bool contains(HexastoreDataType* middle, HexastoreDataType* bottom) const;

		void insert(HexastoreDataType* middle, HexastoreDataType* bottom);

		bool remove(HexastoreDataType* middle, HexastoreDataType* bottom);

		template <class Algorithm>
			void insertConnections(std::vector<QueryChain>& toAdd, Algorithm& algorithm) const
			{
				for (auto& connection : data)
				{
					HexastoreDataType* edge = connection.first;
					const BottomNode& bottomNode = connection.second;
					bottomNode.insertConnections<Algorithm>(toAdd, edge, algorithm);
				}
			}

		const_iterator begin() const
		{
			return data.begin();
		}

		const_iterator end() const
		{
			return data.end();
		}

	private:

		collection data;	

};

struct RootNode
{

	public:

		typedef std::map<HexastoreDataType*, MiddleNode> collection;

		typedef collection::const_iterator const_iterator;

		bool contains(HexastoreDataType* top, HexastoreDataType* middle, HexastoreDataType* bottom) const;

		void insert(HexastoreDataType* top, HexastoreDataType* middle, HexastoreDataType* bottom);

		bool remove(HexastoreDataType* top, HexastoreDataType* middle, HexastoreDataType* bottom);

		template <class Algorithm, class ...Args>
			std::vector<QueryChain> getConnections(HexastoreDataType* root, Algorithm& algorithm) const
			{
				std::vector<QueryChain> toReturn;

				if (data.count(root) == 1)
				{
					const MiddleNode& middleNode = data.at(root);
					middleNode.insertConnections(toReturn, algorithm);
				}

				return toReturn;
			}

		const_iterator begin() const
		{
			return data.begin();
		}

		const_iterator end() const
		{
			return data.end();
		}


		collection data;



};

// Retrieves the QueryChains where the bottom node matches a given value.
struct InsertSpecific
{

	InsertSpecific(HexastoreDataType* specificVal) :
	specificVal(specificVal)
	{}

	void operator()(const std::set<HexastoreDataType*>& data, std::vector<QueryChain>& toAdd, HexastoreDataType* middle)
	{
		if (data.count(specificVal) == 1)
		{
			QueryChain newNode(middle, specificVal); 
			toAdd.push_back(newNode);
		}
	}

	HexastoreDataType* specificVal;

};

// Retrieves the QueryChains generated from all the connections to the bottom nodes.
struct InsertAll
{

	void operator()(const std::set<HexastoreDataType*>& data, std::vector<QueryChain>& toAdd, HexastoreDataType* middle)
	{
		for (auto& bottom : data)
		{
			QueryChain newChain(middle, bottom);
			toAdd.push_back(newChain);
		}
	}

};

#endif
