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

bool Hexastore::contains(HexastoreDataType* subject, HexastoreDataType* predicate, HexastoreDataType* object)
{
	return (roots[spo]).contains(subject, predicate, object);
}

bool RootNode::contains(HexastoreDataType* top, HexastoreDataType* middle, HexastoreDataType* bottom)
{
	if (data.count(top) == 0)
	{
		return false;
	}
	else
	{
		return (data[top]).contains(middle, bottom);
	}
}

bool MiddleNode::contains(HexastoreDataType* middle, HexastoreDataType* bottom)
{
	if (data.count(middle) == 0)
	{
		return false;
	}
	else
	{
		return (data[middle]).contains(bottom);
	}
}

bool BottomNode::contains(HexastoreDataType* bottom)
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

std::vector<QueryNode*> Hexastore::getConnectedVertices(HexastoreDataType* top, RootType rootType)
{
	std::vector<QueryNode*> toReturn;

	if (roots[rootType].data.count(top) == 1)
	{
		MiddleNode& middleNode  = roots[rootType].data[top];

		for (auto& middleIteration : middleNode.data)
		{
			HexastoreDataType* middle = middleIteration.first;
			for (auto& bottom : middleIteration.second.data)
			{
				QueryNode* newQueryResult = new QueryNode(middle);
				newQueryResult->next = new QueryNode(bottom);	
				toReturn.push_back(newQueryResult);
			}
		}
	}

	return toReturn;
}

std::vector<QueryNode*> Hexastore::getConnections(HexastoreDataType* root, HexastoreDataType* bottom, RootType rootType)
{
	std::vector<QueryNode*> toReturn;
	
	if (roots[rootType].data.count(root) == 1)
	{
		MiddleNode& middleNode = roots[rootType].data[root];
		for (auto& connection : middleNode.data)
		{
			HexastoreDataType* edge = connection.first;
			BottomNode& bottomNode = connection.second;

			if (bottomNode.data.count(bottom) == 1)
			{
				QueryNode* newNode = new QueryNode(edge);
				newNode->next = new QueryNode(bottom);
				toReturn.push_back(newNode);
			}
		}
	}

	return toReturn;
}
