test: Makefile $(shell ls test.cpp custom/* src/*)
	# NOTE: must use either -ffloat-store or -mfpmath=sse to avoid dsaupd failures with info=-8
	gcc -g test.cpp custom/* src/* -lf2c -lm -lstdc++ -o test -msse2 -mfpmath=sse ${CXXFLAGS} ${CFLAGS} ${LDFLAGS}
