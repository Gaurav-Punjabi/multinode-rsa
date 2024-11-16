#ifndef RSA_PROCESSOR_SERIAL_HEADER
#define RSA_PROCESSOR_SERIAL_HEADER

#include <cstring>
#include "rsa.h"

std::string encrypt_string(struct Key* key, std::string message);

std::string decrypt_string(struct Key* key, std::string cipher_text);

#endif // !RSA_PROCESSOR_SERIAL_HEADER

