#include <iostream>
#include <cmath>
#include <stdexcept>

using namespace std;

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

    if (r > 1) throw invalid_argument("e is not invertible");
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

int encrypt(int public_key, int e, int message) {
  return mod_exp(message, e, public_key);
}


int decrypt(int private_key, int public_key, int cipher) {
  return mod_exp(cipher, private_key, public_key);
}



int main() {
  int p = 61, q = 53;
  int e = 65537;
  int modulus = compute_modulus(p, q);
  int eulers_totient = compute_eulers_totient(p, q);
  int private_key = mod_inverse(e, eulers_totient);
  cout << "\nThe private key is : " << private_key;
  cout << "\nThe public key is : " << modulus;
  int message = 89;
  cout << "\nThe original message was : " << message;
  long cipher = encrypt(modulus, e, message);
  cout << "\nMessage after encryption : " << cipher;

  long decrypted_message = decrypt(private_key, modulus, cipher);

  cout << "\nThe decrypted text is : " << decrypted_message;
  return 0;
}
