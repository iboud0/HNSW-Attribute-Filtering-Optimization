# HNSW Attribute Filtering Optimization

## Overview
This repository implements an optimized multi-attribute filtering mechanism for Hierarchical Navigable Small World (HNSW) graphs used in Approximate Nearest Neighbor (ANN) search. The goal is to reduce query latency and improve filtering performance for datasets with large attribute spaces (e.g., 1000+ labels per point) by integrating:
- **Bitsets**: For efficient representation and evaluation of attributes.
- **Roaring Bitmaps**: For compact storage and high-performance operations on sparse attributes.

## Features
- **Bitset-based filtering**: Enables rapid evaluation of attribute matches using bitwise operations.
- **Roaring Bitmap integration**: Optimizes storage and set operations for sparse datasets.
- **Dynamic query handling**: Automatically switches between brute-force and ANN search based on filtered set size.
- **HNSWLib enhancements**: Extends HNSW traversal and filtering mechanisms for better performance.

## Project Goals
1. Represent attributes as bitsets and Roaring Bitmaps for efficient filtering.
2. Integrate filtering logic into HNSWLib traversal algorithms.
3. Benchmark performance improvements in query latency and memory consumption.
4. Ensure high recall accuracy and robust performance across various datasets.

## Setup

### Prerequisites
- **C++ Compiler** (e.g., GCC, Clang)
- **CMake** (for building HNSWLib and project dependencies)
- **CRoaring Library** ([GitHub Repository](https://github.com/RoaringBitmap/CRoaring))

### Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/HNSW-Attribute-Filtering-Optimization.git
   ```

2. Navigate to the project directory:
   ```bash
   cd HNSW-Attribute-Filtering-Optimization
   ```

3. Build the project:
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```

---

### Usage

#### Running Benchmarks
To evaluate the performance improvements of the optimized filtering:
1. Prepare datasets with varying attribute distributions and save them in a supported format.
2. Run the benchmark tool included in the project:
   ```bash
   ./benchmark [dataset_path]
   ```

#### Modifying Parameters
- Customize the following in `config.h`:
  - **Number of attributes per point**: Define the size of the bitsets.
  - **Filtering thresholds**: Set constraints for filtering.
  - **Traversal parameters**: Adjust HNSW layer configurations and search heuristics.

---

### Contributing
Contributions to the project are welcome! To contribute:
1. Fork this repository.
2. Create a branch for your feature or bug fix:
   ```bash
   git checkout -b feature/your-feature
   ```
3. Commit your changes with clear messages:
   ```bash
   git commit -m "Add detailed description of changes"
   ```
4. Push your changes:
   ```bash
   git push origin feature/your-feature
   ```
5. Open a pull request, explaining your additions or fixes.

---

### Acknowledgments
- The project builds upon the **Hierarchical Navigable Small World Graphs** algorithm. Read the foundational paper here: [HNSW Paper](https://arxiv.org/abs/1603.09320).
- For efficient bitmap operations, we use the [CRoaring Library](https://github.com/RoaringBitmap/CRoaring).

---

### License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---

### Contact
For inquiries or feedback, please contact:
- **Ilyas Boudhaine**: [ilyasboudhaine1@gmail.com](mailto:your-email@example.com)
```
