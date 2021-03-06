/**
 * Implementation of a weighted directed graph. 
 * 
 * Each vertex is identified by it's index in the graph's array of adjacency lists. 
 * Each adjacency list will hold all the edges (with their destinations and weights)
 * leaving the associated vertex. This array's initial size can be chosen by the 
 * client (alternatively, default values can be used, hiding the internal details 
 * from the client). Note that when a vertex v has an ID greater than the graph's 
 * array of adjacency lists, the array must be expanded (memory reallocation) in
 * order to accommodate v. This might be improved later on through the use of hashing.
 * 
 * @todo function to shrink the graph's array of adjacency lists to a desired size.
 * @todo reduce, by the use of hashing, the memory required by the graph to store 
 * its adjacency lists.
 * 
 * @version 1.0
 * @author Gabriel Nogueira (Talendar)
 */


#include "weighted_digraph.h"
#include "singly_linked_list.h"
#include <stdlib.h>
#include <stdio.h>


/**
 *      General structure of a weighted digraph implemented with adjacency lists 
 * that stores edges connecting each pair of adjacent vertices. An array is used 
 * to store the adjacency lists of each of the graph's vertices. Once the array 
 * is full, it's reallocated in order to grow in size.
 * 
 * Attributes:
 *      . adj_lists: array of adjacency lists (lists of edges leaving each vertex); 
 *      each index represents a vertex in the graph; if adj_lists[i] is NULL, then
 *      the vertex i is not in the graph.
 *      . adj_size: the current size of adj_lists.
 *      . delta_realloc: how much adj_lists will grow in each realloc.
 *      . num_vertices: number of vertices in the graph.
 *      . num_edges: number of edges in the graph (parallel edges do NOT count as 
 *      a single edge).
 * 
 */
struct WeightedDigraph {
    List **adj_lists;       
    int adj_size, delta_realloc, 
        num_vertices, num_edges;          
};


/**
 * General structure of a directed and weighted edge of a graph. 
 * 
 * Attributes:
 *      . from: the edge's tail.
 *      . to: the edge's head.
 *      . weight: the edge's weight.
 */
struct DirectedWeightedEdge {
    int from, to;
    double weight;
};


/**
 *      Creates a new empty weighted digraph and returns a pointer to it. With 
 * this function, the caller can specify the initial size of the graph's array 
 * of adjacency lists, as well as how much it will grow in each realloc.
 * 
 * @param initial_size initial size of the graph's adjacency lists array (once 
 * it's full, realloc will be called to increase its size).
 * @param delta_realloc defines how much the graph's adjacency lists array will 
 * grow in each realloc.
 * @return a pointer to the newly created graph if all the required memory could 
 * be allocated; NULL otherwise.
 */
Graph* graph_create_full(int initial_size, int delta_realloc) 
{
    Graph *g = malloc(sizeof(Graph));
    if(g != NULL) {
        g->adj_lists = malloc(initial_size * sizeof(List*));

        if(g->adj_lists != NULL) {
            for(int i = 0; i < initial_size; i++)
                g->adj_lists[i] = NULL;     // initialising pointers as NULL

            g->adj_size = initial_size;
            g->delta_realloc = delta_realloc;
            g->num_vertices = g->num_edges = 0;
        }
        else {
            free(g);
            g = NULL;
        }
    }

    return g;
}


/**
 *      Creates a new empty weighted digraph and returns a pointer to it. Wrapper 
 * for the function graph_create_full(). Exempts the user from having to choose 
 * an initial size and a growing size for the graph's array of adjacency lists 
 * (default values are used).
 * 
 * @return a pointer to the newly created graph if all the required memory could 
 * be allocated; NULL otherwise.
 */
Graph* graph_create() {
    return graph_create_full(ADJL_ARRAY_INITIAL_SIZE, ADJL_ARRAY_DELTA_REALLOC);
}


/**
 * Frees the memory allocated by the graph (including its edges). 
 * 
 * @param g a double pointer to the graph; by the end of the execution, the 
 * variable pointed by g will be set to NULL.
 */
