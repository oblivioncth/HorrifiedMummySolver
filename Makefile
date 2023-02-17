# Makefile for C++17 program
CXX = g++
CXXFLAGS = -std=c++17

SRCS = src/MummySolver.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = MummySolver

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(EXEC)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)
