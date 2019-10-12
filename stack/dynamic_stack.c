/*
 * Stack implementation based on linked lists.
 * Created by Gabriel Nogueira (Talendar).
 */

#include "dynamic_stack.h"
#include <stdlib.h>
#include <stdio.h>


/*
 * Structure of the stack's nodes.
 */
struct Node {
    void *item;
    Node *previous;
};

/*
 * Stack's structure.
 */
struct Stack {
    Node *top;
    int size;
};

/*
 * Creates a stack with the and returns a pointer to it. Returns NULL if the memmory couldn't be allocated.
 */
Stack* stack_create(void)
{
    Stack *stack = malloc(sizeof(Stack));
    if(stack != NULL) {
        stack->top = NULL;
        stack->size = 0;
    }

    return stack;
}

/*
 * Frees the memmory allocated by the stack AND its items. If the stack's items shouldn't be freed, pass NULL as the argument of the parameter item_free.
 */
void stack_free(Stack **stack, void (*item_free)(void *item))
{
    Node *n = (*stack)->top;
    while(n != NULL) {
        if(item_free != NULL)
            item_free(n->item);

        Node *next = n->previous;
        free(n);
        n = next;
    }

    free(*stack);
    *stack = NULL;
}

/*
 * Pushes a new item into the stack. Returns true if the operation was successful or false otherwise.
 */
bool stack_push(Stack *stack, void *item)
{
    Node *n = malloc(sizeof(Node));
    if(n != NULL) {
        n->item = item;
        n->previous = stack->top;

        stack->size++;
        stack->top = n;
        return true;
    }

    return false;
}

/*
 * Removes and returns the last item pushed into the stack. Returns NULL if the stack is empty (underflow).
 */
void* stack_pop(Stack *stack)
{
    Node *n = stack->top;
    if(n != NULL) {
        void *item = n->item;
        stack->top = n->previous;
        stack->size--;

        free(n);
        return item;
    }

    return NULL;
}

/*
 * Returns the top-most element of the stack without removing it. Returns NULL if the stack is empty(underflow).
 */
void* stack_top(Stack *stack) 
{
    if(stack->top != NULL)
        return stack->top->item;

    return NULL;
}

/*
 * Returns the number of elements the stack has.
 */
int stack_size(Stack *stack) {
    return stack->size;
}

/*
 * Returns true if the stack is empty or false otherwise.
 */
bool stack_empty(Stack *stack) {
    return stack->size == 0;
}

/*
 * Return true if the stack is full or false otherwise. Since this is a dynamic stack (the size of the stack is determined by the amount of memmory available), this function will always return false.
 */
bool stack_full(Stack *stack) {
    return false;
}

/*
 * Inverts the order of the stack's elements.
 */
void stack_invert(Stack *stack)
{
    if(!stack_empty(stack)) {
        Node *previous = stack->top, *current = stack->top->previous;
        stack->top->previous = NULL;

        while(current != NULL) {
            Node *aux = current->previous;
            current->previous = previous;

            previous = current;
            current = aux;
        }

        stack->top = previous;
    }
}

/*
 * Prints the stack's items with the function passed as an argument.
 */
void stack_print(Stack *stack, void (*item_print)(void *item))
{
    Node *n = stack->top;
    while(n != NULL) {
        item_print(n->item);
        n = n->previous;
    }
}