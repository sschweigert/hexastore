#ifndef _HEXASTORE_OPERATIONS_H_
#define _HEXASTORE_OPERATIONS_H_

#include <hexastore/hexastore.h>

void remove(Hexastore* hexastore, HexastoreDataType* subject, HexastoreDataType* predicate, HexastoreDataType* object);

void insert(Hexastore* hexastore, HexastoreDataType* subject, HexastoreDataType* predicate, HexastoreDataType* object);

bool contains(Hexastore* hexastore, HexastoreDataType* subject, HexastoreDataType* predicate, HexastoreDataType* object);


bool contains_root(RootNode* hexastore, HexastoreDataType* top, HexastoreDataType* middle, HexastoreDataType* bottom);

bool contains_middle(MiddleNode* hexastore, HexastoreDataType* middle, HexastoreDataType* bottom);

bool contains_bottom(BottomNode* hexastore, HexastoreDataType* bottom);


void insert_to_root(RootNode* rootNode, HexastoreDataType* top, HexastoreDataType* middle, HexastoreDataType* bottom);

void insert_to_middle(MiddleNode* middleNode, HexastoreDataType* middle, HexastoreDataType* bottom);

void insert_to_bottom(BottomNode* bottomNode, HexastoreDataType* bottom);


bool remove_from_root(RootNode* rootNode, HexastoreDataType* top, HexastoreDataType* middle, HexastoreDataType* bottom);

bool remove_from_middle(MiddleNode* middleNode, HexastoreDataType* middle, HexastoreDataType* bottom);

bool remove_from_bottom(BottomNode* bottomNode, HexastoreDataType* bottom);


#endif
