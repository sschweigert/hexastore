#ifndef _HEXASTORE_HEXASTORE_HPP_
#define _HEXASTORE_HEXASTORE_HPP_

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
	// This is a relatively inefficient way of doing this due to the large number
	// of copies that will occur with nested queries

	std::vector<QueryChain> leads = SearchStrategy::getLeads(*this, querySoFar, connectionType);

	for (QueryChain& lead : leads)
	{
		if (SearchStrategy::acceptLead(lead, querySoFar))
		{
			querySoFar.insert(lead);
			this->runQueryHelper<Args...>(buildingChain, querySoFar, connectionType);
			querySoFar.pop_back();
			querySoFar.pop_back();
		}
	}
}

#endif
