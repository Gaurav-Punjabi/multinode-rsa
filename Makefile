CXX=g++
CXX_FLAGS= -I./includes

default: main

main: 
	${CXX} ${CXX_FLAGS} -O3 -o main main.cpp utils.cpp key_generator.cpp rsa.cpp -fopenmp

clean:
	rm -f main
