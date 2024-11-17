#include <string>
#include <vector>

#include "../includes/workload_utils.h"

std::string divide_workload(std::vector<std::string> file_list, int world_size,
    std::vector<int>& counts, std::vector<int>& displacements) {
  int total_file_count = file_list.size();
  int sub_file_count = total_file_count / world_size;                         
  std::string serialized_string;

  counts = std::vector(world_size, sub_file_count);
  displacements = std::vector(world_size, 0);

  // Distribute remainder files if it is not divisible by num processors
  for(int i=0;i<total_file_count%world_size;i++) {  
    counts[i]++;
  }

  int file_index = 0;
  int offset_index = 0;
  int serialized_length = 0;

  for(int i=0;i<world_size;i++) {
    displacements[i] = offset_index;
    for(int j=0;j<counts[i];j++,file_index++) {
      std::string new_string = file_list[file_index] + '\0';
      serialized_string += new_string;
      serialized_length += new_string.length();
    }

    counts[i] = serialized_length - offset_index;
    offset_index += counts[i];
  }
  
  return serialized_string;
}

void deserialize_file_list(std::vector<char> serialized_text, std::vector<std::string>& file_list) {
  std::string current_string;
  for(char c : serialized_text) {
    if(c == '\0') {
      file_list.push_back(current_string);
      current_string.clear();
    } else {
      current_string += c;
    }
  }
} 
