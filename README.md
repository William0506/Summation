# A Summation Formula for Algebraic Functions and Introduction to General Perfect Hypercomplex Algebras

## Project Overview

This repository contains the supplementary computational tools and implementations for the research project "A Summation Formula for Algebraic Functions and Some Introduction to General Perfect Hypercomplex Algebras", presented at the 2025 Taiwan International Science Fair.

The project explores novel summation formulas for algebraic functions and delves into the properties of General Perfect Hypercomplex Algebras (GPHA).

## Repository Structure and Implementation Details

### Python Implementation

The Python scripts are designed for flexibility and ease of use in research environments. They require Python 3.8+ and NumPy.

**Note**: Before running the Python scripts, you need to modify the file path in the first line of each script to match your local directory structure where the `B.txt` is located.

- `B.txt`: Precomputed Bernoulli number library (up to B_300) for efficient calculations.
- `SPHAmod.py`: Core module for SPHA computations. Implements SPHA operations.
- `Summation_R.py`, `Summation_C.py`: 
  - Calculate summations for Real and Complex powers respectively.
- `Summation_SPHA.py`：
  - Calculate summations for SPHA powers respectively。
  - Dependency：`SPHAmod.py`
- `Summation_mod_SPHA.py`: 
  - Standalone SPHA summation calculator.

### C++ Implementation

The C++ implementation offers high-performance computations, suitable for large-scale calculations. Requires C++17 compatible compiler.

- `B.cpp` / `B.exe`: 
  - Bernoulli number calculator using arbitrary-precision arithmetic.
- `B.txt`: Precomputed Bernoulli number library (up to B_1000).
- `SPHAmod.cpp` / `SPHAmod.h`: 
  - Header and implementation for SPHA operations.
  - Utilizes template metaprogramming for compile-time optimizations.
- `Summation_SPHA.cpp` / `Summation_SPHA.exe`:
  - SPHA summation calculator.
  - Dependency: `SPHAmod.h`
- `Summation_mod_SPHA.cpp` / `Summation_mod_SPHA.exe`:
  - Standalone SPHA summation calculator.

### Web Implementation

The web implementation provides an interactive interface for SPHA calculations, leveraging modern web technologies.

[Project Website](https://william0506.github.io/Summation/HTML/Summation.html)

- `SPHAmod.mjs`: 
  - ES module for SPHA operations.
  - Implements SPHA arithmetic and utility functions.
- `Summation.html` / `Summation.mjs`:
  - Web interface and main script for SPHA summation calculations.
  - Utilizes dynamic imports for modular code structure.

**Technical Notes:**
1. The web implementation uses ES6 modules, which require CORS (Cross-Origin Resource Sharing) to be enabled.
2. Dynamic imports are used for lazy-loading of modules, improving initial page load times.
3. The application must be served from a web server due to security restrictions on ES6 modules when loaded from `file://` URLs.

## Usage Instructions

### Python and C++
1. Ensure you have the required development environment (Python 3.8+ or C++17 compiler).
2. For C++, install the GMP library if using `B.cpp`.
3. Run the scripts/executables from the command line, providing necessary parameters.

### Web Interface
1. Access the web page through a web server.
2. Use a modern browser with ES6 module support (e.g., Chrome 61+, Firefox 60+, Safari 11+).
3. Input parameters in the provided fields and click "Calculate" to perform SPHA summations.