void graph_free(Graph **g) {
    for(int i = 0; i < (*g)->adj_size; i++) {
        if((*g)->adj_lists[i] != NULL)
            list_free(&((*g)->adj_lists[i]), &free);
    }

    free((*g)->adj_lists);
    free((*g));
    (*g) = NULL;
}


/**
 * Increases the size of a graph's array of adjacency lists by g->delta_realloc.
 * 
 * @param g a pointer to the graph.
 * @param num the array will be increased by num*g->delta_realloc.
 * @return true if the operation was successful; false if the memory couldn't be 
 * reallocated (in this case, g->adj_lists will remain unchanged).
 */
static bool graph_grow(Graph *g, int num) 
{
    int new_size = g->adj_size + num*g->delta_realloc;
    List **new_arr = realloc(g->adj_lists, new_size*sizeof(List*));

    if(new_arr == NULL)
        return false;    // realloc failed

    for(int i = g->adj_size; i < new_size; i++)
        new_arr[i] = NULL;

    g->adj_lists = new_arr;
    g->adj_size = new_size;
    return true;
}


/**
 * Checks whether the vertex v is in the graph g.
 * 
 * @param g pointer to a graph.
 * @param v index that identifies the vertex.
 * @return true if v is in g and false otherwise.
 */
bool graph_has_vertex(Graph *g, int v) {
    if(v < 0 || v >= g->adj_size)   // checking if the index is out of bounds
        return false;
    return g->adj_lists[v] != NULL;
}


/**
 * Returns the number of vertices in g.
 */
int graph_num_vertices(Graph *g) {
    return g->num_vertices;
}


/**
 * Returns the number of edges connecting vertices in g.
 */
int graph_num_edges(Graph *g) {
    return g->num_edges;
}


/**
 * Returns the size of the graph's adjacency lists array.
 */
int graph_array_size(Graph *g) {
    return g->adj_size;
}


/**
 * Adds a new vertex v to the graph. Initially, the vertex's adjacency list will 
 * be empty. If the graph's array of adjacency lists size is less than v, it will 
 * grow (reallocated) in order to store the new vertex.
 * 
 * WARNING: be careful when adding/removing vertices from a graph, for the graphs 
 * in this implementation will have an adjacency lists array with a size greater 
 * than or equal to the highest indexed vertex added to it. Once a graph's 
 * adjacency lists array is increased in size, it won't automatically be shrunk 
 * back!
 * 
 * @param g a pointer to the graph.
 * @param v the identifier (index) of the new vertex.
 * @return true if v was successfuly added to g.
 * @return false if v is already in g or if the required memory couldn't be allocated.
 */
bool graph_add_vertex(Graph *g, int v) 
{
    /* Should the size of the graph's array of adjacency lists be increased? */
    if(v >= g->adj_size) {             
        int n = 1;
        while(v >= (g->adj_size + (n++) * g->delta_realloc));
        
        if(!graph_grow(g, --n))
            return false;  // realloc failed (graph's adjacency lists array couldn't grow)
    }
    /* Checking if v is already in the graph */
    else if(graph_has_vertex(g, v))
        return false;  // the vertex is already in the graph

    /* Adding the vertex*/
    g->adj_lists[v] = list_create();
    if(g->adj_lists[v] == NULL)
        return false;  // couldn't allocate the needed memory for v's adjacency list
    
    g->num_vertices++;
    return true;
}


/**
 * Adds to the graph a weighted directed edge from vertex v to vertex w.
 * 
 * @param g a pointer to the graph.
 * @param v the identifier (index) of the source vertex (edge's tail).
 * @param w the identifier (index) of the destination vertex (edge's head).
 * @param weight the edge's weight; use the same weights for all the edges if 
 * you wish to treat the graph as an unweighted one.
 * @param create_if_needed whether vertices v or w should be added to the graph 
 * if they do not exist.
 * @return true if the edge was successfuly added. 
 * @return false if: either v or w doesn't exist and create_if_needed is set to 
 * false OR the memory needed to create either v or w couldn't be allocated.
 */
