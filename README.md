# A Summation Formula for Algebraic Functions and Introduction to General Perfect Hypercomplex Algebras

## Project Overview

This repository contains the supplementary computational tools and implementations for the research project "A Summation Formula for Algebraic Functions and Some Introduction to General Perfect Hypercomplex Algebras", presented at the 2025 Taiwan International Science Fair.

The project explores novel summation formulas for algebraic functions and delves into the properties of General Perfect Hypercomplex Algebras (GPHA), with a focus on Symmetric Perfect Hypercomplex Algebras (SPHA).

## Repository Structure and Implementation Details

### Python Implementation

The Python scripts are designed for flexibility and ease of use in research environments. They require Python 3.8+ and NumPy.

- `B.txt`: Precomputed Bernoulli number library (up to B_300) for efficient calculations.
- `SPHAmod.py`: Core module for SPHA computations. Implements SPHA operations and provides utility functions.
- `Summation_R.py`, `Summation_C.py`, `Summation_SPHA.py`: 
  - Calculate summations for Real, Complex, and SPHA powers respectively.
  - Dependency: `SPHAmod.py`
  - Usage: `python Summation_X.py <parameters>`
- `Summation_mod_SPHA.py`: 
  - Standalone SPHA summation calculator.
  - Usage: `python Summation_mod_SPHA.py <parameters>`

### C++ Implementation

The C++ implementation offers high-performance computations, suitable for large-scale calculations. Requires C++17 compatible compiler.

- `B.cpp` / `B.exe`: 
  - Bernoulli number calculator using arbitrary-precision arithmetic.
  - Dependency: GMP (GNU Multiple Precision Arithmetic Library)
- `B.txt`: Precomputed Bernoulli number library.
- `SPHAmod.cpp` / `SPHAmod.h`: 
  - Header and implementation for SPHA operations.
  - Utilizes template metaprogramming for compile-time optimizations.
- `Summation_SPHA.cpp` / `Summation_SPHA.exe`:
  - SPHA summation calculator.
  - Dependency: `SPHAmod.h`
  - Compilation: `g++ -std=c++17 -O3 Summation_SPHA.cpp -o Summation_SPHA`
- `Summation_mod_SPHA.cpp` / `Summation_mod_SPHA.exe`:
  - Standalone SPHA summation calculator.
  - Compilation: `g++ -std=c++17 -O3 Summation_mod_SPHA.cpp -o Summation_mod_SPHA`

### Web Implementation

The web implementation provides an interactive interface for SPHA calculations, leveraging modern web technologies.

URL: https://william0506.github.io/Summation/HTML/Summation.html

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
1. Access the web page through a web server (local or remote).
2. Use a modern browser with ES6 module support (e.g., Chrome 61+, Firefox 60+, Safari 11+).
3. Input parameters in the provided fields and click "Calculate" to perform SPHA summations.

## Development and Contribution

This project is part of ongoing research in algebraic function summation and hypercomplex algebras. Contributions are welcome, particularly in the following areas:

1. Performance optimizations for large-scale computations.
2. Extension of the algorithms to other types of hypercomplex algebras.
3. Improved visualization of SPHA operations and results.

To contribute:
1. Fork the repository.
2. Create a feature branch (`git checkout -b feature/AmazingFeature`).
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`).
4. Push to the branch (`git push origin feature/AmazingFeature`).
5. Open a Pull Request.
