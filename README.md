# Linear Algebra

> How I wish linear algebra was taught.

## Table of Contents

- [Motivation](#motivation)
- [Features](#features)
- [Concepts Covered](#concepts-covered)
- [Repository Structure](#repository-structure)
- [Getting Started](#getting-started)
  - [Build Instructions](#build-instructions)
  - [Running the Application](#running-the-application)
  - [Web Version](#web-version)
  - [Docker Support](#docker-support)
- [Usage Examples](#usage-examples)
- [Extending the Project](#extending-the-project)
- [Contributing](#contributing)
- [License](#license)

---

## Motivation

Linear algebra is foundational to mathematics, physics, engineering, computer science, machine learning, and graphics. However, it’s often taught in a way that makes it abstract and intimidating. This project aims to bridge the gap between theory and application, making linear algebra more interactive, visual, and approachable for learners and practitioners.

---

## Features

- **Comprehensive linear algebra library** in C/C++ with clear, readable code.
- **Multiple build targets:** Native, WebAssembly (via Emscripten), and Docker.
- **Interactive web demos** (if present in `/web`) for visualizing vectors, transformations, and more.
- **Educational comments and documentation** throughout the codebase.
- **Unit tests** (if `/tests` present) to ensure reliability and demonstrate usage.
- **Modular design** for easy extension and experimentation.

---

## Concepts Covered

- **Vectors:** Representation, operations (addition, scalar multiplication, dot/cross product), linear independence.
- **Matrices:** Creation, arithmetic, transposition, inversion, special matrices (identity, diagonal, etc.).
- **Determinants & Rank:** Calculation and interpretation.
- **Systems of Linear Equations:** Solving via Gaussian elimination, LU decomposition.
- **Eigenvalues & Eigenvectors:** Computation and geometric meaning.
- **Linear Transformations:** Geometric interpretation of matrices (rotation, scaling, projection).
- **Applications:** Examples in graphics, physics, and data science (PCA, transformations).

---

## Repository Structure

```
/src          - Core C/C++ source files
/web          - Web server and static files (for WebAssembly demo)
/tests        - Unit and integration tests (if present)
CMakeLists.txt - Build configuration
Dockerfile    - Docker containerization
README.md     - This file
```

---

## Getting Started

### Build Instructions

#### Native Build

Requires: `cmake`, `make`, and a C compiler

```sh
mkdir build
cd build
cmake ..
make
./linear_algebra
```

#### Web Build (WebAssembly via Emscripten)

Requires: [emsdk](https://emscripten.org/docs/getting_started/downloads.html)

```sh
mkdir build-web
cd build-web
emcmake cmake -DPLATFORM=Web --fresh ..
make
```

#### Docker Support

```sh
docker build -t linear-algebra:latest .
docker run -p 8080:8080 linear-algebra
```

---

### Running the Application

- **CLI (Native):** After building, run `./linear_algebra` in your build directory.
- **Web:** Use the Go server in `/web` to host the WebAssembly files:
  ```sh
  cd web
  go run main.go
  ```
  Then open [http://localhost:8080](http://localhost:8080) in your browser.

---

## Usage Examples

- **Solving a Linear System:**
  ```c
  // Example: Solve Ax = b
  Matrix A = matrix_create(3, 3, ...);
  Vector b = vector_create(3, ...);
  Vector x = solve_linear_system(A, b);
  print_vector(x);
  ```

- **Calculating Eigenvalues:**
  ```c
  Matrix M = matrix_create(2, 2, ...);
  EigenResult result = compute_eigen(M);
  print_eigenvalues(result);
  print_eigenvectors(result);
  ```

- **Visualizing Transformations:**
  - (Web version) Visit the web interface to interactively see how matrices transform 2D/3D space.

---

## Extending the Project

Contributions are encouraged! You can add:
- New linear algebra operations or algorithms
- Interactive demos (especially in the web interface)
- More tests and example applications
- Improved documentation or tutorials

See [CONTRIBUTING.md](CONTRIBUTING.md) if available.

---

## Learning Outcomes

By exploring this repository, you will:
- Gain practical experience implementing and using linear algebra algorithms.
- Develop intuition by visualizing transformations and operations.
- Learn best practices for modular, testable scientific code in C/C++.
- See how mathematical concepts map directly to real code.

---

*Feel free to reach out or open issues for questions, suggestions, or improvements!*
