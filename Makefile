test: Makefile src.c test.cpp funcs.cpp
	# NOTE: must use either -ffloat-store or -mfpmath=sse to avoid dsaupd failures with info=-8
	gcc -g test.cpp funcs.cpp src.c -lm -lstdc++ -o test -msse2 -mfpmath=sse ${CXXFLAGS} ${CFLAGS} ${LDFLAGS}

src.c: Makefile src make-c-cat-friendly-and-cat.sh $(shell echo src/* | grep -v \\*)
	./make-c-cat-friendly-and-cat.sh

src: Makefile run-f2c.sh
	./run-f2c.sh
	touch src # make sure make sees it as up-to-date
