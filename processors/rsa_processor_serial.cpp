#include <sstream>
#include "../includes/rsa_processor_serial.h"

std::string encrypt_string(struct Key* key, std::string message) {
  std::stringstream cipher_text_stream;

  for(int i=0;i<message.length();i++) {
    int target = message[i];
    int cipher = encrypt(key, target);

    cipher_text_stream << cipher << " ";
  }

  return cipher_text_stream.str();
}

std::string decrypt_string(struct Key* key, std::string cipher_text) {
  std::stringstream decrypted_text_stream;
  std::istringstream iss(cipher_text);

  int target;
  while(iss >> target) {
    int decrypted_message = decrypt(key, target);
    decrypted_text_stream << ((char)decrypted_message);
  }

  return decrypted_text_stream.str();
}
