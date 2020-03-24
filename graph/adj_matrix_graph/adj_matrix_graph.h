/*
 * Adjacency matrix implementation of an undirected graph.
 * Created by Gabriel Nogueira (Talendar)
 */

#ifndef ADJ_MATRIX_GRAPH_H
    #define ADJ_MATRIX_GRAPH_H
    typedef struct Graph Graph;

    Graph* graph_create(int v);
    void graph_free(Graph **g);

    void graph_insert_edge(Graph *g, int v1, int v2);
    void graph_remove_edge(Graph *g, int v1, int v2);
    void graph_print(Graph *g);
#endif