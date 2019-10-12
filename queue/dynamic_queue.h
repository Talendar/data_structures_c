/*
 * Linked implementation of a queue (dynamic/variable size).
 * Created by Gabriel Nogueira (Talendar).
 */

#ifndef DYNAMIC_QUEUE_H
    #define DYNAMIC_QUEUE_H
    #include <stdbool.h>
    typedef struct Node Node;
    typedef struct Queue Queue;

    Queue* queue_create(void);
    void queue_free(Queue **q, void (*item_free)(void *item));

    bool queue_insert(Queue *q, void *item);
    void* queue_remove(Queue *q);

    void* queue_front(Queue *q);
    int queue_size(Queue *q);

    bool queue_empty(Queue *q);
    bool queue_full(Queue *q);

    void queue_invert(Queue *q);
    void queue_print(Queue *q, void (*item_print)(void *item));
#endif