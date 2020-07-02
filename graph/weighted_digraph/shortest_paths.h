/**
 * Simple API for resolving single-source shortest paths problems.
 * 
 * Example of use:
 *      SPT *spt = dijkstra_sp(g, s);      // shortest paths tree of the graph g with the vertex s as the root
 *      List *path = spt_path_to(spt, v);  // shortest path from s to v
 * 
 * @todo: change the Dijkstra's algorithm implementation to use a heap-based
 * priority queue.
 * @todo: implement the Bellman-Ford algorithm (deals with negative edges weights).
 * 
 * @version 1.0
 * @author Gabriel Nogueira (Talendar)
 */


#ifndef SHORTEST_PATH_H
    #define SHORTEST_PATH_H
    #include "weighted_digraph.h"
    #include "singly_linked_list.h"
    #include <stdbool.h>

    /* Structs */
    typedef struct ShortestPathsTree SPT;

    /* Memory freers */
    void spt_free(SPT **spt);

    /* Pathfinders */
    SPT* dijkstra_sp(Graph *g, int s);

    /* Queries */
    int spt_source(SPT *spt);
    int spt_size(SPT *spt);

    bool spt_has_path(SPT *spt, int v);
    List* spt_path_to(SPT *spt, int v);
    double spt_path_dist(SPT *spt, int v);
#endif