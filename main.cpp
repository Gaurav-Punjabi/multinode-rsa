#include <iostream>
#include <sstream>
#include <cmath>
#include <omp.h>
#include "includes/utils.h"
#include "includes/key_generator.h"
#include "includes/rsa.h"

using namespace std;

int main() {
  double start_time, end_time, encryption_time, decryption_time;
  struct Key* key = generate_key();
  size_t string_length = 999;
  string input = generate_long_string(string_length);

  string cipher_text = "";

  start_time = omp_get_wtime();

  stringstream cipher_text_stream;

    #pragma omp parallel
    {
        // Thread-local storage for each thread's result
        stringstream local_cipher_text;

        // Parallel for loop
        #pragma omp for
        for (int i = 0; i < input.length(); i++) {
            int message = input[i];
            int cipher = encrypt(key, message);

            // Append the result to the thread's local stringstream
            local_cipher_text << cipher << " ";
        }

        // After the loop, accumulate the local results into the global result
        #pragma omp critical
        {
            cipher_text_stream << local_cipher_text.str();
        }
    }
    cipher_text = cipher_text_stream.str();

  // #pragma omp parallel for num_threads(4)
  // for(int i=0;i<input.length();i++) {
  //   int message = input[i];
  //   int cipher = encrypt(modulus, e, message);
  //   cipher_text.append(to_string(cipher));
  //   cipher_text.append(" ");
  // }

  end_time = omp_get_wtime();

  encryption_time = end_time - start_time;
  cout << "\nTotal encryption time taken : " << encryption_time;


  istringstream iss(cipher_text);

  string decrypted_text = "";

  int num;

  start_time = omp_get_wtime();
  while(iss >> num) {
    int decrypted_message = decrypt(key, num);
    decrypted_text.push_back((char)decrypted_message);
  }
  end_time = omp_get_wtime();
  decryption_time = end_time - start_time;

  cout << "\nTotal decryption time taken : " << decryption_time << endl;

  // cout << "\nThe original message was : " << message; 
  // long cipher = encrypt(modulus, e, message);
  // cout << "\nMessage after encryption : " << cipher;

  // long decrypted_message = decrypt(private_key, modulus, cipher);

  // cout << "\nThe decrypted text is : " << decrypted_message;
  return 0;
}
