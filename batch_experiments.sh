#!/bin/bash
#SBATCH --job-name=rsa_benchmark
#SBATCH --output=rsa_benchmark_%j.out
#SBATCH --error=rsa_benchmark_%j.err
#SBATCH --time=5:00:00
#SBATCH --nodes=4
#SBATCH --ntasks=8   
#SBATCH --cpus-per-task=4
#SBATCH --mem=16G
#SBATCH --partition=cmp

# Load necessary modules
module load OpenMPI

# Base directory and datasets
BASE_DIR="test_datasets"
DATASETS=("large_few" "large_many" "small_few" "small_many")
MPI_TASKS=(1 3 6 8)
RESULTS_FILE="benchmark_results_2.csv"

# Create CSV header if file doesn't exist
if [ ! -f "$RESULTS_FILE" ]; then
    echo "Dataset,Operation,MPI_Tasks,Total_Runtime(s),Process_Runtime(s)" > "$RESULTS_FILE"
fi

# Function to extract both runtimes from program output and save to CSV
run_and_log() {
    local dataset=$1
    local mpi_tasks=$2
    local operation=$3
    
    echo "Running: Dataset=$dataset, MPI=$mpi_tasks, Operation=$operation"
    
    if [ "$operation" = "encrypt" ]; then
        # Capture the output of the program
        output=$(mpirun -np $mpi_tasks ./main encrypt "$BASE_DIR/$dataset")
        # Extract both times using consistent grep pattern
        total_time=$(echo "$output" | grep "Total execution time:" | awk '{print $4}')
        process_time=$(echo "$output" | grep "Encryption/Decryption time:" | awk '{print $3}')
        echo "$dataset,$operation,$mpi_tasks,$total_time,$process_time" >> "$RESULTS_FILE"
        # Store keys for decrypt operation
        private_key=$(echo "$output" | grep "Private Key" | awk '{print $4}')
        public_key=$(echo "$output" | grep "Public Key" | awk '{print $8}')
    else
        output=$(mpirun -np $mpi_tasks ./main decrypt "$BASE_DIR/$dataset" $private_key $public_key)
        total_time=$(echo "$output" | grep "Total execution time:" | awk '{print $4}')
        process_time=$(echo "$output" | grep "Encryption/Decryption time:" | awk '{print $3}')
        echo "$dataset,$operation,$mpi_tasks,$total_time,$process_time" >> "$RESULTS_FILE"
    fi
}

# Main execution
for dataset in "${DATASETS[@]}"; do
    for tasks in "${MPI_TASKS[@]}"; do
        run_and_log "$dataset" "$tasks" "encrypt"
        run_and_log "$dataset" "$tasks" "decrypt"
    done
done

echo "Benchmarking complete!"
echo "Results saved in $RESULTS_FILE"