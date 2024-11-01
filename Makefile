# Compiler + flags
CXX 		  = g++
CXX_FLAGS = -std=c++11 -Wall -Iinclude

# Dirs
SRC_DIR     = src
INCLUDE_DIR = include
OBJ_DIR     = obj
OBJECTS     = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)%.o)  # What the fuck
BIN_DIR     = bin
TESTS_DIR   = tests
EXECUTABLE  = $(BIN_DIR)/qpsk_encoder

# Use object files to make program
$(PROG): $(OBJECTS)
	mkdir -p $(BIN_DIR)
	$(CXX) $(CXX_FLAGS) -o $@ $^
	./$@

# Make object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCLUDE_DIR)/%.h
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CXX_FLAGS) -o $@ $^

clean:
	rm -fr $(OBJ_DIR) $(BIN_DIR)
