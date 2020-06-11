/**
 * Simple program to test my digraph implementation.
 * @author Gabriel Nogueira (Talendar)
 */


#include <stdio.h>
#include <stdlib.h>
#include "unweighted_digraph.h"


/**
 * List of commands:
 * 
 *      0       - exits the program
 *      1 v w   - adds the edge v->w to the graph; if either the vertex v or the vertex w doesn't exit, it's created
 *      2 v w   - removes the edge v->w from the graph
 *      3 v     - adds the vertex v to the graph
 *      4 v     - removes the vertex v from the graph
 *      5       - prints informations about the graph (number of vertices and edges, etc)
 *      6       - prints the adjacency list of all the graph's vertices
 *      
 */
int main(void) 
{
    Graph *g = graph_create();
    if(g != NULL) {
        int opt = -1;
        do {
            scanf(" %d", &opt);

            // [1] ADD EDGE
            if(opt == 1) {
                int v, w;  scanf(" %d %d", &v, &w);
                graph_add_edge(g, v, w, true);
            }
            // [2] REMOVE EDGE
            else if(opt == 2) {
                int v, w;  scanf(" %d %d", &v, &w);
                graph_remove_edge(g, v, w);
            }
            // [3] ADD VERTEX
            else if(opt == 3) {
                int v; scanf(" %d", &v);
                graph_add_vertex(g, v);
            }
            // [4] REMOVE VERTEX
            else if(opt == 4) {
                int v; scanf(" %d", &v);
                graph_remove_vertex(g, v);
            }
            // [5] PRINT INFO
            else if(opt == 5) {
                printf("INFO: { |V| = %d  \\   |E| = %d   \\   adj_lists_array_size = %d }\n", graph_num_vertices(g), graph_num_edges(g), graph_adj_size(g));
            }
            // [6] PRINT ADJACENCY LISTS
            else if(opt == 6) {
                printf("\n--------------------------------------------\n");
                graph_print(g);
                printf("--------------------------------------------\n\n");
            }

        } while(opt != 0);
        
        graph_free(&g);
    }
    return 0;
}