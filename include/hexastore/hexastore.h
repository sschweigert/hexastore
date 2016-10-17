#ifndef _HEXASTORE_HEXASTORE_H_
#define _HEXASTORE_HEXASTORE_H_

#include <map>
#include <set>

typedef std::string HexastoreDataType;

struct BottomNode 
{

	public:

		void insert(HexastoreDataType* bottom)
		{
			data.insert(bottom);
		}

		bool contains(HexastoreDataType* bottom)
		{
			return data.count(bottom);
		}

		bool remove(HexastoreDataType* bottom)
		{
			if (data.count(bottom) == 1)
			{
				data.erase(bottom);
			}
			return (data.size() == 1);
		}

		std::set<HexastoreDataType*> data;

};

struct MiddleNode 
{

		void insert(HexastoreDataType* middle, HexastoreDataType* bottom)
		{
			data[middle].insert(bottom);
		}

		bool contains(HexastoreDataType* middle, HexastoreDataType* bottom)
		{
			if (data.count(middle) == 0)
			{
				return false;
			}
			else
			{
				return data[middle].contains(bottom);
			}
		}

		bool remove(HexastoreDataType* middle, HexastoreDataType* bottom)
		{
			if (data.count(middle) == 1)
			{
				bool nodesLeft = data[middle].remove(bottom);
				if (!nodesLeft)
					data.erase(middle);

				return nodesLeft;
			}
			return (data.size() == 1);
		}

		std::set<HexastoreDataType*> allChildren()
		{
			std::set<HexastoreDataType*> toReturn;
			for (auto& kv : data)
			{
				
				toReturn.insert(kv.second.data.begin(), kv.second.data.end());
			}	
			return toReturn;	
		}

		std::map<HexastoreDataType*, BottomNode> data;	

};

struct RootNode
{

	public:

		void insert(HexastoreDataType* top, HexastoreDataType* middle, HexastoreDataType* bottom)
		{
			data[top].insert(middle, bottom);
		}

		bool contains(HexastoreDataType* top, HexastoreDataType* middle, HexastoreDataType* bottom)
		{
			if (data.count(top) == 0)
			{
				return false;
			}
			else
			{
				return data[top].contains(middle, bottom);
			}
		}

		void remove(HexastoreDataType* top, HexastoreDataType* middle, HexastoreDataType* bottom)
		{
			if (data.count(top) == 1)
			{
				bool nodesLeft = data[top].remove(middle, bottom);
				if (!nodesLeft)
					data.erase(top);
	 		}	
		}

		std::set<HexastoreDataType*> attachedBottoms(HexastoreDataType* key)
		{
			if (data.count(key) == 0)
				return std::set<HexastoreDataType*>();
			else
				return data[key].allChildren();
		}

	private:

		std::map<HexastoreDataType*, MiddleNode> data;

};

struct Triple
{

	Triple(HexastoreDataType subject, HexastoreDataType predicate, HexastoreDataType object) :
		subject(subject),
		predicate(predicate),
		object(object)
	{}

	Triple()
	{}

	HexastoreDataType subject;

	HexastoreDataType predicate;

	HexastoreDataType object;

};

struct Hexastore
{

	public:

		void insert(Triple& triple)
		{
			insert(&triple.subject, &triple.predicate, &triple.object);
		}

		bool contains(Triple& triple)
		{
			return contains(&triple.subject, &triple.predicate, &triple.object);
		}

		void remove(Triple& triple)
		{
			remove(&triple.subject, &triple.predicate, &triple.object);
		}

		std::set<HexastoreDataType*> objectsFromSubject(HexastoreDataType* subject)
		{
			return spo.attachedBottoms(subject);
		}

		std::set<HexastoreDataType*> subjectsFromObject(HexastoreDataType* object)
		{
			return ops.attachedBottoms(object);
		}

	private:
	
		bool contains(HexastoreDataType* subject, HexastoreDataType* predicate, HexastoreDataType* object)
		{
			return spo.contains(subject, predicate, object);
		}

		void insert(HexastoreDataType* subject, HexastoreDataType* predicate, HexastoreDataType* object)
		{
			spo.insert(subject, predicate, object);
			sop.insert(subject, object, predicate);

			pso.insert(predicate, subject, object);
			pos.insert(predicate, object, subject);

			ops.insert(object, predicate, subject);
			osp.insert(object, subject, predicate);
		}

		void remove(HexastoreDataType* subject, HexastoreDataType* predicate, HexastoreDataType* object)
		{
			spo.remove(subject, predicate, object);
			sop.remove(subject, object, predicate);

			pso.remove(predicate, subject, object);
			pos.remove(predicate, object, subject);

			osp.remove(object, subject, predicate);
			ops.remove(object, predicate, subject);
		}

		RootNode spo;	
		RootNode pso;	
		RootNode sop;	
		RootNode osp;	
		RootNode pos;	
		RootNode ops;	

};

#endif
