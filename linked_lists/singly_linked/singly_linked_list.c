/*
 * Singly linked list implementation.
 * Created by Gabriel Nogueira (Talendar).
 */

#include "singly_linked_list.h"
#include <stdio.h>
#include <stdlib.h>


/*
 * Struct of the nodes of a singly linked list. Only a pointer to the next node is stored.
 */ 
struct Node {
    void *item;
    Node *next;
};

/*
 * Struct of a singly linked list. A pointer to the last node of the list is kept in order to make appending to the list an O(1) operation.
 */
struct List {
    Node *head, *end;
    int size;
};

/*
 * Creates a new list and returns a pointer to it. Returns NULL if the function fails to allocate the required memory for the data structure.
 */
List* list_create(void)
{
    List *list = malloc(sizeof(List));
    if(list != NULL) {
        list->head = list->end = NULL;
        list->size = 0;
    }

    return list;
}

/*
 * Frees the memory allocated by the list, its nodes and its items. Pass NULL as the argument of item_free if you don't want to free the memory of the items.
 */
void list_free(List **list, void (*item_free)(void *item))
{
    Node *n = (*list)->head;
    while(n != NULL) {
        if(item_free != NULL)
            item_free(n->item);

        Node *next = n->next;
        free(n);
        n = next;
    }
}

/*
 * Returns the size (number of items) of the list.
 */
int list_size(List *list) {
    return list->size;
}

/*
 * Returns true if the list is empty or false otherwise.
 */
bool list_empty(List *list) {
    return !list->size;
}

/*
 * Since a linked list size is limited only by the available memory of the system, this function always returns true and is here only for completeness.
 */
bool list_full(List *list) {
    return true;
}

/*
 * Inserts an item at the end of the list. Returns true if the operation was successful or false otherwise.
 */
bool list_append(List *list, void *item)
{
    Node *n = malloc(sizeof(Node));
    if(n != NULL) {
        n->item = item;

        if(list_empty(list))
            list->head = n;
        else
            list->end->next = n;

        list->end = n;
        list->size++;
        return true;
    }

    return false;
}

/*
 * Inserts an item at the given index of the list. The index of the newly inserted item will be the index passed as an argument. Returns false if the operation was successful or false otherwise.
 */
bool list_insert_at(List *list, void *item, int index)
{
    Node *new_node = malloc(sizeof(Node));
    if(new_node != NULL) {
        new_node->item = item;
        
        Node *current = list->head, *previous = NULL;
        for(int i = 0; current != NULL && i < index; i++) {
            previous = current;
            current = current->next;
        }

        if(current != NULL || index == 0) {
            if(previous != NULL)
                previous->next = new_node;
            else
                list->head = new_node;

            if(list->size == 0)
                list->end = new_node;
            
            new_node->next = current;
            list->size++;
            return true;
        }

        free(new_node);
    }

    return false;
}

/*
 * Removes and returns a pointer to the item at the given index in the list. Returns NULL if the list is empty or if the index is out of bounds.
 */
void* list_remove_at(List *list, int index) 
{
    Node *current = list->head, *previous = NULL;
    for(int i = 0; current != NULL && i < index; i++) {
        previous = current;
        current = current->next;
    }

    void *item = NULL;
    if(current != NULL) {
        item = current->item;
        Node *next = current->next;

        if(current == list->head)
            list->head = next;

        if(current == list->end)
            list->end = previous;

        if(previous != NULL) 
            previous->next = next;

        list->size--;
        free(current);
    }

    return item;
}

/*
 * Removes and returns a pointer to the last item of the list. Returns NULL if the list is empty. In a singly linked list, this operation takes O(n) time and is just a wrapper for the function list_remove_at.
 */
void* list_pop(List *list) {
    return list_remove_at(list, list->size - 1);
}

/*
 * Removes and returns a pointer to the item with the given ID on the list. Returns NULL if the item is not on the list. The ID can be any data type. A function to verify if some item's ID is the correct one is expected.
 */
void* list_remove(List *list, void *id, bool (*compare_id)(void *item, void *id))
{
    Node *current = list->head, *previous = NULL;
    while(current != NULL) {
        if(compare_id(current->item, id)) {
            void *item = current->item;
            Node *next = current->next;

            if(current == list->head)
                list->head = next;

            if(current == list->end)
                list->end = previous;

            if(previous != NULL) 
                previous->next = next;

            list->size--;
            free(current);
            return item;
        }

        previous = current;
        current = current->next;
    }

    return NULL;
}

/*
 * Returns, without removing, the item with the given ID on the list. Returns NULLL if the item is not on the list. The ID can be any data type. A function to verify if some item's ID is the correct one is expected.
 */
void* list_find(List *list, void *id, bool (*compare_id)(void *item, void *id))
{
    Node *n = list->head;
    while(n != NULL) {
        if(compare_id(n->item, id))
            return n->item;

        n = n->next;
    }

    return NULL;
}

/*
 * Switches the items of in the indices i1 and i2 of the list. Returns true if the operation was successful or false otherwise.
 */
bool list_switch_items(List *list, int i1, int i2)
{
    Node *current = list->head, *n1 = NULL, *n2 = NULL;
    int i = (i1 >= i2) ? i1 : i2;

    for(int k = 0; k <= i && current != NULL; k++) {
        if(k == i1)
            n1 = current;
        if(k == i2)
            n2 = current;
        
        current = current->next; 
    }

    if(n1 != NULL && n2 != NULL) {
        void *item1 = n1->item;
        n1->item = n2->item;
        n2->item = item1;
        return true;
    }

    return false;
}

/*
 * Inverts the order of the list elements.
 */
void list_invert(List *list) {
    for(int i = 0; i < list->size/2; i++)
        list_switch_items(list, i, (list->size - i - 1));
}

/*
 * Prints all the items of the list using the given function.
 */
void list_print(List *list, void (*item_print)(void *item))
{
    Node *n = list->head;
    while(n != NULL) {
        item_print(n->item);
        n = n->next;
    }
}