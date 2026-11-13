##################################################################################
# EXTERNAL LIBRARY PATHS
##################################################################################

SRC  := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
INC_LIST = $(SRC) 
INC = $(foreach d, $(INC_LIST), -I$d)

##################################################################################
# GENERAL SETTINGS
##################################################################################
COMP=gnu

# Include global settings.
include Makefile.$(COMP)

##################################################################################
# OBJECT FILES
##################################################################################

graph.o: graph.cpp
	$(CXX_SERIAL) $(CXX_FLAGS) $(INC) -c graph.cpp

##################################################################################
# TARGETS
##################################################################################

clean: 
	rm *.o test_graph_unit samc

# ==== UNIT TESTS ====

test_graph_unit: graph.o
	$(CXX_SERIAL) $(CXX_FLAGS) $(INC) -o test_graph_unit test_graph_unit.cpp graph.o

# ==== SAMC ====

samc: graph.o
	$(CXX_SERIAL) $(CXX_FLAGS) $(INC) -o samc samc.cpp graph.o

