#ifndef KEY_GENERATOR_HEADER
#define KEY_GENERATOR_HEADER

#include "utils.h"
#include <cstring>

struct Key {
  int public_key, private_key;
  int e;
};

struct Key* generate_key(int p, int q);

#endif // !KEY_GENERATOR_HEADER


