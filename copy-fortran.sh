#!/bin/bash -e

arpackDir=$HOME/src/arpack-ng
lapackDir=$HOME/src/lapack-3.8.0
if ! [[ -d "$arpackDir" ]]; then
    echo "ARPACK directory not found" >&2
    exit 1
fi
if ! [[ -d "$lapackDir" ]]; then
    echo "LAPACK directory not found" >&2
    exit 1
fi

files=(daxpy dcopy ddot dgemv dgetv0 dlarnv dlaruv dlascl dlartg dlaset dlanst dlaev2 dlae2 dlapy2 dlasr dlasrt dlassq dnrm2 dsaitr dstqrb dsapps dsaup2 dsaupd dseupd dscal dsconv dseigt dsgets dsortr dstats dswap dsteqr dsesrt dgeqr2 dorm2r dlacpy dger dlarf dlarfg iladlc iladlr) # dlamch xerbla dvout dmout ivout second)

mkdir -pv fortran
for func in "${files[@]}"
do
    find "$arpackDir" "$lapackDir"  -name "$func.f" -print -exec cp {} fortran/ \;
done
cp "$lapackDir/BLAS/SRC/lsame.f" "$arpackDir/SRC/stat.h" "$arpackDir/SRC/debug.h" fortran/
