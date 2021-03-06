CXXFLAGS=-O3 -march=native

test: Makefile templated-arpack.hxx test.cpp
	${CXX} -std=c++14 -pedantic-errors -Werror=implicit-function-declaration -g test.cpp -o test ${CXXFLAGS} ${LDFLAGS}

templated-arpack.hxx: Makefile funcs.cpp includes-begin.h src combine-c-srcs.sh $(shell echo src/* | grep -v \\*)
	./combine-c-srcs.sh

src: Makefile run-f2c.sh fortran $(shell echo fortran/* | grep -v \\*)
	./run-f2c.sh
	touch src # make sure make sees it as up-to-date

clean:
	rm -rf src-before-cat src templated-arpack.hxx test
.PHONY: clean
