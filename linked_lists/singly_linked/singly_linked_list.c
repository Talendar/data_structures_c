/**
 * Implementation of a singly linked list.
 * Created by Gabriel Nogueira (Talendar).
 */


#include "singly_linked_list.h"
#include <stdio.h>
#include <stdlib.h>


/**
 * Struct of the nodes of a singly linked list. Only a pointer to the next node is stored.
 */ 
struct Node {
    void *item;
    Node *next;
};


/**
 * Struct of a singly linked list. A pointer to the last node of the list is kept in order to make appending to the list an O(1) operation.
 */
struct List {
    Node *head, *end;
    int size;
};


/**
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


/**
 * Frees the memory allocated by the list, its nodes and its items. Pass NULL as the argument of item_free if you don't want to free the memory of the items.
 * 
 * @param list double pointer to an allocated list; after the execution, the variable pointed by this argument will be set to NULL.
 * @return -
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


/**
 * Returns the size (number of items) of the list.
 */
int list_size(List *list) {
    return list->size;
}


/**
 * Returns true if the list is empty or false otherwise.
 */
bool list_empty(List *list) {
    return !list->size;
}


/**
 * In this implementation, the linked list size is limited only by the amount of memory available to the program during its execution. For this reason, this function always returns false (list not full) and is here only for completeness.
 */
bool list_full(List *list) {
    return false;
}


/**
 * Inserts an item at the end of the list.
 * 
 * @param list 
 * @param item pointer to the item that will be stored in the list's new node.
 * @return true if the operation was successful or false otherwise.
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


/**
 * Inserts an item at the given index of the list. This function cannot insert items in the end of the list, use list_append for that.
 * 
 * @param list
 * @param item pointer to the item that will be stored in the list's new node.
 * @param index index of the newly inserted item.
 * @return true if the operation was successful or false otherwise.
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


/**
 * Removes and returns a pointer to the item at the given index in the list. 
 * 
 * @param list
 * @param index index of the item to be removed.
 * @return NULL if the list is empty or if the index is out of bounds.
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


/**
 * Removes and returns a pointer to the first item (index 0) of the list. Wrapper for list_remove_at(0).
 * 
 * @param list
 * @return a pointer to the removed item or NULL if the list is empty.
 */
void* list_pop(List *list) {
    return list_remove_at(list, 0);
}


/**
 * Pushes an item into the list's start (wrapper for list_insert_at(0)).
 * 
 * @param list 
 * @param item 
 * @return if the operation was sucessful and false otherwise. 
 */
bool list_push(List *list, void *item) {
    return list_insert_at(list, item, 0);
}


/**
 * Appends an item to the end of the list. Wrapper for list_append(). 
 * 
 * @param list 
 * @param item 
 * @return if the operation was sucessful and false otherwise. 
 */
bool list_enqueue(List *list, void *item) {
    return list_append(list, item);
}


/**
 * Removes and returns a pointer to the first item (index 0) of the list. Wrapper for list_remove_at(0).
 * 
 * @param list 
 * @return a pointer to the removed item or NULL if the list is empty.
 */
void* list_dequeue(List *list) {
    return list_remove_at(list, 0);
}


/**
 * Removes and returns a pointer to the item with the given ID on the list.
 * 
 * @param list 
 * @param id identifies the item; can be any data type.
 * @param compare_id function to verify if some item's ID is the correct one.
 * @return a pointer to the item with the given ID or NULL if the item is not on the list.
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


/**
 * Returns, without removing, the item with the given ID on the list.
 * 
 * @param list 
 * @param id identifies the item; can be any data type.
 * @param compare_id function to verify if some item's ID is the correct one.
 * @return the item with the given ID or NULLL if the item is not on the list.
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


/**
 * Switches the items in the indices i1 and i2 of the list. Returns true if the operation was successful or false otherwise.
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


/**
 * Inverts the order of the list elements.
 */
void list_invert(List *list) {
    for(int i = 0; i < list->size/2; i++)
        list_switch_items(list, i, (list->size - i - 1));
}


/**
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