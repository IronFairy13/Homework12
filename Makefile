.PHONY: all clean test

CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -pedantic
CXXFLAGS += -I.
COMMON_SOURCES = stats.cpp

bin:
	mkdir -p bin

all: bin/mapper bin/reducer

bin/mapper: mapper.cpp $(COMMON_SOURCES) stats.hpp | bin
	$(CXX) $(CXXFLAGS) mapper.cpp $(COMMON_SOURCES) -o $@

bin/reducer: reducer.cpp $(COMMON_SOURCES) stats.hpp | bin
	$(CXX) $(CXXFLAGS) reducer.cpp $(COMMON_SOURCES) -o $@

bin/tests: tests/test_stats.cpp tests/gtestlite.hpp $(COMMON_SOURCES) stats.hpp | bin
	$(CXX) $(CXXFLAGS) tests/test_stats.cpp $(COMMON_SOURCES) -o $@ -pthread

test: bin/tests
	./bin/tests

clean:
	rm -rf bin output
