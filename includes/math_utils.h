#ifndef MATH_UTILS_HEADER
#define MATH_UTILS_HEADER
#include <stdexcept>

int compute_modulus(int p, int q);

int compute_eulers_totient(int p, int q);

int mod_inverse(int e, int eulers_totient);

int mod_exp(unsigned long long base, unsigned long long exp, unsigned long long mod);

#endif // !MATH_UTILS_HEADER


