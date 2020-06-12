/**
 * Implementation of an unweighted directed graph. 
 * 
 * Each vertex is identified by it's index in the graph's array of adjacency lists. This array's initial size can be chosen by the client (alternatively, default values can be used, hiding the internal details from the client). Note that when a vertex v has an ID greater than the graph's array of adjacency lists, the array must be expanded (memory reallocation). This might be improved later on through the use of hashing.
 * 
 * @todo function to shrink the graph's array of adjacency lists to a desired size.
 * @todo reduce, by the use of hashing, the memory required by the graph to store its adjacency lists.
 * 
 * @version 1.0
 * @author Gabriel Nogueira (Talendar)
 */


#ifndef UNWEIGHTED_DIGRAPH_H
    #define UNWEIGHTED_DIGRAPH_H
    #include <stdbool.h>
    #include "singly_linked_list.h"

    /* Constants */
    static const int ADJ_LISTS_ARRAY_INITIAL_SIZE = 20;        // the initial size of a graph's adjacency lists array
    static const int ADJ_LISTS_ARRAY_DELTA_REALLOC = 10;       // how much a graph's adjacency lists array will grow in each realloc

    /* Structs */
    typedef struct UnweightedDigraph Graph;

    /* Create/Free */
    Graph* graph_create_full(int initial_size, int delta_realloc);
    Graph* graph_create();
    void graph_free(Graph **g);

    /* Insertions */
    bool graph_add_vertex(Graph *g, int v);
    bool graph_add_edge(Graph *g, int v, int w, bool create_if_needed);

    /* Removals */
    bool graph_remove_vertex(Graph *g, int v);
    bool graph_remove_edge(Graph *g, int v, int w);

    /* Queries */
    int graph_num_vertices(Graph *g);
    int graph_num_edges(Graph *g);
    int graph_array_size(Graph *g);

    bool graph_has_vertex(Graph *g, int v);
    bool graph_has_cycle(Graph *g);
    List* graph_find_sources(Graph *g);

    int* graph_vertices(Graph *g);
    int* graph_adj_to(Graph *g, int v);
    int graph_adj_count(Graph *g, int v);

    /* Others */
    void graph_print(Graph *g);
#endif