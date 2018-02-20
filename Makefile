test: Makefile src.c $(shell echo test.cpp custom/*)
	# NOTE: must use either -ffloat-store or -mfpmath=sse to avoid dsaupd failures with info=-8
	gcc -g test.cpp custom/* src.c -lf2c -lm -lstdc++ -o test -msse2 -mfpmath=sse ${CXXFLAGS} ${CFLAGS} ${LDFLAGS}

src.c: Makefile make-c-cat-friendly-and-cat.sh run-f2c.sh $(shell echo src/*)
	./run-f2c.sh
	./make-c-cat-friendly-and-cat.sh
