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
		bool contains(HexastoreDataType* subject, HexastoreDataType* predicate, HexastoreDataType* object);

		// Remove a Subhect-Predicate-Object triplet from the hexastore.
		void remove(HexastoreDataType* subject, HexastoreDataType* predicate, HexastoreDataType* object);

		// Obtain a vector containing all the connections to all the neighbor vertices of a chosen vertex.
		std::vector<QueryChain> getConnectedVertices(HexastoreDataType* top, RootType rootType);

		// Obtain a vector containing all the connections between two vertices. This must be a vector because
		// two vertices can connect with multiple different paths.
		std::vector<QueryChain> getConnections(HexastoreDataType* root, HexastoreDataType* bottom, RootType rootType);

		
		template <class ...Args>
			void runQuery(std::vector<QueryChain>& buildingChain, QueryChain& querySoFar, RootType connectionType);

		// Runs a query on all root nodes, one by one
		template <class ...Args>
			void runQueryOnAllRoots(std::vector<QueryChain>& buildingChain, RootType connectionType)
			{
				for (auto& triangleRoot : roots[connectionType].data)
				{
					HexastoreDataType* topNode = triangleRoot.first;

					QueryChain buildingQuery;
					buildingQuery.insert(topNode);

					runQuery<Args...>(buildingChain, buildingQuery, connectionType);
				}
			}

		RootNode roots[6];	

	private:

		template <class SearchStrategy, class ...Args>
			void runQueryHelper(std::vector<QueryChain>& buildingChain, QueryChain& querySoFar, RootType connectionType);

};

// This only exists as a placeholder in the template. I specialize it to push the
// query being built onto the vector of query chains.
// It is meant to be the last item in the template list, and it finishes the chain.
struct Push 
{};


	template <class ...Args>
void Hexastore::runQuery(std::vector<QueryChain>& buildingChain, QueryChain& querySoFar, RootType connectionType)
{
	// Delegate to the helper function
	runQueryHelper<Args..., Push>(buildingChain, querySoFar, connectionType);
}

// Specialization of runQuery which pushes the value to the vector, thereby ending the recursion
	template <>
inline void Hexastore::runQueryHelper<Push>(std::vector<QueryChain>& buildingChain, QueryChain& querySoFar, RootType connectionType)
{
	buildingChain.push_back(querySoFar);
}


	template <class SearchStrategy, class ...Args>
inline void Hexastore::runQueryHelper(std::vector<QueryChain>& buildingChain, QueryChain& querySoFar, RootType connectionType)
{
	// This is a relatively inefficient way of doing this
	std::vector<QueryChain> leads = SearchStrategy::getLeads(*this, querySoFar, connectionType);

	for (QueryChain& lead : leads)
	{
		if (SearchStrategy::acceptLead(lead, querySoFar))
		{
			querySoFar.extend(lead);
			this->runQueryHelper<Args...>(buildingChain, querySoFar, connectionType);
			querySoFar.pop_back();
			querySoFar.pop_back();
		}
	}
}


#endif
