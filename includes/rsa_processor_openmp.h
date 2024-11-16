#ifndef RSA_PROCESSOR_OPENMP_HEADER
#define RSA_PROCESSOR_OPENMP_HEADER

#include <cstring>
#include "rsa.h"

std::string encrypt_string(struct Key* key, std::string message, int thread_count);

std::string decrypt_string(struct Key* key, std::string cipher_text, int thread_count);

#endif // !RSA_PROCESSOR_OPENMP_HEADER
