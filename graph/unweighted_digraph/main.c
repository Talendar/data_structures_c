/**
 * Simple program to test my digraph implementation. 
 * To help with the tests, the following website was used: https://visualgo.net/en/graphds (access on June 11, 2020).
 * 
 * @author Gabriel Nogueira (Talendar)
 */


#include <stdio.h>
#include <stdlib.h>
#include "unweighted_digraph.h"
#include "singly_linked_list.h"


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
 *      7       - prints all the source vertices of the graph
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
                printf("INFO: { |V| = %d  <>   |E| = %d   <>   DAG: %d   <>   adj_lists_array_size = %d }\n", 
                                graph_num_vertices(g), graph_num_edges(g), !graph_has_cycle(g), graph_array_size(g));
            }
            // [6] PRINT ADJACENCY LISTS
            else if(opt == 6) {
                printf("\n--------------------------------------------\n");
                graph_print(g);
                printf("--------------------------------------------\n\n");
            }
            // [7] PRINT SOURCE VERTICES
            else if(opt == 7) {
                List *sources = graph_find_sources(g);
                if(sources != NULL) {
                    printf("SOURCE VERTICES: ");
                    while(!list_empty(sources)) {
                        int *v = list_pop(sources);
                        printf(" %d  ", *v);
                        free(v);
                    }
                    printf("\n\n");
                }
                else 
                    printf("NO SOURCE VERTICES.\n\n");
                
                list_free(&sources, NULL);
            }

        } while(opt != 0);
        
        graph_free(&g);
    }
    return 0;
}