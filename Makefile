test: Makefile $(shell ls custom/* src/*)
	gcc custom/* src/* -lf2c -lm -lstdc++ -o test
