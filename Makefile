PROGRAM = vns.out
CXX = g++
CXXFLAGS = -g -std=c++17 -Wall -Wextra 


$(PROGRAM): vns.o  Graph.o
	$(CXX) -o $@ $^

vns.o: vns.cpp  Graph.hpp
	$(CXX) -c -o $@ $(CXXFLAGS) $< 

Graph.o: Graph.cpp Graph.hpp
	$(CXX) -c -o $@ $(CXXFLAGS) $<

.PHONY: clean
clean:
	rm $(PROGRAM) *.o	


