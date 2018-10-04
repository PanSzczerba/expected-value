CXXFLAGS=-std=c++11 -Wall -O3
LIBS=-lpthread

all: expected_value

expected_value.o: expected_value.cpp ../highest_subsequence_sum.h

expected_value: expected_value.o
	$(CXX) $(CXXFLAGS) -o $@ $< $(LIBS)

clean:
	$(RM) *.o
	$(RM) expected_value
