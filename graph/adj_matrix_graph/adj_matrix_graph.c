/*
 * Adjacency matrix implementation of an undirected graph.
 * Created by Gabriel Nogueira (Talendar)
 */

#include "adj_matrix_graph.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Basic structure of a graph.
 */
struct Graph {
    int **adj_matrix; //adjancy matrix containing the edges of the graph
    int v; //number of vertices
};

/*
 * Creates a graph with "v" vertices and returns a pointer to it. Returns NULL if the required memory couldn't be allocated.
 */
Graph* graph_create(int v)
{
    Graph *g = malloc(sizeof(Graph));
    if(g != NULL) {
        g->v = v;
        g->adj_matrix = malloc(v * sizeof(int*));

        if(g->adj_matrix != NULL) {
            for(int i = 0; i < v; i++) {
                g->adj_matrix[i] = calloc(v, sizeof(int));
                if(g->adj_matrix[i] == NULL) {
                    graph_free(&g);
                    break;
                }
            }
        }
        else
            graph_free(&g);
    }

    return g;
}

/*
 * Frees the memory allocated by the given graph. Expects a pointer to the variable that holds a pointer to the graph. That variable will be set to NULL.
 */
void graph_free(Graph **g) 
{
    for(int i = 0; i < (*g)->v; i++) 
        if((*g)->adj_matrix[i] != NULL)
            free((*g)->adj_matrix[i]);
    
    free((*g)->adj_matrix);
    free((*g));
    (*g) = NULL;
}

/*
 * Inserts an undirected edge connecting the node i with the node j.
 */
void graph_insert_edge(Graph *g, int i, int j) {   
    g->adj_matrix[i][j] = g->adj_matrix[j][i] = 1;
}

/* 
 * Removes the undirected edge connecting the nodes i and j.
 */
void graph_remove_edge(Graph *g, int i, int j) {
    g->adj_matrix[i][j] = g->adj_matrix[j][i] = 0;
}

/*
 * Prints the adjacency matrix of the given graph.
 */
void graph_print(Graph *g) {
    for(int i = 0; i < g->v; i++) {
        for(int j = 0; j < g->v; j++)
            printf("%d ", g->adj_matrix[i][j]);
        printf("\n");
    }
    printf("\n");
}