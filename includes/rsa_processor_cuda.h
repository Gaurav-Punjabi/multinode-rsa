#ifndef RSA_PROCESSOR_CUDA_HEADER
#define RSA_PROCESSOR_CUDA_HEADER

#include <cstring>
#include "rsa.h"

std::string encrypt_string_gpu(struct Key* key, std::string message, int block_size);

std::string decrypt_string_gpu(struct Key* key, std::string cipher_text, int block_size);

#endif // !RSA_PROCESSOR_CUDA_HEADER
