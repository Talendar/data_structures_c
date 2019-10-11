#include "static_stack.h"
#include <stdlib.h>
#include <stdio.h>

/*
 * Stack's structure.
 */
struct Stack {
    int MAX_SIZE, 
        top; //set to -1 when the stack is empty!
    void **items; //array of pointers to the stack's items
};

/*
 * Creates a stack with the given maximum size and returns a pointer to it. Returns NULL if the memmory couldn't be allocated.
 */
Stack* stack_create(int max_size)
{
    Stack *stack = malloc(sizeof(Stack));
    if(stack != NULL) {
        stack->MAX_SIZE = max_size;
        stack->top = -1;
        stack->items = malloc(max_size * sizeof(void*));
    }

    return stack;
}

/*
 * Frees the memmory allocated by the stack AND its items. If the stack's items shouldn't be freed, pass NULL as the argument of the parameter item_free.
 */
void stack_free(Stack **stack, void (*item_free)(void *item))
{
    if(item_free != NULL) {
        for(int i = 0; i < stack_size(*stack); i++) {
            item_free((*stack)->items[i]);
        }
    }

    free((*stack)->items);
    free(*stack);
    stack = NULL;
}

/*
 * Pushes a new item into the stack. Returns true if the operation was successful or false otherwise.
 */
bool stack_push(Stack *stack, void *item)
{
    if(!stack_full(stack)) {
        stack->items[++stack->top] = item;
        return true;
    }

    return false;
}

/*
 * Removes and returns the last item pushed into the stack. Returns NULL if the stack is empty (underflow).
 */
void* stack_pop(Stack *stack)
{
    if(!stack_empty(stack))
        return stack->items[stack->top--];

    return NULL;
}

/*
 * Returns the top-most element of the stack without removing it. Returns NULL if the stack is empty(underflow).
 */
void* stack_top(Stack *stack) {
    return stack->items[stack->top];
}

/*
 * Returns the number of elements the stack has.
 */
int stack_size(Stack *stack) {
    return stack->top + 1;
}

/*
 * Returns true if the stack is empty or false otherwise.
 */
bool stack_empty(Stack *stack) {
    return stack->top == -1;
}

/*
 * Return true if the stack is full or false otherwise.
 */
bool stack_full(Stack *stack) {
    return (stack->top + 1) == stack->MAX_SIZE;
}

/*
 * Inverts the order of the stack's elements.
 */
void stack_invert(Stack *stack)
{
    for(int i = 0; i < stack->top/2; i++) {
        void *aux = stack->items[i];
        int j = stack->top - i;

        stack->items[i] = stack->items[j];
        stack->items[j] = aux;
    }  
}

/*
 * Prints the stack's items with the function passed as an argument.
 */
void stack_print(Stack *stack, void (*item_print)(void *item))
{
    for(int i = stack->top; i >= 0; i--) {
        item_print(stack->items[i]);
    }
}