run: program
	./program

all: clean main.o dynamic_queue.o
	gcc dynamic_queue.o main.o -o program

main.o: main.c
	gcc -c main.c

static_stack.o: static_queue.c static_queue.h
	gcc -c static_queue.c

dynamic_stack.o: dynamic_queue.c dynamic_queue.h
	gcc -c dynamic_queue.c

clean:
	rm -rf *.o program
