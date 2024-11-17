# multinode-rsa
A distributed version of RSA algorithm.

## Steps to run Record RSA Performance
To run `record_rsa_performance.cpp` on WAVE HPC
1. Login to wave HPC
2. Clone the repository
3. Load GCC compiler and OpenMPI using command `module load OpenMPI`
4. To build the executable `make record_rsa_performance`
5. Then run the `record_rsa_performance`

The program generates a string of specified value, encrypts and then decrypts it. It also logs the execution time for each phase.

