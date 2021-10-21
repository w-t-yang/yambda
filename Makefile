CFLAGS=-std=gnu99 -g -O2 -Wall

.PHONY: clean test

yambda: clean input.o output.o env.o utils.o
	gcc -o yambda yambda.c input.o output.o env.o utils.o

input.o: src/input.c
	gcc -c src/input.c

output.o: src/output.c
	gcc -c src/output.c

env.o: src/env.c
	gcc -c src/env.c

utils.o: src/utils.c
	gcc -c src/utils.c

primitives.o: src/primitives.c
	gcc -c src/primitives.c

clean:
	rm -f yambda *.o *~

test: yambda
	@./test.sh
