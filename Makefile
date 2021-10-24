CFLAGS=-std=gnu99 -g -O2 -Wall

.PHONY: clean test

yambda: clean input.o output.o env.o funcs.o utils.o
	gcc -o yambda yambda.c input.o output.o env.o math.o prim.o funcs.o utils.o

ydebug: clean input.o output.o env.o funcs.o utils.o
	gcc -o ydebug ydebug.c input.o output.o env.o math.o prim.o funcs.o utils.o

input.o: src/input.c
	gcc -c src/input.c
output.o: src/output.c
	gcc -c src/output.c
env.o: src/env.c
	gcc -c src/env.c
utils.o: src/utils.c
	gcc -c src/utils.c


math.o: src/funcs/math.c
	gcc -c src/funcs/math.c
prim.o: src/funcs/prim.c
	gcc -c src/funcs/prim.c
funcs.o: math.o prim.o
	gcc -c src/funcs.c

clean:
	rm -f yambda ydebug *.o *~

test: yambda
	@./test.sh
