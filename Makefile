# crude makefile for initial testing

all:
	gcc -shared -fPIC recess.c -o recess.so -ldl
	gcc test.c -o test

clean:
	rm -f recess.so
	rm -f test

run: all
	LD_PRELOAD=./recess.so ./test

.PHONY: all clean run