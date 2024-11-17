#include <iostream>
#include <sstream>
#include "includes/key_generator.h"
#include "includes/rsa_processor_serial.h"
#include "includes/rsa_processor_openmp.h" 

using namespace std;

int main(int argc, char** argv) {
  int world_size, world_rank, file_count;
  vector<string> file_list;
  vector<int> displacements, counts;
  string target_dir;

  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  if(argc < 2) {
    if(world_rank == MASTER_WORLD) { 
      cerr << "Invalid arguments passed. Directory name is required" << endl;
      MPI_Finalize();
    }
    return 1;
  }

  // Getting the list of files present in the given directory
  if(world_rank == MASTER_WORLD) {
    target_dir = argv[1];
    file_list = get_files(target_dir);
    file_count = file_list.size();
  }


  MPI_Finalize();
  return 0;
}































}
