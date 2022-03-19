CXX=gcc
CXX_FLAGS=-Wall -Wextra -pedantic -std=c11 -O3 -pthread
.PHONY: all clean
TARGETS=benchmarking barriers

all: $(TARGETS)

benchmarking.o: benchmarking.h
benchmarking.o: benchmarking.c
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

benchmarking: benchmarking.o
	$(CXX) $(CXX_FLAGS) -o $@ $^

barriers.o: barriers.hh
barriers.o: barriers.cc
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

barriers: barriers.o
	$(CXX) $(CXX_FLAGS) -o $@ $^

clean:
	rm -f *~ *.o $(TARGETS)
