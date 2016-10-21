#include <hexastore/output.h>

std::ostream& operator<<(std::ostream& os, const HexastoreDataType& dataType)
{
	os << (dataType.index);
	return os;
}

std::ostream& operator<<(std::ostream& os, const QueryNode& query)
{
	const QueryNode* currentQuery = &query;

	while (currentQuery != NULL)
	{
		os << *(currentQuery->record->value);

		currentQuery = currentQuery->next;
	
		if (currentQuery != NULL)
		{
			os << " -> ";
		}
		

	} 
	
	return os;
}

std::ostream& operator<<(std::ostream& os, const QueryChain& dataType)
{
	bool first = true;
	for (auto data : dataType)
	{
		if (!first) os << " -> ";

		os << *data;
		
		first = false;
	}
	return os;
}
