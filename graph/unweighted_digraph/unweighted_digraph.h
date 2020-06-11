/**
 * TO_DO
 * 
 * @author Gabriel Nogueira (Talendar)
 */


#ifndef UNWEIGHTED_DIGRAPH_H
    #define UNWEIGHTED_DIGRAPH_H
    #include <stdbool.h>

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
    bool graph_has_vertex(Graph *g, int v);
    int graph_num_vertices(Graph *g);
    int graph_num_edges(Graph *g);
    int graph_adj_size(Graph *g);

    int* graph_vertices(Graph *g);
    int* graph_adj(Graph *g, int v);
    int graph_adj_count(Graph *g, int v);

    /* Others */
    void graph_print(Graph *g);
#endif