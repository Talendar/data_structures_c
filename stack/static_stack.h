/*
 * Array implementation of a stack (static/fixed size).
 * Created by Gabriel Nogueira (Talendar).
 */

#ifndef STATIC_STACK_H
    #define STATIC_STACK_H
    #include <stdbool.h>
    typedef struct Stack Stack;

    Stack* stack_create(int max_size);
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
