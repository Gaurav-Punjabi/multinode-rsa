CXX=g++
CXX_FLAGS= -I./includes -O3 -fopenmp
DEPENDENCIES = utils/math_utils.cpp utils/test_utils.cpp key_generator.cpp rsa.cpp processors/rsa_processor_serial.cpp processors/rsa_processor_openmp.cpp utils/file_utils.cpp
TARGET=main

default: $(TARGET) 

main: 
	${CXX} ${CXX_FLAGS} -o $(TARGET) $(TARGET).cpp $(DEPENDENCIES)

record_rsa_performance:  
	${CXX} ${CXX_FLAGS} -o record_rsa_performance $(DEPENDENCIES) record_rsa_performance.cpp

clean:
	rm -f main record_rsa_performance
