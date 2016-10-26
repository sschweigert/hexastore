#ifndef _HEXASTORE_GENERIC_QUERY_H_
#define _HEXASTORE_GENERIC_QUERY_H_

#include <hexastore/hexastore.h>

template <class SearchStrategy, class ...Args>
void runQuery(Hexastore& hexastore, std::vector<QueryChain>& buildingChain, QueryChain& querySoFar, RootType connectionType)
{
	std::vector<QueryChain> leads = SearchStrategy::getLeads(hexastore, querySoFar, connectionType);
	
	for (QueryChain& lead : leads)
	{
		if (SearchStrategy::acceptLead(lead, querySoFar))
		{
			querySoFar.extend(lead);
			runQuery<Args...>(hexastore, buildingChain, querySoFar, connectionType);
			querySoFar.pop_back();
			querySoFar.pop_back();
		}
	}
}


// Specialization of runQuery which pushes 
	template <>
inline void runQuery<Push>(Hexastore& hexastore, std::vector<QueryChain>& buildingChain, QueryChain& querySoFar, RootType connectionType)
{
	buildingChain.push_back(querySoFar);
}


#endif
