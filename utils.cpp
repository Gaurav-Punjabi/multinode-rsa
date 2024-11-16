#include <stdexcept>
#include <cstring>
#include "includes/utils.h"

int compute_modulus(int p, int q) {
  return p * q;
}

int compute_eulers_totient(int p, int q) {
  return (p - 1) * (q - 1); 
}

int mod_inverse(int e, int eulers_totient) {
    int t = 0, new_t = 1;
    int r = eulers_totient, new_r = e;

    while (new_r != 0) {
        int quotient = r / new_r;
        int temp_t = new_t;
        new_t = t - quotient * new_t;
        t = temp_t;

        int temp_r = new_r;
        new_r = r - quotient * new_r;
        r = temp_r;
    }

    if (r > 1) throw std::invalid_argument("e is not invertible");
    if (t < 0) t += eulers_totient;

    return t;
}

int mod_exp(unsigned long long base, unsigned long long exp, unsigned long long mod) {
    unsigned long long result = 1;
    base = base % mod; // Handle large base values

    while (exp > 0) {
        // If exp is odd, multiply base with result
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        // Square the base and reduce exp by half
        base = (base * base) % mod;
        exp /= 2;
    }

    return (int)result;
}

std::string generate_long_string(size_t length) {
  std::string long_string(length, 'A');
  return long_string;
}


