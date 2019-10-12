/*
 * Simple program to test my queue implementation.
 * Created by Gabriel Nogueira (Talendar).
 */

#include <stdio.h>
#include <stdlib.h>
#include "dynamic_queue.h"

void print_int(void *integer) {
    printf("%d ", *((int*) integer));
}

void test_queue(Queue *q) 
{
    printf("\n");
    //queue_invert(q);
    int c = 0;
    do {
        printf("QUEUE: [");  queue_print(q, &print_int);
        printf("]  |  SIZE: %d  |  FRONT: %d", queue_size(q), *((int*) queue_front(q)));
        printf("  |  REMOVED: %d\n", *((int*) queue_remove(q)));

        if(++c == 3) {
            printf("\nEnter 5 numbers:\n");
            for(int i = 0; i < 5; i++) {
                int *n = malloc(sizeof(int));
                scanf(" %d", n);
                queue_insert(q, n);
            }
            printf("\n");
        }
    } while(!queue_empty(q));
}

int main(void)
{
    printf("Enter 5 numbers:\n");
    Queue *q = queue_create();

    for(int i = 0; i < 5; i++) {
        int *n = malloc(sizeof(int));
        scanf(" %d", n);
        queue_insert(q, n);
    }
    test_queue(q);

    queue_free(&q, &free);
    return 0;
}