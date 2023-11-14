CXX = g++
CXXFLAGS = -W -g

mytest: mytest.cpp pqueue.cpp 
	$(CXX) $(CXXFLAGS) mytest.cpp pqueue.cpp -o mytest

run:
	./mytest