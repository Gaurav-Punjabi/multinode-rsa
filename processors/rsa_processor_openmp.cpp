#include <omp.h>
#include <sstream>
#include "../includes/rsa_processor_openmp.h"

std::string encrypt_string(struct Key* key, std::string message, int thread_count) {
  std::stringstream cipher_text_stream;

  #pragma omp parallel num_threads(thread_count) 
  {
    std::stringstream local_cipher_stream;
    #pragma omp for
    for(int i=0;i<message.length();i++) {
      int target = message[i];
      int cipher = encrypt(key, target);

      local_cipher_stream << cipher << " ";
    }

    #pragma omp critical 
    {
      cipher_text_stream << local_cipher_stream.str();
    }
  }

  return cipher_text_stream.str();
}

std::string decrypt_string(struct Key* key, std::string cipher_text, int thread_count) {
  std::istringstream iss(cipher_text);
  std::stringstream decrypted_stream;

  #pragma omp parallel num_threads(thread_count) 
  {
    std::stringstream local_decrypted_stream;

    int cipher;
    while(iss >> cipher) {
      int decrypted_message = decrypt(key, cipher);
      local_decrypted_stream << ((char)decrypted_message);
    }

    #pragma omp critical 
    {
      decrypted_stream << local_decrypted_stream.str();
    }
  }

  return decrypted_stream.str();
}
