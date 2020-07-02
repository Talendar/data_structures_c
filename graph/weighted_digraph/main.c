/**
 * Simple program to test my weighted digraph implementation. 
 * To help with the tests, the following websites were used (access on Jul 1, 2020): 
 *      https://visualgo.net/en/graphds 
 *      https://www.cs.usfca.edu/~galles/visualization/Dijkstra.html
 * 
 * @author Gabriel Nogueira (Talendar)
 */


#include <stdio.h>
#include <stdlib.h>
#include "weighted_digraph.h"
#include "singly_linked_list.h"
#include "shortest_paths.h"


/**
 * Auxiliary function to print an edge's destination vertex.
 */
void print_edge_head(void *edge) {
    printf(" -> %d", edge_dest(edge));
}


/**
 * List of commands:
 * 
 *      0        - exits the program
 *      1 s d w  - adds the edge s->d with weight w to the graph; if either the vertex s or the vertex d doesn't exist, it's created;
 *      2 v w    - removes the edge v->w from the graph
 *      3 v      - adds the vertex v to the graph
 *      4 v      - removes the vertex v from the graph
 *      5        - prints informations about the graph (number of vertices and edges, etc)
 *      6        - prints the adjacency list of all the graph's vertices
 *      7 s v     - prints the single source shortest path from s to v
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
                int s, d;  double w;
                scanf(" %d %d %lf", &s, &d, &w);
                graph_add_edge(g, s, d, w, true);
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
                printf("INFO: { |V| = %d   <>   |E| = %d   <>   adj_lists_array_size = %d }\n", 
                                graph_num_vertices(g), graph_num_edges(g), graph_array_size(g));
            }
            // [6] PRINT ADJACENCY LISTS
            else if(opt == 6) {
                printf("\n--------------------------------------------\n");
                graph_print(g);
                printf("--------------------------------------------\n\n");
            }
            // [7] SSSP
            else if(opt == 7) {
                int s, v;  scanf(" %d %d", &s, &v);
                SPT *spt = dijkstra_sp(g, s);

                printf("\nspt->source = %d  |  spt->size = %d\n", 
                        spt_source(spt), spt_size(spt));
                printf("HAS PATH: %d  |  PATH WEIGHT: %.2lf\n", 
                        spt_has_path(spt, v), spt_path_dist(spt, v));
                
                List *path = spt_path_to(spt, v);
                if(path != NULL) {
                    printf("PATH: { %d", s);
                    list_print(path, &print_edge_head);
                    list_free(&path, &free);
                    printf(" }\n");
                }
                printf("\n");
                spt_free(&spt);
            }
        } while(opt != 0);
        
        graph_free(&g);
    }
    return 0;
}
