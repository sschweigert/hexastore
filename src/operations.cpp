#include <hexastore/operations.h>

void insert_to_bottom(BottomNode* bottomNode, HexastoreDataType* bottom)
{
	bottomNode->data.insert(bottom);
}

void insert_to_middle(MiddleNode* middleNode, HexastoreDataType* middle, HexastoreDataType* bottom)
{
	insert_to_bottom(&(middleNode->data[middle]), bottom);
}

void insert_to_root(RootNode* rootNode, HexastoreDataType* top, HexastoreDataType* middle, HexastoreDataType* bottom)
{
	insert_to_middle(&(rootNode->data[top]), middle, bottom);
}

void insert(Hexastore* hexastore, HexastoreDataType* subject, HexastoreDataType* predicate, HexastoreDataType* object)
{
	insert_to_root(&(hexastore->roots[spo]), subject, predicate, object);
	insert_to_root(&(hexastore->roots[sop]), subject, object, predicate);
	insert_to_root(&(hexastore->roots[pso]), predicate, subject, object);
	insert_to_root(&(hexastore->roots[pos]), predicate, object, subject);
	insert_to_root(&(hexastore->roots[ops]), object, predicate, subject);
	insert_to_root(&(hexastore->roots[osp]), object, subject, predicate);
}

bool contains(Hexastore* hexastore, HexastoreDataType* subject, HexastoreDataType* predicate, HexastoreDataType* object)
{
	return contains_root(&(hexastore->roots[spo]), subject, predicate, object);
}

bool contains_root(RootNode* rootNode, HexastoreDataType* top, HexastoreDataType* middle, HexastoreDataType* bottom)
{
	if (rootNode->data.count(top) == 0)
	{
		return false;
	}
	else
	{
		return contains_middle(&(rootNode->data[top]), middle, bottom);
	}
}

bool contains_middle(MiddleNode* middleNode, HexastoreDataType* middle, HexastoreDataType* bottom)
{
	if (middleNode->data.count(middle) == 0)
	{
		return false;
	}
	else
	{
		return contains_bottom(&(middleNode->data[middle]), bottom);
	}
}

bool contains_bottom(BottomNode* bottomNode, HexastoreDataType* bottom)
{
	return (bottomNode->data.count(bottom) == 1);
}

bool remove_from_bottom(BottomNode* bottomNode, HexastoreDataType* bottom)
{
	if (bottomNode->data.count(bottom) == 1)
	{
		bottomNode->data.erase(bottom);
	}
	return (bottomNode->data.size() == 1);
}

bool remove_from_middle(MiddleNode* middleNode, HexastoreDataType* middle, HexastoreDataType* bottom)
{
	if (middleNode->data.count(middle) == 1)
	{
		bool nodesLeft = remove_from_bottom(&(middleNode->data[middle]), bottom);
		if (!nodesLeft)
			middleNode->data.erase(middle);

		return nodesLeft;
	}
	return (middleNode->data.size() == 1);
}

bool remove_from_root(RootNode* rootNode, HexastoreDataType* top, HexastoreDataType* middle, HexastoreDataType* bottom)
{
	if (rootNode->data.count(top) == 1)
	{
		bool nodesLeft = remove_from_middle(&(rootNode->data[top]), middle, bottom);
		if (!nodesLeft)
			rootNode->data.erase(top);
	}	
}

void remove(Hexastore* hexastore, HexastoreDataType* subject, HexastoreDataType* predicate, HexastoreDataType* object)
{
	remove_from_root(&(hexastore->roots[spo]), subject, predicate, object);
	remove_from_root(&(hexastore->roots[sop]), subject, object, predicate);
	remove_from_root(&(hexastore->roots[pso]), predicate, subject, object);
	remove_from_root(&(hexastore->roots[pos]), predicate, object, subject);
	remove_from_root(&(hexastore->roots[ops]), object, predicate, subject);
	remove_from_root(&(hexastore->roots[osp]), object, subject, predicate);
}
