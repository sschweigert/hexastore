#ifndef _HEXASTORE_HEXASTORE_H_
#define _HEXASTORE_HEXASTORE_H_

#include <hexastore/datatypes.h>
#include <hexastore/internals.h>
#include <hexastore/query_chain.h>

#include <vector>

typedef enum { spo, sop, pso, pos, osp, ops } RootType;

// This only exists as a placeholder in the template. I specialize it to push the
// query being built onto the vector of query chains.
// It is meant to be the last item in the template list, and it finishes the chain.
struct Push 
{};


struct Hexastore
{

	public:

		void remove(HexastoreDataType* subject, HexastoreDataType* predicate, HexastoreDataType* object);

		void insert(HexastoreDataType* subject, HexastoreDataType* predicate, HexastoreDataType* object);

		bool contains(HexastoreDataType* subject, HexastoreDataType* predicate, HexastoreDataType* object);

		std::vector<QueryChain> getConnectedVertices(HexastoreDataType* top, RootType rootType);

		std::vector<QueryChain> getConnections(HexastoreDataType* root, HexastoreDataType* bottom, RootType rootType);

		template <class SearchStrategy, class ...Args>
			void runQuery(std::vector<QueryChain>& buildingChain, QueryChain& querySoFar, RootType connectionType);

		template <class ...Args>
			void runHexastoreQuery(std::vector<QueryChain>& buildingChain, RootType connectionType)
			{
				for (auto& triangleRoot : roots[connectionType].data)
				{
					HexastoreDataType* topNode = triangleRoot.first;

					QueryChain buildingQuery;
					buildingQuery.insert(topNode);

					runQuery<Args..., Push>(buildingChain, buildingQuery, connectionType);
				}
			}


	public:

		RootNode roots[6];	


};

	template <class SearchStrategy, class ...Args>
inline void Hexastore::runQuery(std::vector<QueryChain>& buildingChain, QueryChain& querySoFar, RootType connectionType)
{
	std::vector<QueryChain> leads = SearchStrategy::getLeads(*this, querySoFar, connectionType);

	for (QueryChain& lead : leads)
	{
		if (SearchStrategy::acceptLead(lead, querySoFar))
		{
			querySoFar.extend(lead);
			this->runQuery<Args...>(buildingChain, querySoFar, connectionType);
			querySoFar.pop_back();
			querySoFar.pop_back();
		}
	}
}

// Specialization of runQuery which pushes 
	template <>
inline void Hexastore::runQuery<Push>(std::vector<QueryChain>& buildingChain, QueryChain& querySoFar, RootType connectionType)
{
	buildingChain.push_back(querySoFar);
}


#endif
