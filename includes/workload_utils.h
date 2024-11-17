#ifndef WORKLOAD_UTILS_HEADER
#define WORKLOAD_UTILS_HEADER
#include <vector>
#include <string>

std::string divide_workload(std::vector<std::string> file_list, int world_size, std::vector<int>& counts, std::vector<int>& displacements);

void deserialize_file_list(std::vector<char> serialized_text, std::vector<std::string>& file_list);

#endif // !WORKLOAD_UTILS_HEADER