bool graph_add_edge(Graph *g, int v, int w, double weight, bool create_if_needed) 
{
    if( (!graph_has_vertex(g, v) || !graph_has_vertex(g, w)) && !create_if_needed)
        return false;  // one of the vertices doesn't exist and can't be created

    /* Adding v to g if needed*/
    if(!graph_has_vertex(g, v)) {
        bool failed = !graph_add_vertex(g, v);
        if(failed)
            return false;   // v couldn't be added to g
    }

    /* Adding w to g if needed*/
    if(!graph_has_vertex(g, w)) {
        bool failed = !graph_add_vertex(g, w);
        if(failed)
            return false;   // w couldn't be added to g
    }

    /* Adding the edge v->w */
    Edge *edge = malloc(sizeof(Edge));
    edge->from = v;  edge->to = w;
    edge->weight = weight;

    list_append(g->adj_lists[v], edge);
    g->num_edges++;
    return true;
} 


/**
 * @todo NOT IMPLEMENTED 
 * 
 * @param g 
 * @param num 
 * @return true 
 * @return false 
 */
static bool graph_shrink(Graph *g, int num) {
    return false;
}


/**
 * Returns true if the given edge points to the given vertex. Auxiliary function.
 * 
 * @param edge the edge; pointer to Edge.
 * @param v the destination vertex's id; pointer to int.
 * @return true if edge->to equals to v.
 */
static bool is_edge_head(void *edge, void *v) {
    return ((Edge*) edge)->to == *((int*) v);
}


/**
 * Removes a vertex from the given graph, along with all the edges associated
 * with it.
 * 
 * WARNING: be careful when adding/removing vertices from a graph, for the graphs 
 * in this implementation will have an adjacency lists array with a size greater 
 * than or equal to the highest indexed vertex added to it. Once a graph's 
 * adjacency lists array is increased in size, it won't automatically be shrunk 
 * back!
 * 
 * @param g pointer to the graph.
 * @param v index that identifies the vertex to be removed.
 * @return true if the vertex was successfuly removed; false otherwise.
 */
bool graph_remove_vertex(Graph *g, int v) 
{
    if(!graph_has_vertex(g, v))
        return false;       // the vertex doesn't exist

    /* Removing v and edges leaving it */
    g->num_vertices--;
    g->num_edges -= list_size(g->adj_lists[v]);

    list_free(&g->adj_lists[v], &free);
    g->adj_lists[v] = NULL;

    /* Removing edges pointing to v */
    for(int w = 0; w < g->adj_size; w++) {
        if(g->adj_lists[w] != NULL) 
            g->num_edges -= list_remove_all(g->adj_lists[w], &v, &is_edge_head, &free);
    }
}


/**
 * Removes the edge v->w from the graph. Parallel edges are also removed.
 * 
 * @param g pointer to the graph.
 * @param v index that identifies the first vertex.
 * @param w index that identifies the second vertex.
 * @return true if at least one edge was removed; false otherwise.
 */
bool graph_remove_edge(Graph *g, int v, int w) {
     if(!graph_has_vertex(g, v) || !graph_has_vertex(g, w))
        return false;   // either v or w isn't in the graph!

    int count = list_remove_all(g->adj_lists[v], &w, &is_edge_head, &free);
    g->num_edges -= count;
    return count > 0;
}


/**
 *      Returns an array containing the IDs (indices) of all of the graph's 
 * vertices. This function makes it possible for the caller to safely iterate 
 * through a graph from which one or more vertices were removed (remember that 
 * if v was removed from g, than g->adj_lists[v] is NULL, so simply iterating 
 * through the graph based on the number of vertices it currently have might 
 * lead to undefined behaviour).
 * 
 * @param g a pointer to the graph.
 * @return an array containing the IDs (indices) of all of the graph's vertices 
 * (sorted from the lowest key to the highest key) or NULL if the graph has no 
 * vertices.
 */
