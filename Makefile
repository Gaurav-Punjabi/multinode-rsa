CXX=g++
CXX_FLAGS= -I./includes -O3 -fopenmp
DEPENDENCIES = utils.cpp key_generator.cpp rsa.cpp processors/rsa_processor_serial.cpp processors/rsa_processor_openmp.cpp
TARGET=main

default: $(TARGET)

main: 
	${CXX} ${CXX_FLAGS} -o $(TARGET) $(TARGET).cpp $(DEPENDENCIES)

clean:
	rm -f main
