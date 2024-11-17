#ifndef FILE_UTILS_HEADER
#define FILE_UTILS_HEADER

#include <cstring>
#include <vector>

std::string read_file(std::string path);

std::vector<std::string> get_files(std::string target_dir);

#endif // !FILE_UTILS_HEADER
