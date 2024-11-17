#include <iostream>
#include <sstream>
#include <omp.h>
#include "includes/math_utils.h"
#include "includes/test_utils.h"
#include "includes/key_generator.h"
#include "includes/rsa_processor_serial.h"
#include "includes/rsa_processor_openmp.h" 

using namespace std;

int main() {
  string input_text, cipher_text, decrypted_text;
  double start_time, end_time, encryption_time, decryption_time;
  struct Key* key = generate_key();
  // Update the length to change the input string dimensions
  size_t string_length = 9999999;
  
  input_text = generate_long_string(string_length);
  // input_text = "Hello";

  // Encryption
  start_time = omp_get_wtime();
  // Open MP Version : encrypt_string(key, message, thread_count)
  // cipher_text = encrypt_string(key, input_text, 6);
  // Serial Version
  cipher_text = encrypt_string(key, input_text);
  end_time = omp_get_wtime();
  encryption_time = end_time - start_time;
 


  // Decryption
  start_time = omp_get_wtime();
  // Open MP Version : decrypt_string(key, cipher, thread_count)
  // decrypted_text = decrypt_string(key, cipher_text, 6);
  // Serial Version
  decrypted_text = decrypt_string(key, cipher_text);
  end_time = omp_get_wtime();
  decryption_time = end_time - start_time;

  cout << "The encryption time is " << encryption_time << endl;

  cout << "The decryption time is " << decryption_time << endl;

  return 0;
}
