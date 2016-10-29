#ifndef _HEXASTORE_HEXASTORE_H_
#define _HEXASTORE_HEXASTORE_H_

#include <hexastore/datatypes.h>
#include <hexastore/internals.h>
#include <hexastore/query_chain.h>

#include <vector>

typedef enum { spo, sop, pso, pos, osp, ops } RootType;

struct Hexastore
{

	public:

		// Insert a Subject-Predicate-Object triplet into the hexastore.
		void insert(HexastoreDataType* subject, HexastoreDataType* predicate, HexastoreDataType* object);

		// Check if a Subject-Predicate-Object triplet exists in the hexastore.
		bool contains(HexastoreDataType* subject, HexastoreDataType* predicate, HexastoreDataType* object) const;

		// Remove a Subhect-Predicate-Object triplet from the hexastore.
		void remove(HexastoreDataType* subject, HexastoreDataType* predicate, HexastoreDataType* object);

		// Obtain a vector containing all the connections to all the neighbor vertices of a chosen vertex.
		std::vector<QueryChain> getConnectedVertices(HexastoreDataType* top, RootType rootType) const;

		// Obtain a vector containing all the connections between two vertices. This must be a vector because
		// two vertices can connect with multiple different paths.
		std::vector<QueryChain> getConnections(HexastoreDataType* root, HexastoreDataType* bottom, RootType rootType) const;

		
		template <class ...Args>
			void runQuery(std::vector<QueryChain>& buildingChain, QueryChain& querySoFar, RootType connectionType) const;

		// Runs a query on all root nodes, one by one
		template <class ...Args>
			void runQueryOnAllRoots(std::vector<QueryChain>& buildingChain, RootType connectionType) const;

		RootNode roots[6];	

	private:

		template <class SearchStrategy, class ...Args>
			void runQueryHelper(std::vector<QueryChain>& buildingChain, QueryChain& querySoFar, RootType connectionType) const;

};

// Implementation
#include <hexastore/hexastore.hpp>

#endif
