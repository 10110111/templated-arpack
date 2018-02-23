Templated ARPACK is a header-only version of ARPACK, relying on C++ templates to provide support for extended floating-point types.
Its main objective is to combine the power of arbitrary floating-point types (like e.g. `long double` or `boost::multiprecision::float128`) with the well-debugged algorithms from ARPACK. To achieve this, it uses `f2c` Fortran-to-C translator, does simple mechanical changes to the resulting sources and adds several helper functions.

Currently it's in **early alpha stage**. The only currently implemented functions are `dsaupd` and `dseupd` and their dependencies from LAPACK and ARPACK. Anything related to benchmarking, debug output and statistics is stubbed away (might be brought back later).

The library requires a compiler with C++14 support (e.g. g++-5 and clang-3.7).

To build the C++ header which provides all the functionality, you need Bash and F2C. After you make sure they are available, simply type `make`. You'll get `test` executable, which demonstrates the use of the library, and `templated-arpack.hxx` — the actual header you can `#include` in your project and use in the same way as you'd use the low-level interface of Fortran version of ARPACK.
