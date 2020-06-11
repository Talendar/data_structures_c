run: program
	./program

all: clean main.o singly_linked_list.o unweighted_digraph.o
	gcc singly_linked_list.o unweighted_digraph.o main.o -o program

main.o: main.c
	gcc -c main.c

singly_linked_list.o: singly_linked_list.c singly_linked_list.h
	gcc -c singly_linked_list.c

unweighted_digraph.o: unweighted_digraph.c unweighted_digraph.h
	gcc -c unweighted_digraph.c

clean:
	rm -rf *.o program
