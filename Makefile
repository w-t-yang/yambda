CFLAGS=-std=gnu99 -g -O2 -Wall

.PHONY: clean test

yambda: clean input.o output.o env.o utils.o funcs.o
	gcc -o yambda yambda.c input.o output.o env.o utils.o math.o prim.o elmt.o funcs.o
ydebug: clean input.o output.o env.o utils.o funcs.o
	gcc -o ydebug ydebug.c input.o output.o env.o utils.o math.o prim.o elmt.o funcs.o
ytest: clean input.o output.o env.o utils.o funcs.o
	gcc -o ytest ytest.c input.o output.o env.o utils.o math.o prim.o elmt.o funcs.o

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
elmt.o: src/funcs/elmt.c
	gcc -c src/funcs/elmt.c
funcs.o: math.o prim.o elmt.o
	gcc -c src/funcs.c

clean:
	rm -f yambda ydebug ytest *.o *~

test: yambda
	@./test.sh
