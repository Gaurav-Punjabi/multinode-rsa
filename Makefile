# Compiler definitions
CXX=oshc++        # MPI C++ compiler (replace this with your specific MPI compiler)
NVCC=nvcc         # CUDA compiler (nvcc)
MPI_FLAGS=-lmpi    # MPI linking flag

# Compiler flags
CXX_FLAGS=-I./includes -O3 -fopenmp  # OpenMP and optimization for C++
NV_FLAGS=-O3 -Xcompiler -fopenmp     # Pass OpenMP to host compiler for CUDA files

# Dependencies
CXX_DEPENDENCIES = \
    utils/math_utils.cpp \
    utils/test_utils.cpp \
    key_generator.cpp \
    rsa.cpp \
    processors/rsa_processor_serial.cpp \
    processors/rsa_processor_openmp.cpp \
    utils/file_utils.cpp \
    utils/workload_utils.cpp

CUDA_DEPENDENCIES = \
    processors/rsa_processor_cuda.cu  # CUDA-specific dependencies

# Target names
TARGET=main  # Target executable name
CUDA_TARGET=main.cu  # CUDA source file

# Default target
default: $(TARGET)

# Main target (compiled with MPI, OpenMP, and CUDA)
$(TARGET): $(CUDA_TARGET) $(CXX_DEPENDENCIES) $(CUDA_DEPENDENCIES)
	$(NVCC) $(NV_FLAGS) -o $(TARGET) $(CUDA_TARGET) $(CXX_DEPENDENCIES) $(CUDA_DEPENDENCIES) $(MPI_FLAGS)

# Clean up compiled files
clean:
	rm -f $(TARGET)

