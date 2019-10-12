/*
 * Array implementation of a queue (static/fixed size).
 * Created by Gabriel Nogueira (Talendar).
 */

#ifndef STATIC_QUEUE_H
    #define STATIC_QUEUE_H
    #include <stdbool.h>
    typedef struct Queue Queue;

    Queue* queue_create(int max_size);
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