#include <fstream>
#include <sstream>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <vector>
#include "../includes/file_utils.h"

std::string read_file(std::string path) {
  std::ifstream file(path);
  if(!file.is_open()) {
    std::cerr << "Error: could not open file on path : " << path << std::endl;
  }

  std::ostringstream buffer;
  buffer << file.rdbuf();
  std::string file_contents = buffer.str();

  file.close();

  return file_contents;
}

void write_file(std::string path, std::string content) {
  std::ofstream file(path, std::ios::out);
  if(!file.is_open()) {
    std::cerr << "Error: could not open file on path " << path << std::endl;
  }

  file << content;

  file.close();
}

std::vector<std::string> get_files(std::string target_dir) {
  std::vector<std::string> file_list;
  try {
    for(const auto& entry : std::filesystem::directory_iterator(target_dir)) {
      file_list.push_back(entry.path().string());
    }
  } catch (std::filesystem::filesystem_error &e) {
    std::cerr << "Error : " << e.what() << std::endl;
  }
  return file_list;
}
