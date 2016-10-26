#ifndef _HEXASTORE_INTERNALS_H_
#define _HEXASTORE_INTERNALS_H_

#include <map>
#include <set>

struct InsertSpecific
{

	void operator()(std::set<HexastoreDataType*>& data, std::vector<QueryChain>& toAdd, HexastoreDataType* middle, HexastoreDataType* specificVal)
	{
		if (data.count(specificVal) == 1)
		{
			QueryChain newNode; 
			newNode.insert(middle);
			newNode.insert(specificVal);
			toAdd.push_back(newNode);
		}
	}

};

struct InsertAll
{

	void operator()(std::set<HexastoreDataType*>& data, std::vector<QueryChain>& toAdd, HexastoreDataType* root)
	{
		for (auto& bottom : data)
		{
			QueryChain newChain;
			newChain.insert(root);
			newChain.insert(bottom);
			toAdd.push_back(newChain);
		}
	}

};

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

		std::map<HexastoreDataType*, BottomNode> data;	

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

};

struct RootNode
{

	public:

		bool contains(HexastoreDataType* top, HexastoreDataType* middle, HexastoreDataType* bottom);

		void insert(HexastoreDataType* top, HexastoreDataType* middle, HexastoreDataType* bottom);

		bool remove(HexastoreDataType* top, HexastoreDataType* middle, HexastoreDataType* bottom);

		std::vector<QueryChain> getConnectedVertices(HexastoreDataType* top);


		std::vector<QueryChain> getConnections(HexastoreDataType* root, HexastoreDataType* bottom);


		std::map<HexastoreDataType*, MiddleNode> data;



};

#endif
