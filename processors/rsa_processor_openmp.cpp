#include <omp.h>
#include <sstream>
#include <vector>
#include "../includes/rsa_processor_openmp.h"

std::string encrypt_string(struct Key* key, std::string message, int thread_count) {
  std::vector<int> cipher_list(message.length());

  #pragma omp parallel num_threads(thread_count) 
  {
    #pragma omp for
    for(int i=0;i<message.length();i++) {
      int target = message[i];
      int cipher = encrypt(key, target);

      cipher_list[i] =  cipher;
    }
  }

  std::stringstream cipher_text_stream;
  for(int cipher : cipher_list) {
    cipher_text_stream << cipher << " ";
  }
  return cipher_text_stream.str();
}

std::string decrypt_string(struct Key* key, std::string cipher_text, int thread_count) {
  std::istringstream iss(cipher_text);
  std::stringstream decrypted_stream;

  std::vector<int> cipher_list;
  int temp;
  while(iss >> temp) {
    cipher_list.push_back(temp);
  }

  std::vector<char> decrypted_chars(cipher_list.size());

  #pragma omp parallel num_threads(thread_count) 
  {
    #pragma omp for 
    for(int i=0;i<cipher_list.size();i++) {
      int cipher = cipher_list[i];
      int decrypted_message = decrypt(key, cipher);
      decrypted_chars[i] =  ((char)decrypted_message);
    }
  }

  return std::string(decrypted_chars.begin(), decrypted_chars.end());
}
