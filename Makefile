CXX := g++
CXX_FLAGS := -Wall -Wextra -std=c++20 -ggdb

BIN := bin
SRC := src
INCLUDE := include
LIB := lib
LIBRARIES :=
EXECUTABLE := main

all: $(BIN)/$(EXECUTABLE)

run: clean all
	@echo "Executing..."
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	@echo "Building..."
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)

clean:
	@echo "Clearing..."
	-rm $(BIN)/*