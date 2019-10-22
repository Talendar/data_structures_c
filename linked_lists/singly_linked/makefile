run: program
	./program

all: clean main.o singly_linked_list.o
	gcc main.o singly_linked_list.o -o program

main.o: main.c
	gcc -c main.c

singly_linked_list.o: singly_linked_list.c singly_linked_list.h
	gcc -c singly_linked_list.c

clean:
	rm -rf *.o program
