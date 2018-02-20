test: Makefile $(shell ls test.cpp custom/* src/*)
	gcc test.cpp custom/* src/* -lf2c -lm -lstdc++ -o test
