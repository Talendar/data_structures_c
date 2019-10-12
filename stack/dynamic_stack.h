/*
 * Linked implementation of a stack (dynamic/variable size).
 * Created by Gabriel Nogueira (Talendar).
 */

#ifndef DYNAMIC_STACK_H
    #define DYNAMIC_STACK_H
    #include <stdbool.h>

    typedef struct Stack Stack;
    typedef struct Node Node;

    Stack* stack_create(void);
    void stack_free(Stack **stack, void (*item_delete)(void *item));

    bool stack_push(Stack *stack, void *item);
    void* stack_pop(Stack *stack);

    void* stack_top(Stack *stack);
    int stack_size(Stack *stack);
    
    bool stack_empty(Stack *stack);
    bool stack_full(Stack *stack);

    void stack_invert(Stack *stack);
    void stack_print(Stack *stack, void (*item_print)(void *item));
#endif
