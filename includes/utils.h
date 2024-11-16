#ifndef UTILS_HEADER
#define UTILS_HEADER
#include <cstring>

int compute_modulus(int p, int q);

int compute_eulers_totient(int p, int q);

int mod_inverse(int e, int eulers_totient);

int mod_exp(unsigned long long base, unsigned long long exp, unsigned long long mod);

std::string generate_long_string(size_t length);

#endif // !UTILS_HEADER


