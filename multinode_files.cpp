#include <iostream>
#include <vector>
#include <mpi.h>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <sstream>

#define MASTER_WORLD 0

using namespace std;

string read_file(string path) {
  ifstream file(path);
  if(!file.is_open()) {
    std::cerr << "Error: could not open file on path : " << path << endl;
  }

  ostringstream buffer;
  buffer << file.rdbuf();
  string file_contents = buffer.str();

  file.close();

  return file_contents;
}

vector<string> get_files(string target_dir) {
  vector<string> file_list;
  try {
    for(const auto& entry : filesystem::directory_iterator(target_dir)) {
      file_list.push_back(entry.path().string());
    }
  } catch (filesystem::filesystem_error &e) {
    cerr << "Error : " << e.what() << endl;
  }
  return file_list;
}

int main(int argc, char** argv) {
  int world_size, world_rank, file_count, sub_file_count;
  vector<string> file_list;
  vector<int> displacements, counts;
  string target_dir;
  string serialized_string;

  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  
  if(world_rank == MASTER_WORLD) {
    target_dir = "input_files";
    file_list = get_files(target_dir);
    file_count = file_list.size();
  }

  MPI_Bcast(&file_count, 1, MPI_INT, MASTER_WORLD, MPI_COMM_WORLD);

  cout << "The total file count : " << file_count << endl; 

  if(world_rank == MASTER_WORLD) {
    sub_file_count = file_count / world_size;
    counts = vector(world_size, sub_file_count);
    displacements = vector(world_size, 0);

    for(int i=0;i<file_count%world_size;i++) {
      counts[i]++;
    }
    int file_index = 0;
    int offset_index = 0;
    int serialized_length = 0;

    for(int i=0;i<world_size;i++) {
      displacements[i] = offset_index; 
      for(int j=0;j<counts[i];j++,file_index++) {
        string new_string = file_list[file_index] + '\0';
        serialized_string += new_string;
        serialized_length += new_string.length();
        cout << "String length : " << new_string.length() << endl;
      }
      counts[i] = serialized_length - offset_index;
      offset_index += counts[i];
    }

    cout << "Total String length : " << serialized_string.length() << endl;
    for(int i=0;i<world_size;i++) {
      cout << "Count : " << counts[i] << " Displacement : " << displacements[i] << endl;
    }
  }

  int local_count;
  MPI_Scatter(counts.data(), 1, MPI_INT, &local_count, 1, MPI_INT, MASTER_WORLD, MPI_COMM_WORLD);
  vector<char> local_data(local_count);
  MPI_Scatterv(serialized_string.data(), counts.data(), displacements.data(), 
      MPI_CHAR, local_data.data(), local_count, MPI_CHAR, MASTER_WORLD, MPI_COMM_WORLD);

  vector<string> local_file_list;
  string current_string;
  for(char c : local_data) {
    if(c == '\0') {
      local_file_list.push_back(current_string);
      current_string.clear();
    } else {
      current_string += c;
    }
  }

  for(string file : local_file_list) {
    cout << endl << "Showing file content from " << world_rank << endl;
    string file_content = read_file(file);
    cout << file_content << endl;
  }


  MPI_Finalize();
  return 0;

}
