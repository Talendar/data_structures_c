run: program
	./program

all: clean main.o singly_linked_list.o weighted_digraph.o shortest_paths.o
	gcc -lm singly_linked_list.o weighted_digraph.o shortest_paths.o main.o -o program

main.o: main.c
	gcc -c main.c

singly_linked_list.o: singly_linked_list.c singly_linked_list.h
	gcc -c singly_linked_list.c

weighted_digraph.o: weighted_digraph.c weighted_digraph.h
	gcc -c weighted_digraph.c
	
shortest_paths.o: shortest_paths.c shortest_paths.h
	gcc -c shortest_paths.c

clean:
	rm -rf *.o program
