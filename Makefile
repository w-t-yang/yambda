CFLAGS=-std=gnu99 -g -O2 -Wall

.PHONY: clean test

yambda: d.input.o d.output.o
	gcc -o yambda yambda.c input.o output.o -DDEBUG=1

d.input.o: src/input.c
	gcc -c src/input.c -DDEBUG=1

d.output.o: src/output.c
	gcc -c src/output.c -DDEBUG=1


release: clean input.o output.o
	gcc -o yambda yambda.c input.o output.o

input.o: src/input.c
	gcc -c src/input.c

output.o: src/output.c
	gcc -c src/output.c


clean:
	rm -f yambda *.o *~

test: yambda
	@./test.sh
