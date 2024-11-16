#include <iostream>
#include <sstream>
#include <omp.h>
#include "includes/utils.h"
#include "includes/key_generator.h"
#include "includes/rsa_processor_serial.h"
#include "includes/rsa_processor_openmp.h" 

using namespace std;

int main() {
  string input_text, cipher_text, decrypted_text;
  double start_time, end_time, encryption_time, decryption_time;
  struct Key* key = generate_key();
  size_t string_length = 999;
  
  // input_text = generate_long_string(string_length);
  input_text = "Hello";
  cout << "The input text is : " << input_text << endl;

  // Encryption
  start_time = omp_get_wtime();
  cipher_text = encrypt_string(key, input_text, 4);
  end_time = omp_get_wtime();
  encryption_time = end_time - start_time;
  
  cout << "The cipher text is : " << cipher_text << endl;

  // Decryption
  start_time = omp_get_wtime();
  decrypted_text = decrypt_string(key, cipher_text, 4);
  end_time = omp_get_wtime();
  decryption_time = end_time - start_time;

  cout << "The decrypted text is : " << decrypted_text << endl;

  cout << "The encryption time is " << encryption_time << endl;

  cout << "The decryption time is " << decryption_time << endl;

  cout << "\nTotal decryption time taken : " << decryption_time << endl;

  return 0;
}
