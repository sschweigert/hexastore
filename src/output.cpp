#include <hexastore/output.h>

std::ostream& operator<<(std::ostream& os, const HexastoreDataType& dataType)
{
	os << *(dataType.value);
	return os;
}

std::ostream& operator<<(std::ostream& os, const QueryNode& query)
{
	const QueryNode* currentQuery = &query;

	do
	{
		os << *(currentQuery->record);

		currentQuery = currentQuery->next;
	
		if (currentQuery != NULL)
		{
			os << " -> ";
		}
		

	} while (currentQuery != NULL);
	
	return os;
}
