#!/bin/bash -e

mkdir -pv src
for file in fortran/*.f; do
    f2c -s -C++ -dsrc $file
done
