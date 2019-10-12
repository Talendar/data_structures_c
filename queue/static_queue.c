/*
 * Array implementation of a queue (static/fixed size).
 * Created by Gabriel Nogueira (Talendar).
 */

#include "static_queue.h"
#include <stdlib.h>
#include <stdio.h>

/*
 * Structure of a static queue.
 */
struct Queue {
    int MAX_SIZE, //maximum number of elements the queue supports
        size,
        front, rear; //front and rear indices
    void **items; //array of pointers to the queue items
};

/*
 * Creates a new queue with the specified max size and returns a pointer to it. Returns NULL if the memmory couldn't be allocated.
 */
Queue* queue_create(int max_size)
{
    Queue *q = malloc(sizeof(Queue));
    if(q != NULL) {
        q->MAX_SIZE = max_size;
        q->front = q->size = 0;
        q->rear = -1;
        q->items = malloc(max_size * sizeof(void*));

        if(q->items == NULL)
            return NULL;
    }

    return q;
}

/*
 * Free the allocated memmory associated with the queue and its items. If you don't want to free the items, pass NULL to item_free.
 */
void queue_free(Queue **q, void (*item_free)(void *item))
{
    if(!queue_empty(*q) && item_free != NULL) {
        for(int i = 0; i < (*q)->size; i++) {
            int j = (((*q)->front + i) < (*q)->MAX_SIZE) ? ((*q)->front + i) : ((*q)->front + i - (*q)->MAX_SIZE);
            item_free((*q)->items[j]);
        }
    }

    free((*q)->items);
    free(*q);
    *q = NULL;
}

/*
 * Insert an element on the end of the queue. Returns true if the item was inserted successfuly or false otherwise.
 */
bool queue_insert(Queue *q, void *item)
{
    if(!queue_full(q)) {
        q->rear = ((q->rear + 1) < q->MAX_SIZE) ? q->rear + 1 : 0;
        q->items[q->rear] = item;
        q->size++;
        return true;
    }

    return false;
}

/*
 * Removes and returns the item on the front of the queue. Returns NULL if the queue is empty (underflow).
 */
void* queue_remove(Queue *q)
{
    if(!queue_empty(q)) {
        void *item = q->items[q->front];
        q->front = ((q->front + 1) < q->MAX_SIZE) ? q->front + 1 : 0;
        q->size--;
        return item;
    }

    return NULL;
}

/*
 * Returns the item in the front of the queue without removing it. Returns NULL if the queue is empty (underflow).
 */
void* queue_front(Queue *q) 
{
    if(!queue_empty(q))
        return q->items[q->front];
    
    return NULL;
}

/*
 * Returns the size of the queue.
 */
int queue_size(Queue *q) {
    return q->size;
}

/*
 * Returns true if the queue is empty or false otherwise.
 */
bool queue_empty(Queue *q) {
    return !q->size;
}

/*
 * Returns true if the is full or false otherwise.
 */
bool queue_full(Queue *q) {
    return q->size == q->MAX_SIZE;
}

/*
 * Inverts the order of the queue's items.
 */
void queue_invert(Queue *q)
{
    if(!queue_empty(q)) {
        int f = q->front, r = q->rear;
        for(int i = 0; i < q->size/2; i++) {
            void *aux = q->items[f];
            q->items[f] = q->items[r];
            q->items[r] = aux;
            
            f = ((f + 1) < q->MAX_SIZE) ? f + 1 : 0;
            r = (r > 0) ? r - 1 : (q->MAX_SIZE - 1);
        }
    }
}

/*
 * Prints all the queue's items, starting from its front item.
 */
void queue_print(Queue *q, void (*item_print)(void *item))
{
    if(!queue_empty(q)) {
        for(int i = 0; i < q->size; i++) {
            int j = ((q->front + i) < q->MAX_SIZE) ? (q->front + i) : (q->front + i - q->MAX_SIZE);
            item_print(q->items[j]);
        }
    }
}