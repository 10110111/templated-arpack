test: Makefile templated-arpack.hxx test.cpp funcs.cpp f2c.h
	${CXX} -std=c++14 -pedantic-errors -Werror=implicit-function-declaration -g -I. test.cpp funcs.cpp templated-arpack.hxx -o test ${CXXFLAGS} ${CFLAGS} ${LDFLAGS}

templated-arpack.hxx: Makefile src combine-c-srcs.sh $(shell echo src/* | grep -v \\*)
	./combine-c-srcs.sh

src: Makefile run-f2c.sh fortran $(shell echo fortran/* | grep -v \\*)
	./run-f2c.sh
	touch src # make sure make sees it as up-to-date
