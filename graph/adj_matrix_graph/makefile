run: program
	./program

all: clean main.o adj_matrix_graph.o
	gcc adj_matrix_graph.o main.o -o program

main.o: main.c
	gcc -c main.c

adj_matrix_graph.o: adj_matrix_graph.c adj_matrix_graph.h
	gcc -c adj_matrix_graph.c

clean:
	rm -rf *.o program