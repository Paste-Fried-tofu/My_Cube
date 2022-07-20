CXX = g++
INCLUDES = -Iinclude
SRC = src/Cube.cpp \
		src/get_move.cpp \
		src/solver.cpp
		
LFLAGS = -pthread
CFLAGS = -Wall -std=c++11 -g -pipe -DDEBUG

EXAMPLE_SRC = example_program/example.cpp

EXAMPLE_OBJ = $(EXAMPLE_SRC:.cpp = .o)

OBJ = $(SRC:.cpp = .o) $(EXAMPLE_OBJ)

EXE = example

.PHONY:clean



EXE:$(OBJ)
	$(CXX) $(CFLAGS) $(OBJ) $(INCLUDES) $(LFLAGS) -o $(EXE)
.cpp.o:
	$(CXX) $(INCLUDES) $(CFLAGS) -c $< -o $@ 
clean:
	rm $(OBJ)
