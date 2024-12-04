#include <omp.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <cuda_runtime.h>
#include "../includes/rsa_processor_cuda.h"

// CUDA error checking macro
#define CUDA_CHECK(call) \
do { \
    cudaError_t err = call; \
    if (err != cudaSuccess) { \
        std::cerr << "CUDA Error in " << __FILE__ << " at line " << __LINE__ << ": " \
                  << cudaGetErrorString(err) << std::endl; \
        throw std::runtime_error("CUDA Error: " + std::string(cudaGetErrorString(err))); \
    } \
} while(0)

__device__ int mod_exp_gpu(unsigned long long base, unsigned long long exp, unsigned long long mod) {
    unsigned long long result = 1;
    base = base % mod; // Handle large base values
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp /= 2;
    }

    return (int)result;
}

__device__ int encrypt_gpu(struct Key* key, int message) {
    return mod_exp_gpu(message, key->e, key->public_key);
}

__device__ int decrypt_gpu(struct Key* key, int cipher) {
    return mod_exp_gpu(cipher, key->private_key, key->public_key);
}

// CUDA kernel for encryption
__global__ void encrypt_kernel(struct Key* key, char* message, int* cipher_list, int message_length) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < message_length) {
        int target = message[idx];
        cipher_list[idx] = encrypt_gpu(key, target);
    }
}

// CUDA kernel for decryption
__global__ void decrypt_kernel(struct Key* key, int* cipher_list, char* decrypted_chars, int cipher_length) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < cipher_length) {
        int cipher = cipher_list[idx];
        decrypted_chars[idx] = (char)decrypt_gpu(key, cipher);
    }
}

// Helper function to copy the key to device memory
void copy_key_to_device(struct Key* key, struct Key** d_key) {
    CUDA_CHECK(cudaMalloc((void**)d_key, sizeof(struct Key)));
    CUDA_CHECK(cudaMemcpy(*d_key, key, sizeof(struct Key), cudaMemcpyHostToDevice));
}

// Encryption function with CUDA and error checking
std::string encrypt_string_gpu(struct Key* key, std::string message, int block_size) {
    try {
        int message_length = message.length();
        
        // Allocate device memory
        char* d_message = nullptr;
        int* d_cipher_list = nullptr;
        struct Key* d_key = nullptr; // Device pointer for key
        
        CUDA_CHECK(cudaMalloc(&d_message, message_length * sizeof(char)));
        CUDA_CHECK(cudaMalloc(&d_cipher_list, message_length * sizeof(int)));
        
        // Copy host data to device
        CUDA_CHECK(cudaMemcpy(d_message, message.c_str(), message_length * sizeof(char), cudaMemcpyHostToDevice));
        
        // Copy key to device
        copy_key_to_device(key, &d_key);
        
        // Configure grid and block dimensions
        int grid_size = (message_length + block_size - 1) / block_size;
        
        // Launch encryption kernel
        encrypt_kernel<<<grid_size, block_size>>>(d_key, d_message, d_cipher_list, message_length);
        
        // Check for kernel launch errors
        CUDA_CHECK(cudaGetLastError());
        
        // Synchronize to ensure kernel completion
        CUDA_CHECK(cudaDeviceSynchronize());
        
        // Allocate host memory for results
        std::vector<int> cipher_list(message_length);
        
        // Copy results back to host
        CUDA_CHECK(cudaMemcpy(cipher_list.data(), d_cipher_list, message_length * sizeof(int), cudaMemcpyDeviceToHost));
        
        // Free device memory
        CUDA_CHECK(cudaFree(d_message));
        CUDA_CHECK(cudaFree(d_cipher_list));
        CUDA_CHECK(cudaFree(d_key));
        
        // Convert to string
        std::stringstream cipher_text_stream;
        for(int cipher : cipher_list) {
            cipher_text_stream << cipher << " ";
        }
        
        return cipher_text_stream.str();
    }
    catch (const std::exception& e) {
        std::cerr << "Error in encrypt_string: " << e.what() << std::endl;
        throw;
    }
}

// Decryption function with CUDA and error checking
std::string decrypt_string_gpu(struct Key* key, std::string cipher_text, int block_size) {
    try {
        // Parse cipher text
        std::istringstream iss(cipher_text);
        std::vector<int> cipher_list;
        int temp;
        while(iss >> temp) {
            cipher_list.push_back(temp);
        }
        int cipher_length = cipher_list.size();
        
        // Allocate device memory
        int* d_cipher_list = nullptr;
        char* d_decrypted_chars = nullptr;
        struct Key* d_key = nullptr; // Device pointer for key
        
        CUDA_CHECK(cudaMalloc(&d_cipher_list, cipher_length * sizeof(int)));
        CUDA_CHECK(cudaMalloc(&d_decrypted_chars, cipher_length * sizeof(char)));
        
        // Copy input data to device
        CUDA_CHECK(cudaMemcpy(d_cipher_list, cipher_list.data(), cipher_length * sizeof(int), cudaMemcpyHostToDevice));
        
        // Copy key to device
        copy_key_to_device(key, &d_key);
        
        // Configure grid and block dimensions
        int grid_size = (cipher_length + block_size - 1) / block_size;
        
        // Launch decryption kernel
        decrypt_kernel<<<grid_size, block_size>>>(d_key, d_cipher_list, d_decrypted_chars, cipher_length);
        
        // Check for kernel launch errors
        CUDA_CHECK(cudaGetLastError());
        
        // Synchronize to ensure kernel completion
        CUDA_CHECK(cudaDeviceSynchronize());
        
        // Allocate host memory for results
        std::vector<char> decrypted_chars(cipher_length);
        
        // Copy results back to host
        CUDA_CHECK(cudaMemcpy(decrypted_chars.data(), d_decrypted_chars, cipher_length * sizeof(char), cudaMemcpyDeviceToHost));
        
        // Free device memory
        CUDA_CHECK(cudaFree(d_cipher_list));
        CUDA_CHECK(cudaFree(d_decrypted_chars));
        CUDA_CHECK(cudaFree(d_key));
        
        return std::string(decrypted_chars.begin(), decrypted_chars.end());
    }
    catch (const std::exception& e) {
        std::cerr << "Error in decrypt_string: " << e.what() << std::endl;
        throw;
    }
}

// Optional: Device information function
void printDeviceInfo() {
    int deviceCount;
    CUDA_CHECK(cudaGetDeviceCount(&deviceCount));
    
    for (int dev = 0; dev < deviceCount; dev++) {
        cudaDeviceProp deviceProp;
        CUDA_CHECK(cudaGetDeviceProperties(&deviceProp, dev));
        
        std::cout << "Device " << dev << ": " << deviceProp.name << std::endl;
        std::cout << "  Compute Capability: " << deviceProp.major << "." << deviceProp.minor << std::endl;
        std::cout << "  Total Global Memory: " << deviceProp.totalGlobalMem / (1024 * 1024) << " MB" << std::endl;
        std::cout << "  Multiprocessor Count: " << deviceProp.multiProcessorCount << std::endl;
    }
}

