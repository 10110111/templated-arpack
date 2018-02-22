test: Makefile src.cpp test.cpp funcs.cpp f2c.h
	# NOTE: must use either -ffloat-store or -mfpmath=sse to avoid dsaupd failures with info=-8
	${CXX} -Werror=implicit-function-declaration -g -I. test.cpp funcs.cpp src.cpp -o test -msse2 -mfpmath=sse ${CXXFLAGS} ${CFLAGS} ${LDFLAGS}

src.cpp: Makefile src make-c-cat-friendly-and-cat.sh $(shell echo src/* | grep -v \\*)
	./make-c-cat-friendly-and-cat.sh

src: Makefile run-f2c.sh $(shell echo fortran/* | grep -v \\*)
	./run-f2c.sh
	touch src # make sure make sees it as up-to-date
