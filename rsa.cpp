#include "includes/rsa.h"
#include <iostream>

int encrypt(struct Key* key, int message) {
  return mod_exp(message, key->e, key->public_key);
}

int decrypt(struct Key* key, int cipher) {
  return mod_exp(cipher, key->private_key, key->public_key);
}
