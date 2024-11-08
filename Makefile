# Compiler + flags
CXX 		  = clang++
CXX_FLAGS = -std=c++20 -Wall -Iinclude

# Dirs
SRC_DIR     = src
INCLUDE_DIR = include
OBJ_DIR     = obj
OBJECTS     = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)%.o)  # What the fuck
BIN_DIR     = bin
TESTS_DIR   = tests
EXECUTABLE  = $(BIN_DIR)/qpsk_encoder

# Use object files to make program
%: %.cpp
	# mkdir -p $(BIN_DIR)
	@echo "HERE"
	$(CXX) $(CXX_FLAGS) -o $@ $^
	# ./$(BIN_DIR)/$@

# Make object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCLUDE_DIR)/%.h
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CXX_FLAGS) -o $@ $^

clean:
	rm -fr $(OBJ_DIR) $(BIN_DIR)
