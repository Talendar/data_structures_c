run: program
	./program

all: clean main.o dynamic_stack.o
	gcc dynamic_stack.o main.o -o program -lm

main.o: main.c
	gcc -c main.c

static_stack.o: static_stack.c static_stack.h
	gcc -c static_stack.c

dynamic_stack.o: dynamic_stack.c dynamic_stack.h
	gcc -c dynamic_stack.c

clean:
	rm -rf *.o program
