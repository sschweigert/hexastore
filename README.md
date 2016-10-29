
# Overview

This code implements a hexastore datastructure, roughly as described in the following paper:

http://karras.rutgers.edu/hexastore.pdf

# Ongoing tasks

- The paper points out that the last table in the structure is duplicated and memory can be saved by only allocating it once. This has yet to be implemented.

## Files to look at:

include/hexastore/hexastore.h

include/hexastore/internals.h

include/hexastore/triangle_detection.h

src/triangle_detection.cpp

test/hexastore_test.h