int* graph_vertices(Graph *g) {
    if(g->num_vertices == 0)
        return NULL;

    int i = 0, *arr = malloc(g->num_vertices * sizeof(int));
    for(int v = 0; v < g->adj_size; v++) {
        if(g->adj_lists[v] != NULL) 
            arr[i++] = v;
    }

    return arr;
}


/**
 * Returns a pointer to a copy of the given edge.
 * 
 * @param e a pointer to the edge being copied.
 * @return a pointer to the copy edge or NULL if the memory couldn't be allocated.
 */
Edge* copy_edge(Edge *e)
{
    Edge *e_cpy = malloc(sizeof(Edge));
    if(e_cpy != NULL) {
        e_cpy->from = e->from;
        e_cpy->to = e->to;
        e_cpy->weight = e->weight;
    }

    return e_cpy;
}


/**
 *      Returns the size of a vertex adjacency list (i.e. the amount of edges
 * leaving) the vertex.
 * 
 * @param g a pointer to the graph.
 * @param v the identifier (index) of the vertex.
 * @return the size of v's adjacency list or 0 if it's not in the graph.
 */
int vertex_adj_size(Graph *g, int v) {
    if(graph_has_vertex(g, v))
        return list_size(g->adj_lists[v]);
    else 
        return 0;
}


/**
 *      Returns an array with all the edges leaving the vertex v (i.e. the 
 * adjacency list of v). The array is generated from v's adjacency list, so keep 
 * in mind that this is NOT an O(1) operation!
 * 
 * @param g a pointer to the graph.
 * @param v the identifier (index) of vertex v.
 * @return an array containing the edges leaving v (i.e. edges that have v as 
 * their tail) or NULL if either v is not adjacent to any vertices or v doesn't
 * exist in the given graph; it's the caller's responsability to free the array.
 */
Edge** edges_from_vertix(Graph *g, int v) 
{
    if(!graph_has_vertex(g, v))
        return NULL;

    int size = list_size(g->adj_lists[v]);
    if(size == 0)  
        return NULL;

    int i = 0; 
    Edge **arr = malloc(size * sizeof(Edge*));
    Node *n = list_head(g->adj_lists[v]);

    while(n != NULL) {
        Edge *edge = list_node_item(n);
        arr[i++] = copy_edge(edge);
        n = list_next_node(n);
    }

    return arr;
}


/**
 *      Frees the memory allocated by an array of edges (possibly generated with 
 * the function edges_from_vertix).
 * 
 * @param arr a pointer to a variable that holds a pointer to an array of Edge 
 * pointers; after the call, the variable will be set to NULL.
 * @param n the size of the array.
 */
void free_edges_array(Edge **arr[], int n) 
{
    Edge **e = *arr;
    for(int i = 0; i < n; i++)
        free(e[i]);
    
    free(e);
    *arr = NULL;
}   


/**
 * Returns the edge's source vertex (i.e. the edge's tail).
 */
int edge_source(Edge *e) {
    return e->from;
}


/**
 * Returns the edge's destination vertex (i.e. the edge's head).
 */
int edge_dest(Edge *e) {
    return e->to;
}


/**
 * Returns the edge's weight.
 */
double edge_weight(Edge *e) {
    return e->weight;
}


/**
 * Returns the amount of neighbours that vertex v has (i.e. the size of v's 
 * adjacency list).
 * 
 * @param g a pointer to the graph.
 * @param v the identifier (index) of vertex v.
 * @return size of vertex v's adjacency list.
 */
int graph_adj_count(Graph *g, int v) {
    return list_size(g->adj_lists[v]);
}


/**
 * Auxialiary function used by list_print.
 */
static void print_vertex_adj(void *edge) {
    Edge *e = edge;
    printf("(%d, %.1f) ", e->to, e->weight);
}


/** 
 * Prints the edges in the adjacency lists of the graph's vertices.
 */
void graph_print(Graph *g) {
    for(int i = 0; i < g->adj_size; i++) {
        if(g->adj_lists[i] != NULL) {
            printf("[%d]: { ", i);
            list_print(g->adj_lists[i], &print_vertex_adj);
            printf("}\n");
        }
    }
}