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


#include "unweighted_digraph.h"
#include "singly_linked_list.h"
#include <stdlib.h>
#include <stdio.h>


/**
 * General structure of an unweighted digraph implemented with adjacency lists. An array is used to store the adjacency lists of each of the graph's vertices. Once the array is full, it's reallocated in order to grow in size.
 */
struct UnweightedDigraph {
    List **adj_lists;       // array of adjacency lists; each index represents a vertex in the graph; if adj_lists[i] is NULL, then the vertex i is not in the graph.
    int adj_size,           // the current size of adj_lists
        delta_realloc;      // defines how much adj_lists will grow in each realloc

    int num_vertices,       // number of vertices in the graph
        num_edges;          // number of edges in the graph (parallel edges do NOT count as a single edge)
};


/**
 * Creates a new empty unweighted digraph and returns a pointer to it. With this function the caller can specify the graph's adjacency lists array initial size as well as how much it will grow in each realloc.
 * 
 * @param initial_size initial size of the graph's adjacency lists array (once it's full, realloc will be called to increase its size).
 * @param delta_realloc defines how much the graph's adjacency lists array will grow in each realloc.
 * @return a pointer to the newly created graph if all the required memory could be allocated; NULL otherwise.
 */
Graph* graph_create_full(int initial_size, int delta_realloc) 
{
    Graph *g = malloc(sizeof(Graph));
    if(g != NULL) {
        g->adj_lists = malloc(initial_size * sizeof(List*));

        if(g->adj_lists != NULL) {
            for(int i = 0; i < initial_size; i++)
                g->adj_lists[i] = NULL;

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
 * Creates a new empty unweighted digraph and returns a pointer to it. Wrapper for the function graph_create_full(). Exempts the user from having to choose an initial size and a growing size for the graph's adjacency lists array (default values are used).
 * 
 * @return a pointer to the newly created graph if all the required memory could be allocated; NULL otherwise.
 */
Graph* graph_create() {
    return graph_create_full(ADJ_LISTS_ARRAY_INITIAL_SIZE, ADJ_LISTS_ARRAY_DELTA_REALLOC);
}


/**
 * Frees the memory allocated by the graph. 
 * 
 * @param g a double pointer to the graph; by the end of the execution, the variable pointed by g will be set to NULL.
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
 * Increases the size of a graph's adjacency lists array by g->delta_realloc.
 * 
 * @param g a pointer to the graph.
 * @param num the array will be increased by num*g->delta_realloc.
 * @return true if the operation was successful; false if the memory couldn't be reallocated (in this case, g->adj_lists will remain unchanged).
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
 * Checks whether the given graph contains a cycle. Uses a non-recursive version of Depth-First Search (the code is harder to read, but it reduces the chances of a stack overflow).
 * 
 * @param g pointer to the graph.
 * @return true if g has at least one cycle; false otherwise (g is a DAG).
 */
bool graph_has_cycle(Graph *g) 
{
    bool *visited = calloc(g->adj_size, sizeof(bool));          // used to keep track of the visited vertices
    bool *on_stack = calloc(g->adj_size, sizeof(bool));         // used to keep track of the vertices in the current path
    List *stack = list_create();

    for(int v = 0; v < g->adj_size; v++) {                      // iterate through all the vertices of g
        if(g->adj_lists[v] == NULL || visited[v])
            continue;

        int *t = malloc(sizeof(int));  *t = v;
        list_push(stack, t);

        while(!list_empty(stack)) {                             // iterating through the stack
            int w = *((int*) list_top(stack));
            
            if(!visited[w])
                visited[w] = on_stack[w] = true;
            else {
                on_stack[w] = false;
                free(list_pop(stack));
            }

            int *adj_w = graph_adj_to(g, w);
            for(int i = 0; i < graph_adj_count(g, w); i++) {    // iterating through w's adjacency list
                int s = adj_w[i];
                if(!visited[s]) {
                    int *t2 = malloc(sizeof(int));  *t2 = s;
                    list_push(stack, t2);
                }
                else if(on_stack[s]) {
                    free(visited);  free(on_stack);  list_free(&stack, &free);  free(adj_w);
                    return true;
                }
            }
            free(adj_w);
        }
    }

    free(visited);  free(on_stack);
    list_free(&stack, &free);
    return false;
}


/**
 * Finds all the source vertices of the graph. Source vertices are those that don't have any edges pointing to them.
 */
List* graph_find_sources(Graph *g) 
{
    bool *is_source = calloc(g->adj_size, sizeof(bool));
    int source_count = g->adj_size;
    for(int i = 0; i < g->adj_size; i++)
        is_source[i] = true;

    /* Finding sources */
    for(int v = 0; v < g->adj_size; v++) {
        if(g->adj_lists[v] != NULL) {
            int *adj_v = graph_adj_to(g, v);
            for(int i = 0; i < graph_adj_count(g, v); i++) {
                int w = adj_v[i];
                if(is_source[w]) {
                    is_source[w] = false;
                    source_count--;
                }
            }
            free(adj_v);
        }
        else {
            is_source[v] = false;
            source_count--;
        }
    }

    /* Retrieving sources */
    if(source_count == 0) {
        free(is_source);
        return NULL;
    }

    List *sources = list_create();
    for(int v = 0; v < g->adj_size && source_count > 0; v++) {
        if(is_source[v]) {
            int *t = malloc(sizeof(int));  *t = v;
            list_append(sources, t);
            source_count--;
        }
    }

    free(is_source);
    return sources;
}


/**
 * Adds a new vertex v to the graph. Initially, the vertex's adjacency list will be empty. If the graph's adjacency lists array's size is less than v, it will be reallocated in order to store the new vertex.
 * 
 * WARNING: be careful when adding/removing vertices from a graph, for the graphs in this implementation will have an adjacency lists array with a size greater than or equal to the highest indexed vertex added to it. Once a graph's adjacency lists array is increased in size, it won't automatically be shrunk back!
 * 
 * @param g a pointer to the graph.
 * @param v the identifier (index) of the new vertex.
 * @return true if v was successfuly added to g.
 * @return false if v is already in g or if the required memory couldn't be allocated.
 */
bool graph_add_vertex(Graph *g, int v) 
{   
    /* Checking whether the graph's adjacency lists array's size should be increased */
    if(v >= g->adj_size) {             
        int n = 1;
        while(v >= (g->adj_size + (n++) * g->delta_realloc));
        
        if(!graph_grow(g, --n))
            return false;               // realloc failed (graph's adjacency lists array couldn't grow)
    }
    /* Checking if v is already in the graph */
    else if(graph_has_vertex(g, v))
        return false;                   // the vertex is already in the graph

    /* Adding the vertex*/
    g->adj_lists[v] = list_create();
    if(g->adj_lists[v] == NULL)
        return false;                   // couldn't allocate the needed memory for v's adjacency list
    
    g->num_vertices++;
    return true;
}


/**
 * Adds a directed edge from vertex v to vertex w.
 * 
 * @param g a pointer to the graph.
 * @param v the identifier (index) of vertex v.
 * @param w the identifier (index) of vertex w.
 * @param create_if_needed should vertices v or w be added to g if they do not exist?
 * @return true if the edge was successfuly added. 
 * @return false if: either v or w doesn't exist and create_if_needed is set to false OR the memory needed to create either v or w couldn't be allocated.
 */
bool graph_add_edge(Graph *g, int v, int w, bool create_if_needed) 
{
    if( (!graph_has_vertex(g, v) || !graph_has_vertex(g, w)) && !create_if_needed)
        return false;       // one of the vertices doesn't exist

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
    int *w_cpy = malloc(sizeof(int));  (*w_cpy) = w;
    list_append(g->adj_lists[v], w_cpy);
    g->num_edges++;
    return true;            // edge v->w successfuly added
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
 * Returns true if the given integers are equal or false otherwise. Auxiliary function.
 */
static bool compare_ints(void *a, void *b) {
    return *((int*) a) == *((int*) b);
}


/**
 * Removes a vertex from the given graph (along with all the edges that points to it).
 * 
* WARNING: be careful when adding/removing vertices from a graph, for the graphs in this implementation will have an adjacency lists array with a size greater than or equal to the highest indexed vertex added to it. Once a graph's adjacency lists array is increased in size, it won't automatically be shrunk back!
 * 
 * @param g pointer to the graph.
 * @param v index that identifies the vertex.
 * @return true if the vertex was successfuly removed; false otherwise.
 */
bool graph_remove_vertex(Graph *g, int v) 
{
    if(!graph_has_vertex(g, v))
        return false;       // the vertex doesn't exist

    g->num_vertices--;
    g->num_edges -= list_size(g->adj_lists[v]);

    list_free(&g->adj_lists[v], &free);
    g->adj_lists[v] = NULL;

    for(int w = 0; w < g->adj_size; w++) {
        if(g->adj_lists[w] != NULL) 
            g->num_edges -= list_remove_all(g->adj_lists[w], &v, &compare_ints, &free);
    }

    return true;
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
        return false;

    int count = list_remove_all(g->adj_lists[v], &w, &compare_ints, &free);
    g->num_edges -= count;
    return count > 0;
}


/**
 * Returns an array containing the IDs (indices) of all the graph's vertices. This function makes it possible for the caller to safely iterate through a graph from which one or more vertices were removed (remember that if v was removed from g, than g->adj_lists[v] is NULL, so simply iterating through the graph based on the number of vertices it currently have might lead to undefined behaviour).
 * 
 * @param g a pointer to the graph.
 * @return an array containing the IDs (indices) of all the graph's vertices or NULL if the graph has no vertices.
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
 * Returns an array containing the vertices adjacent to v. The array is generated from v's adjacency list, so keep in mind that this is NOT an O(1) operation!
 * 
 * @param g a pointer to the graph.
 * @param v the identifier (index) of vertex v.
 * @return an array containing the vertices adjacent to v or NULL if v is not adjacent to any vertices.
 */
int* graph_adj_to(Graph *g, int v) {
    int size = list_size(g->adj_lists[v]);
    if(size == 0)  
        return NULL;

    int i = 0, *arr = malloc(size * sizeof(int));
    Node *n = list_head(g->adj_lists[v]);

    while(n != NULL) {
        int *val = list_node_item(n);
        arr[i++] = *val;
        n = list_next_node(n);
    }

    return arr;
}


/**
 * Returns the amount of neighbours of the vertex v.
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
static void print_vertex_adj(void *v) {
    printf(" %d ", *((int*) v));
}


/** 
 * Prints the adjacency lists of the graph's vertices.
 */
void graph_print(Graph *g) {
    for(int i = 0; i < g->adj_size; i++) {
        if(g->adj_lists[i] != NULL) {
            printf("[%d]: {", i);
            list_print(g->adj_lists[i], &print_vertex_adj);
            printf("}\n");
        }
    }
}