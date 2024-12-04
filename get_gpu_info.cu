// gpu_properties.cpp
#include <iostream>
#include <cuda_runtime.h>

void printDeviceProperties() {
    int deviceCount;
    cudaGetDeviceCount(&deviceCount);
    
    if (deviceCount == 0) {
        std::cout << "No CUDA devices found." << std::endl;
        return;
    }

    for (int dev = 0; dev < deviceCount; dev++) {
        cudaDeviceProp deviceProp;
        cudaGetDeviceProperties(&deviceProp, dev);
        
        std::cout << "Device " << dev << ": " << deviceProp.name << std::endl;
        std::cout << "  Max Threads per Block: " << deviceProp.maxThreadsPerBlock << std::endl;
        std::cout << "  Max Threads per Multiprocessor: " << deviceProp.maxThreadsPerMultiProcessor << std::endl;
        std::cout << "  Max Blocks per MultiProcessor: " << deviceProp.maxBlocksPerMultiProcessor << std::endl;
        std::cout << "  Max Shared Memory per Block: " << deviceProp.sharedMemPerBlock / (1024) << " KB" << std::endl;
        std::cout << "  Total Global Memory: " << deviceProp.totalGlobalMem / (1024 * 1024) << " MB" << std::endl;
        std::cout << "  Compute Capability: " << deviceProp.major << "." << deviceProp.minor << std::endl;
        std::cout << "  Multi-Processor Count: " << deviceProp.multiProcessorCount << std::endl;
    }
}

int main() {
    printDeviceProperties();
    return 0;
}

