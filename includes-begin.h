#ifndef TEMPLATED_ARPACK_E2EA3165_F54C_41CB_8C66_523EDDCD97B5
#define TEMPLATED_ARPACK_E2EA3165_F54C_41CB_8C66_523EDDCD97B5

typedef long int integer;
typedef long int ftnlen;
typedef long int logical;

#include <math.h> // to make all the math functions with all their overloads appear in global namespace (g++ doesn't put some overloads from <cmath> into global namespace)
#include <cmath> // for std::abs, because ::abs doesn't appear to be defined in math.h when compiling with clang
#include <limits> // for dlamch_
#include <iostream> // for xerbla_()

