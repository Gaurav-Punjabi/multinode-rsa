#include <iostream>
#include <sstream>
#include <vector>
#include <mpi.h>

#include "includes/utils.h"
#include "includes/file_utils.h"
#include "includes/workload_utils.h"
#include "includes/key_generator.h"
#include "includes/rsa_processor_serial.h"
#include "includes/rsa_processor_openmp.h" 
#include "includes/rsa_processor_cuda.h"

#define BLOCK_SIZE 256


using namespace std;

int main(int argc, char** argv) {
  struct Key* key;
  int private_key, public_key, e;
  int world_size, world_rank;
  bool needs_encryption;
  vector<string> file_list;
  vector<int> displacements, counts;
  string target_dir, serialized_string;

  MPI_Init(NULL, NULL);

  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  if(argc < 2) {
    if(world_rank == MASTER_WORLD) { 
      cerr << "Invalid arguments passed. Directory name is required" << endl;
    }
    MPI_Finalize();
    return 1;
  }

  // Getting the list of files present in the given directory
  if(world_rank == MASTER_WORLD) {
    string operation_type = argv[1];
    if(operation_type == "encrypt") {
      needs_encryption = 1;
    } else if(operation_type == "decrypt") {
      needs_encryption = 0;
    } else {
      cerr << "Invalid operation type specified (Valid options : encrypt / decrypt)" << endl;
      MPI_Finalize();
      return 1;
    }
    target_dir = argv[2]; 
    if(needs_encryption) {
      key = generate_key();
      private_key = key->private_key;
      public_key = key->public_key;
      e = key->e;

      cout << "Private Key : " << private_key << " Public Key : " << public_key << endl;
    } else {
      key = (struct Key*)malloc(sizeof(struct Key));
      private_key = stoi(argv[3]);
      public_key = stoi(argv[4]);
      e = 1;
    }
    file_list = get_files(target_dir);
    serialized_string = divide_workload(file_list, world_size, counts, displacements); 
  }

  MPI_Bcast(&needs_encryption, 1, MPI_CXX_BOOL, MASTER_WORLD, MPI_COMM_WORLD);
  MPI_Bcast(&private_key, 1, MPI_INT, MASTER_WORLD, MPI_COMM_WORLD);
  MPI_Bcast(&public_key, 1, MPI_INT, MASTER_WORLD, MPI_COMM_WORLD);
  MPI_Bcast(&e, 1, MPI_INT, MASTER_WORLD, MPI_COMM_WORLD);

  struct Key* local_key = (struct Key *)malloc(sizeof(struct Key));
  local_key->private_key = private_key;
  local_key->public_key = public_key;
  local_key->e = e;

  int local_file_count;
  MPI_Scatter(counts.data(), 1, MPI_INT, &local_file_count, 1,
      MPI_INT, MASTER_WORLD, MPI_COMM_WORLD);

  vector<char> local_data(local_file_count);
  MPI_Scatterv(serialized_string.data(), counts.data(), 
      displacements.data(), MPI_CHAR, local_data.data(), 
      local_file_count, MPI_CHAR, MASTER_WORLD, MPI_COMM_WORLD);

  vector<string> local_file_list;
  deserialize_file_list(local_data, local_file_list);

  for(string file : local_file_list) {
    string file_content = read_file(file);
    string new_content;
    if(needs_encryption) {
      new_content = encrypt_string_gpu(local_key, file_content, BLOCK_SIZE);
    } else {
      new_content = decrypt_string_gpu(local_key, file_content, BLOCK_SIZE);
    }

    write_file(file, new_content);
  }

  MPI_Finalize();
  return 0;
}
