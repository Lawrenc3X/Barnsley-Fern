CC = gcc
CXX = g++
CFLAGS = -lncurses
CXXFLAGS = -lncurses

Barnsley: Barnsley.cpp
	$(CXX) $(CXXFLAGS) -o Barnsley Barnsley.cpp

all: Barnsley.cpp 
	$(CXX) $(CXXFLAGS) -o Barnsley Barnsley.cpp

clean:
	rm -r Barnsley Barnsley.dSYM
