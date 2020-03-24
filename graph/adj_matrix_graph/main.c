/*
 * Simple program to test an adjacency matrix implementation of an undirected graph.
 * Created by Gabriel Nogueira (Talendar)
 */

#include <stdio.h>
#include "adj_matrix_graph.h"

int main(void)
{
    //preparing the graph
    int nodes, edges;  scanf(" %d %d", &nodes, &edges);
    Graph *g = graph_create(nodes);
    
    for(int i = 0; i < edges; i++) {
        int i, j;  scanf(" %d %d", &i, &j);
        graph_insert_edge(g, i, j);
    }

    //testing the graph
    int num_ops;  scanf(" %d", &num_ops);
    for(int i = 0; i < num_ops; i++) {
        int op;  scanf(" %d", &op);
        if(op == 1 || op == 2) {
            int i, j;  scanf(" %d %d", &i, &j);
            if(op == 1)
                graph_insert_edge(g, i, j);
            else 
                graph_remove_edge(g, i, j);
        } 
        else if(op == 3)
            graph_print(g);
    }

    //freeing the memory allocated by the graph
    graph_free(&g);
    return 0;
}