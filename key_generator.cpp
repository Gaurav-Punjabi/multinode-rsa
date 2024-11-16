#include "includes/key_generator.h"

struct Key* generate_key() {
  int p = 61, q = 53;
  int e = 65537;
  int public_key = compute_modulus(p, q);
  int eulers_totient = compute_eulers_totient(p, q);
  int private_key = mod_inverse(e, eulers_totient);

  struct Key* key = (struct Key*)malloc(sizeof(struct Key));
  if(key == NULL) {
    return NULL;
  }

  key->public_key = public_key;
  key->private_key = private_key;
  key->e = e;

  return key;
}
