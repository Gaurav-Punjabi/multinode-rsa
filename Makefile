CXX=g++
CXX_FLAGS= -I./includes

default: test

test: 
	${CXX} ${CXX_FLAGS} -O3 -o test test.cpp utils.cpp -fopenmp

clean:
	rm -f test
