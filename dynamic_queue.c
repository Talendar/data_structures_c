/*
 * Linked implementation of a queue (dynamic/variable size).
 * Created by Gabriel Nogueira (Talendar).
 */

#include "dynamic_queue.h"
#include <stdlib.h>
#include <stdio.h>

/*
 * Structure of a dynamic queue's node.
 */
struct Node {
    void *item;
    Node *previous;
};

/*
 * Structure of a dynamic queue.
 */
struct Queue {
    Node *front, *rear;
    int size;
};

/*
 * Creates a new queue and returns a pointer to it. Returns NULL if the memmory couldn't be allocated.
 */
Queue* queue_create(void)
{
    Queue *q = malloc(sizeof(Queue));
    if(q != NULL) {
        q->front = q->rear = NULL;
        q->size = 0;
    }

    return q;
}

/*
 * Free the allocated memmory associated with the queue and its items. If you don't want to free the items, pass NULL to item_free.
 */
void queue_free(Queue **q, void (*item_free)(void *item))
{
    if(!queue_empty(*q)) {
        Node *n = (*q)->front;
        while(n != NULL) {
            if(item_free != NULL)
                item_free(n->item);
            
            Node *next = n->previous;
            free(n);
            n = next;
        }
    }

    free(*q);
    *q = NULL;
}

/*
 * Insert an element on the end of the queue. Returns true if the item was inserted successfuly or false otherwise.
 */
bool queue_insert(Queue *q, void *item)
{
    Node *n = malloc(sizeof(Node));
    if(n != NULL) {
        n->previous = NULL;
        n->item = item;

        if(queue_empty(q)) 
            q->front = n;
        else 
            q->rear->previous = n;

        q->rear = n;
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
        Node *n = q->front;
        q->front = n->previous;

        if(q->rear == n)
            q->rear = NULL;

        void *item = n->item;
        free(n);
        q->size--;
        return item;
    }

    return NULL;
}

/*
 * Returns the item in the front of the queue without removing it. Returns NULL if the queue is empty (underflow).
 */
void* queue_front(Queue *q) {
    return q->front->item;
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
 * Returns true if the is full or false otherwise. Since a dynamic queue's size is limited only by the available memory, this function will always return false and is here only for completeness.
 */
bool queue_full(Queue *q) {
    return false;
}

/*
 * Inverts the order of the queue's items.
 */
void queue_invert(Queue *q)
{
    Node *current = q->front, *prev = NULL;
    q->front = q->rear;
    q->rear = current;

    while(current != NULL) {
        Node *next = current->previous;
        current->previous = prev;
        prev = current;
        current = next;
    }
}

/*
 * Prints all the queue's items, starting from its front item.
 */
void queue_print(Queue *q, void (*item_print)(void *item))
{
    Node *n = q->front;
    while(n != NULL) {
        item_print(n->item);
        n = n->previous;
    }
}