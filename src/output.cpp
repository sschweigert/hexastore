#include <hexastore/output.h>

std::ostream& operator<<(std::ostream& os, const HexastoreDataType& dataType)
{
	os << *(dataType.value);
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
