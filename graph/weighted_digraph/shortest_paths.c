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


#include "shortest_paths.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>


/**
 *      Structure of a shortest-paths tree, generated as a result from pathfinding
 * algorithms.
 * 
 * Attributes:
 *      . size: the number of vertices in the tree.
 *      . source: the source vertex (starting point of the algorithms).
 *      . dist_to: array with the distances of the known shortest path from the 
 *      source vertex to any other vertex in the tree; the distance to vertices 
 *      not reachable from the source is INFINITY and the distance from the 
 *      source to itself, 0.
 *      . edge_to: array of edges; edge_to[v] holds a pointer to the edge that
 *      connects v to its parent in the tree (the last edge on a shortest path
 *      from the source to v). If v is not reachable from the source, edge_to[v] 
 *      is NULL.
 * 
 */
struct ShortestPathsTree {
    int size, source;
    double *dist_to;
    Edge **edge_to;
};


/**
 *      Creates and initializes with default values a structure of a shortest-paths
 * tree. Returns a pointer to it.
 * 
 * @param size number of vertices in the associated graph.
 * @param source identifier (index) of the source vertex (starting point of the
 * pathfinding algorithms).
 * @return a pointer to the created SPT or NULL if the necessary memory couldn't
 * be allocated.
 */
static SPT* spt_create(int size, int source)
{
    SPT *spt = malloc(sizeof(SPT));
    if(spt != NULL) {
        spt->size = size;
        spt->source = source;

        spt->dist_to = malloc(sizeof(double) * size);
        if(spt->dist_to == NULL) {
            free(spt);
            return NULL;
        }

        spt->edge_to = malloc(sizeof(Edge*) * size);
        if(spt->edge_to == NULL) {
            free(spt);  free(spt->dist_to);
            return NULL;
        }

        for(int i = 0; i < size; i++) {
            spt->dist_to[i] = INFINITY;
            spt->edge_to[i] = NULL;
        }

        spt->dist_to[source] = 0;
    }

    return spt;
}


/**
 * Frees the memory allocated by a SPT object.
 * 
 * @param spt a pointer to the variable that is holding a pointer to the object;
 * by the end of the call, the variable will be set to NULL.
 */
void spt_free(SPT **spt) 
{
    free((*spt)->dist_to);
    for(int i = 0; i < (*spt)->size; i++) {
        if((*spt)->edge_to[i] != NULL)
            free((*spt)->edge_to[i]);
    }
    free((*spt)->edge_to);

    free(*spt);
    *spt = NULL;
}


/**
 * Returns the source vertex of a SPT.
 */
int spt_source(SPT *spt) {
    return spt->source;
}


/**
 * Returns the number of vertices in a SPT.
 */
int spt_size(SPT *spt) {
    return spt->size;
}


/**
 * Checks whether there is a path from the SPT source to the vertex v.
 * 
 * @param spt a pointer to the SPT.
 * @param v the identifier (index) of the destination vertex.
 * @return true if there is a path from the source to v and false otherwise. 
 */
bool spt_has_path(SPT *spt, int v) {
    return !isinf(spt->dist_to[v]);
}


/**
 * Returns a list with the edges from the SPT's source to the vertex v. 
 * 
 * @param spt a pointer to a SPT.
 * @param v the identifier (index) of the destination vertex.
 * @return a list with the edges in the path from the source to v if such path
 * exists; an empty list if v is the source vertex; NULL if there is no path from
 * the source to v.
 */
List* spt_path_to(SPT *spt, int v) 
{
    if(!spt_has_path(spt, v))
        return NULL;

    List *path = list_create();
    Edge *e = spt->edge_to[v];

    while(e != NULL) {      // iterates untill an edge from the source is found
        Edge *e_cpy = copy_edge(e);
        list_push(path, e_cpy);
        e = spt->edge_to[edge_source(e)];
    }

    return path;
}


/**
 *      Returns the weight of the path from the source vertex to v (INFINITY if 
 * there is no path).
 * 
 * @param spt a pointer to a SPT.
 * @param v the identifier (index) of the destination vertex.
 * @return the weight of the path from the source vertex to v or INFINITY, if 
 * there is no path.
 */
double spt_path_dist(SPT *spt, int v) {
    return spt->dist_to[v];
}


/**
 *      Checks if an edge can be relaxed and, if that's the case, relaxes it. To 
 * relax an edge v->w means to test whether the best known way from the source 
 * vertex s to w is to go from s to v, taking the edge from v to w, and, if so,
 * update our data structures to indicate that to be the case (Sedgewick & Wayne, 
 * 2011). 
 *      Either the edge is ineligible, an no changes are made, or the edge v->w 
 * leads to a shorter path to w, in which case the SPT object is updated.
 * 
 * @param e a pointer to the edge.
 * @param spt the SPT object with the relevant data structures.
 */
static bool relax_edge(Edge *e, SPT *spt) 
{   
    int v = edge_source(e), w = edge_dest(e);
    double current_dist = spt->dist_to[w],
           new_dist = edge_weight(e) + spt->dist_to[v];

    if(new_dist < current_dist) {
        free(spt->edge_to[w]);
        spt->edge_to[w] = copy_edge(e);
        spt->dist_to[w] = new_dist;
        return true;
    }

    return false;
}


/**
 * Pops the lowest key from the set. Auxiliary function.
 */
static int pop_lowest_key(bool *set, SPT *spt)
{
    int lowest_key = -1;
    double lowest_val = INFINITY;

    for(int i = 0; i < spt->size; i++) {
        if(set[i] && spt->dist_to[i] < lowest_val) {
            lowest_key = i;
            lowest_val = spt->dist_to[i];
        }
    }

    set[lowest_key] = false;
    return lowest_key;
}


/**
 *      Implements Dijkstra's shortest-paths algorithm using an unoreded list.
 * Although modern implementations of the algorithm use heap-based priority 
 * queues, the original algorithm used an unordered set (here represented by an
 * unordered list) and linear search.
 * 
 * @todo: implement the algorithm using a heap-based priority queue, reducing 
 * the running time complexity from O(|V|^2) to O(|E|log|V|).
 * 
 * @param g a pointer to the graph (expects a weighted digraph).
 * @param s the identifier (index) of the source vertex.
 * @return a pointer to a SPT object with relevant informations obtained from
 * the pathfinding algorithm.
 */
SPT* dijkstra_sp(Graph *g, int s)
{
    SPT *spt = spt_create(graph_array_size(g), s);
    bool *set = calloc(sizeof(bool), graph_array_size(g));

    set[spt->source] = true;
    int count = 1;  // number of vertices currently in the set

    while(count > 0) {
        int v = pop_lowest_key(set, spt);
        count--;

        // relaxes the vertex
        Edge **edges = edges_from_vertix(g, v);
        if(edges != NULL) {
            for(int e = 0; e < vertex_adj_size(g, v); e++) {
                bool relaxed = relax_edge(edges[e], spt);
                int w = edge_dest(edges[e]);

                if(relaxed && !set[w]) {
                    set[w] = true;
                    count++;
                }
                free(edges[e]);
            }
            free(edges);
        }
    }

    free(set);
    return spt;
}
