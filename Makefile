test: Makefile $(shell ls test.cpp custom/* src/*)
	gcc -g test.cpp custom/* src/* -lf2c -lm -lstdc++ -o test ${CXXFLAGS} ${CFLAGS} ${LDFLAGS}
