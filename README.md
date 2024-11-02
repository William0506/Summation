# A Summation Formula for Algebraic Functions and Introduction to General Perfect Hypercomplex Algebras

## Project Overview

This repository contains the supplementary computational tools and implementations for the research project "A Summation Formula for Algebraic Functions and Some Introduction to General Perfect Hypercomplex Algebras", presented at the 2025 Taiwan International Science Fair.

The project explores novel summation formulas for algebraic functions and delves into the properties of General Perfect Hypercomplex Algebras (GPHA).

## Repository Structure and Implementation Details

### Python Implementation

The Python scripts are designed for flexibility and ease of use in research environments. They require Python 3.8+ and NumPy.

**Important**: Before running the Python scripts, you need to modify the file path in the first line of each script to match your local directory structure where the `B.txt` is located.

- `B.txt`: Precomputed Bernoulli number library (up to B_300) for efficient calculations
- `SPHAmod.py`: Core module for SPHA computations. Implements SPHA operations
- `Summation_R.py`, `Summation_C.py`:
  - Calculate summations for Real and Complex powers respectively
- `Summation_SPHA.py`:
  - Calculates summations for SPHA powers
  - Dependency: `SPHAmod.py`
- `Summation_mod_SPHA.py`:
  - Standalone SPHA summation calculator

### C++ Implementation

C++ implementation provides high-performance computing capabilities, suitable for large-scale calculations. Requires C++17 compatible compiler.

- `B.cpp`:
  - Bernoulli number calculator using arbitrary-precision arithmetic
  - Dependency: GMP (GNU Multiple Precision Arithmetic Library)
- `B.txt`: Precomputed Bernoulli number library (up to B_1000)
- `SPHAmod.cpp` / `SPHAmod.h`:
  - Header and implementation for SPHA operations
  - Utilizes template metaprogramming for compile-time optimizations
- `Summation_SPHA.cpp` / `Summation_SPHA.exe`:
  - SPHA summation calculator
  - Dependency: `SPHAmod.h`
- `Summation_mod_SPHA.cpp` / `Summation_mod_SPHA.exe`:
  - Standalone SPHA summation calculator

### Web Implementation

Web implementation provides an interactive interface for SPHA calculations, utilizing modern web technologies.

[Project Website](https://william0506.github.io/Summation/HTML/Summation.html)

- `SPHAmod.mjs`: 
  - ES module for SPHA operations
  - Implements SPHA arithmetic and utility functions
- `Summation.html` / `Summation.mjs`:
  - Web interface and main script for SPHA summation calculations
  - Utilizes dynamic imports for modular code structure

## Technical Requirements

### Development Environment
- Python: Version 3.8 or higher with NumPy
- C++: C++17 compatible compiler and GMP library
- Web: Modern browser with ES6 module support (Chrome 61+, Firefox 60+, Safari 11+)

### Additional Notes
1. Web implementation requires CORS (Cross-Origin Resource Sharing) enabled
2. ES6 modules must be served from a web server (not `file://` URLs)
3. Dynamic imports are used for optimized module loading

## Usage Instructions

### Command-line Tools (Python and C++)
1. Set up the required development environment
2. Install necessary dependencies
3. For Python scripts: Update file paths in script headers
4. Execute scripts/programs with appropriate parameters

### Web Interface
1. Access through a modern web browser
2. Input parameters in the designated fields
3. Click "Calculate" to perform computations
