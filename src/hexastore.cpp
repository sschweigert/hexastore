#include <hexastore/hexastore.h>

void BottomNode::insert(HexastoreDataType* bottom)
{
	data.insert(bottom);
}

void MiddleNode::insert(HexastoreDataType* middle, HexastoreDataType* bottom)
{
	(data[middle]).insert(bottom);
}

void RootNode::insert(HexastoreDataType* top, HexastoreDataType* middle, HexastoreDataType* bottom)
{
	(data[top]).insert(middle, bottom);
}

void Hexastore::insert(HexastoreDataType* subject, HexastoreDataType* predicate, HexastoreDataType* object)
{
	(roots[spo]).insert(subject, predicate, object);
	(roots[sop]).insert(subject, object, predicate);
	(roots[pso]).insert(predicate, subject, object);
	(roots[pos]).insert(predicate, object, subject);
	(roots[ops]).insert(object, predicate, subject);
	(roots[osp]).insert(object, subject, predicate);
}

bool Hexastore::contains(HexastoreDataType* subject, HexastoreDataType* predicate, HexastoreDataType* object) const
{
	return (roots[spo]).contains(subject, predicate, object);
}

bool RootNode::contains(HexastoreDataType* top, HexastoreDataType* middle, HexastoreDataType* bottom) const
{
	if (data.count(top) == 0)
	{
		return false;
	}
	else
	{
		return (data.at(top)).contains(middle, bottom);
	}
}

bool MiddleNode::contains(HexastoreDataType* middle, HexastoreDataType* bottom) const
{
	if (data.count(middle) == 0)
	{
		return false;
	}
	else
	{
		return (data.at(middle)).contains(bottom);
	}
}

bool BottomNode::contains(HexastoreDataType* bottom) const
{
	return (data.count(bottom) == 1);
}

bool BottomNode::remove(HexastoreDataType* bottom) 
{
	if (data.count(bottom) == 1)
	{
		data.erase(bottom);
	}
	return (data.size() == 1);
}

bool MiddleNode::remove(HexastoreDataType* middle, HexastoreDataType* bottom) 
{
	if (data.count(middle) == 1)
	{
		bool nodesLeft = (data[middle]).remove(bottom);
		if (!nodesLeft)
			data.erase(middle);

		return nodesLeft;
	}
	return (data.size() == 1);
}

bool RootNode::remove(HexastoreDataType* top, HexastoreDataType* middle, HexastoreDataType* bottom)
{
	if (data.count(top) == 1)
	{
		bool nodesLeft = (data[top]).remove(middle, bottom);
		if (!nodesLeft)
			data.erase(top);
	}	
}

void Hexastore::remove(HexastoreDataType* subject, HexastoreDataType* predicate, HexastoreDataType* object)
{
	(roots[spo]).remove(subject, predicate, object);
	(roots[sop]).remove(subject, object, predicate);
	(roots[pso]).remove(predicate, subject, object);
	(roots[pos]).remove(predicate, object, subject);
	(roots[ops]).remove(object, predicate, subject);
	(roots[osp]).remove(object, subject, predicate);
}

std::vector<QueryChain> Hexastore::getConnectedVertices(HexastoreDataType* top, RootType rootType) const
{
	InsertAll algorithm;
	return roots[rootType].getConnections(top, algorithm);
}

std::vector<QueryChain> Hexastore::getConnections(HexastoreDataType* root, HexastoreDataType* bottom, RootType rootType) const
{
	InsertSpecific algorithm(bottom);
	return roots[rootType].getConnections<InsertSpecific>(root, algorithm);
}
