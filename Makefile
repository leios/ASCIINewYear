# Makefile for general visualization

CXX = g++
CXXFLAGS = -std=c++14 -g -O2 -fPIC

LIBFLAGS = -lncurses

VPATH = src

SRCS = $(wildcard src/*.cpp) 

DEPS = $(wildcard include/*.h)

OBJS = $(notdir $(SRCS:.cpp=.o))

LIB_OBJS = $(filter-out vis_test.o, $(OBJS))

ASCIINewYear: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBFLAGS)

%.o:%.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(LIBFLAGS)

.PHONEY: clean

clean:
	rm -Rf ASCIINewYear *.o
