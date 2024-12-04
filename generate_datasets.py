import os
import time
from pathlib import Path

def get_sample_text():
    """Return sample text that will be repeated."""
    return [
        "This is a sample line of text for RSA encryption testing.\n",
        "The quick brown fox jumps over the lazy dog.\n",
        "Python is a versatile programming language used worldwide.\n",
        "OpenMP and CUDA are powerful tools for parallel computing.\n",
        "RSA is an asymmetric cryptographic algorithm.\n"
    ]

def generate_meaningful_text(file_path, size_mb):
    """Generate file with repeated meaningful text of specified size."""
    sample_lines = get_sample_text()
    one_block = ''.join(sample_lines)
    block_size = len(one_block.encode('utf-8'))
    
    # Calculate how many blocks we need to reach target size
    target_size = size_mb * 1024 * 1024  # Convert MB to bytes
    num_blocks = (target_size + block_size - 1) // block_size
    
    try:
        with open(file_path, 'w') as f:
            for block in range(num_blocks):
                f.write(one_block)
    except Exception as e:
        print(f"Error writing to {file_path}: {str(e)}")
        raise

def create_dataset(base_dir, file_size_mb, num_files, category_name):
    """Create a dataset directory with specified number of files of given size."""
    # Create directory
    dir_path = Path(base_dir) / category_name
    dir_path.mkdir(parents=True, exist_ok=True)
    
    total_size_gb = (file_size_mb * num_files) / 1024
    print(f"\nGenerating {category_name}:")
    print(f"- {num_files} files")
    print(f"- {file_size_mb}MB each")
    print(f"- Total size: {total_size_gb:.2f}GB")
    
    start_time = time.time()
    
    for i in range(num_files):
        file_path = dir_path / f"file_{i+1}.txt"
        generate_meaningful_text(file_path, file_size_mb)
        
        # Progress update every 10% or for each file if few files
        if num_files < 10 or (i + 1) % (num_files // 10) == 0:
            progress = ((i + 1) / num_files) * 100
            elapsed_time = time.time() - start_time
            print(f"Progress: {progress:.1f}% ({i+1}/{num_files} files) - Time elapsed: {elapsed_time:.1f}s")

def main():
    # Configuration
    SMALL_SIZE_MB = 1
    LARGE_SIZE_MB = 15
    FEW_FILES = 10
    MANY_FILES = 100
    
    # Create base directory for all datasets
    base_dir = Path("test_datasets")
    base_dir.mkdir(exist_ok=True)
    
    # Calculate total storage needed
    datasets = [
        ("small_few", SMALL_SIZE_MB, FEW_FILES),
        ("small_many", SMALL_SIZE_MB, MANY_FILES),
        ("large_few", LARGE_SIZE_MB, FEW_FILES),
        ("large_many", LARGE_SIZE_MB, MANY_FILES)
    ]
    
    total_storage_gb = sum(size * num / 1024 for _, size, num in datasets)
    print(f"Total storage required: {total_storage_gb:.2f}GB")
    
    # Confirm with user
    response = input("Do you want to proceed? (y/n): ")
    if response.lower() != 'y':
        print("Operation cancelled.")
        return
    
    # Generate each dataset
    for category_name, file_size, num_files in datasets:
        create_dataset(base_dir, file_size, num_files, category_name)
        
    print("\nDataset generation complete!")

if __name__ == "__main__":
    main()