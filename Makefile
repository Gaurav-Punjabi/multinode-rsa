CXX=g++

default: test

test: 
	${CXX} -O3 -o test test.cpp

clean:
	rm -f test
