#include <hexastore/triangle_detection.h>

#include <hexastore/hexastore.h>


std::set<DirectedTriangle> findAllDirectedTriangles(Hexastore& hexastore)
{
	std::set<DirectedTriangle> toReturn;
	
	for (auto& rootIteration : toReturn.roots[spo])
	{
		HexastoreDataType* top = rootIteration.first;
		for (auto& middleIteration : rootIteration.second.data)
		{
			HexastoreDataType* middle = middleIteration.first;
			for (auto& bottom : middleIteration.second.data)
			{
				
			}
		}
	}

	return toReturn;
}
