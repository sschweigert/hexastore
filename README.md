
# Overview

This code implements a hexastore datastructure, roughly as described in the following paper:

http://karras.rutgers.edu/hexastore.pdf

# Key Interface Files:

include/hexastore/hexastore.h

include/hexastore/query_iterator.h

include/hexastore/triangle_detection.h

# Example executables:

src/main.cpp 

src/main.cpp

# Unit-test file:

test/hexastore_test.h

# Ongoing tasks

- The paper points out that the last table in the structure is duplicated and memory can be saved by only allocating it once. This has yet to be implemented, although it is relatively trivial to do so.

- Currently QueryIterator only operates on the spo direction. This is relatively easy to change, it just needs to be parameterized in some way. 

- Documentation and refactoring ongoing
