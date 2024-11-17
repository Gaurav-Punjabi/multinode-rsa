
# Multi-Node RSA Encryption/Decryption Project

This project implements the RSA algorithm, leveraging **OpenMP** for parallelization at the thread level and **OpenMPI** to distribute workload across multiple machines. The application processes all files in a specified folder, splitting the workload among machines. Each machine performs encryption/decryption in parallel using OpenMP, ensuring high performance and scalability.

---

## Prerequisites

To build and run this project, the following tools are required:

- **GCC Compiler**: Ensure GCC is installed on your system.
- **OpenMPI**: Required for distributed workload handling.
  - For **WAVE HPC users**, load OpenMPI using:
    ```bash
    module load OpenMPI
    ```
- **Make**: Required to compile the project.

---

## Building the Project

To compile the executable:
```bash
make
```
This will generate an executable file named `main`.

---

## Usage

The `main` executable supports two operations: **encryption** and **decryption**.

### **Encrypting Files**

To encrypt all files inside a directory:
```bash
./main encrypt <file_dir_path>
```

- **Input**: `file_dir_path` is the path to the folder containing files to be encrypted.
- **Output**: 
  - The program generates a **public key** and **private key**.
  - **Important**: Save these keys securely, as they are required to decrypt the files.

### **Decrypting Files**

To decrypt files inside a directory:
```bash
./main decrypt <file_dir_path> <private_key> <public_key>
```

- **Input**:
  - `file_dir_path`: Path to the folder containing encrypted files.
  - `private_key`: The private key generated during encryption.
  - `public_key`: The public key generated during encryption.

---

## Steps to Record RSA Performance

The project includes a utility to record the performance of the RSA algorithm, implemented in `record_rsa_performance.cpp`.

### Steps for WAVE HPC Users:
1. **Login to WAVE HPC**:
   Connect to the WAVE HPC environment.

2. **Clone the Repository**:
   Clone this repository to your WAVE HPC workspace.

3. **Load Necessary Modules**:
   Load the GCC compiler and OpenMPI using:
   ```bash
   module load OpenMPI
   ```

4. **Build the Performance Testing Executable**:
   Use the following command:
   ```bash
   make record_rsa_performance
   ```

5. **Run the Performance Test**:
   Execute the program:
   ```bash
   ./record_rsa_performance
   ```

### What the Program Does:
- Generates a random string of specified length.
- Encrypts and decrypts the string using the RSA algorithm.
- Logs the execution time for the encryption and decryption phases.

---

## Key Features
- **Multi-Node Scalability**: Efficient workload distribution using OpenMPI.
- **Thread-Level Parallelism**: High-speed encryption and decryption using OpenMP.
- **Performance Logging**: Measure and analyze the performance of RSA encryption/decryption.

Feel free to explore the project, and let us know if you encounter any issues or have suggestions for improvements!

