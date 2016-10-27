
# Overview

This code implements a hexastore datastructure, roughly as described in the following paper:

http://karras.rutgers.edu/hexastore.pdf

# Ongoing tasks

- Queries are currently implemented for a proof of concept. The current implimentation requires a lot of copies into and out of vectors while searching through the nodes. For example, in triangle detection all vertices connected to a vertex are queried and copied into a vector. The process is then repeated for each element in that vector, and so on. In the intermediate stages there will be a large number of copies. I plan to change this design so that a templated iterator is used instead of copies. The iterator would skip elements depending on its template parameters (and/or member variables). This way the iterator could be used to directly retrieve the results of a query instead of having to copy them into a vector first.

- The paper points out that the last table in the structure is duplicated and memory can be saved by only allocating it once. This has yet to be implemented.

## Files to look at:

include/hexastore/hexastore.h

include/hexastore/internals.h

include/hexastore/triangle_detection.h

src/triangle_detection.cpp

test/hexastore_test.h
