#ifndef KEY_GENERATOR_HEADER
#define KEY_GENERATOR_HEADER

#include "math_utils.h"
#include <cstring>

struct Key {
  int public_key, private_key;
  int e;
};

struct Key* generate_key();

#endif // !KEY_GENERATOR_HEADER


