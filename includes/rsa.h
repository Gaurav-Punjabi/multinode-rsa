#ifndef RSA_HEADER
#define RSA_HEADER

#include "utils.h"
#include "key_generator.h"

int encrypt(struct Key* key, int message);

int decrypt(struct Key* key, int cipher);

#endif // !RSA_HEADER

