# gsysc

## Introduction

This project is a fork from [GsysC](https://fameibl.de/file.php?doc=content/projects/gsysc&lang=en "GsysC") that was published under the conditions of the GNU General Public License (GPL).

## Prepare development environment

### Dependencies

- CMake (Version 3.14 or higher)
- SystemC (Version 2.3.2)
- Qt4 library

### Environment

Please create a environment variable `SYSTEMC_ROOT` that points to the root directory of your SystemC installation, for instance

```bash
export SYSTEMC_ROOT=/opt/systemc-2.3.2
```
### CMake

From root directory of gsysc type

```bash
cmake -b <build-directory-path> -DCMAKE_C_COMPILER=<compiler> -DCMAKE_CXX_COMPILER=<compiler> -DINCLUDE_TESTS=ON -G <generator>
```

- `build-directory-path`: Path to the build directory
- `compiler`: C and C++ compiler for instance clang or gcc
- `generator`: Target generator for instance make or ninja
- `DINCLUDE_TESTS=ON`: If this line is added to the confiuration call a small test project is added to the project for debugging purposes.
