/**
 * Implementation of a singly linked list.
 * @author Gabriel Nogueira (Talendar).
 */


#include "singly_linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


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

    free(*list);
    (*list) = NULL;
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
    Node *n = calloc(1, sizeof(Node));
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
 * Removes the given node from the list.
 * 
 * @param list 
 * @param current a pointer to the node that will be removed; this node's memory will be freed.
 * @param previous a pointer to the node that precedes the node to be removed; pass NULL if there is none.
 * @return a pointer to the item stored in the removed node.
 */
static void* remove_node(List *list, Node *current, Node *previous) 
{
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

    if(current != NULL)
        return remove_node(list, current, previous);

    return NULL;
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
 * Seeks for a node with the given item. 
 * 
 * @param current a pointer to the variable that stores a pointer to the initial search node; by the end of the execution, the variable pointed by "current" will contain a pointer to the last node searched (the 1st node with the given item if it's found or NULL if the search failed).
 * @param previous a pointer to the variable that stores a pointer to the node that immediately precedes the initial search node; by the end of the execution, the variable pointed by "previous" will contain a pointer to the node the immediately precedes the last searched node (the list's end, if the search failed).
 * @param id some identifier of the searched item.
 * @param compare_id function used to verify the eligibility of an item; expected to return true if the item is the one being searched for and false otherwise.
 * @return -
 */
static void seek(Node **current, Node **previous, void *id, bool (*compare_id)(void *item, void *id)) 
{
    while((*current) != NULL) {
        if(compare_id((*current)->item, id))
            return;

        (*previous) = (*current);
        (*current) = (*current)->next;
    }
}


/**
 * Removes, from the list, all the nodes with an item that matches the given ID.
 * 
 * @param list 
 * @param id some identifier of the searched item.
 * @param compare_id function used to verify the eligibility of an item; expected to return true if the item is the one being searched for and false otherwise.
 * @param free_item function to free the memory allocated by the items of the removed nodes; pass NULL if it isn't necessary to free the items.
 * @return the number of nodes removed from the list. 
 */
int list_remove_all(List *list, void *id, bool (*compare_id)(void *item, void *id), void (*free_item)(void *item)) 
{   
    int count = 0;
    if(list->size > 0) {
        Node *current = list->head, *previous = NULL;
        seek(&current, &previous, id, compare_id);

        while(current != NULL) {
            count++;
            Node *next = current->next;
            void *i = remove_node(list, current, previous);  

            if(free_item != NULL && i != NULL)
                free_item(i);

            current = next;
            seek(&current, &previous, id, compare_id);
        }
    }

    return count;
} 


/**
 * Removes the first node found with an item of the given ID and returns a pointer to the item. 
 * 
 * @param list 
 * @param id some identifier of the searched item.
 * @param compare_id function used to verify the eligibility of an item; expected to return true if the item is the one being searched for and false otherwise.
 * @return a pointer to the item of the removed node or NULL if the item is not on the list.
 */
void* list_remove(List *list, void *id, bool (*compare_id)(void *item, void *id))
{
    Node *current = list->head, *previous = NULL;
    seek(&current, &previous, id, compare_id);
    
    if(current != NULL)
        return remove_node(list, current, previous);

    return NULL;
}


/**
 * Returns, without removing its node, the first item with the given ID on the list.
 * 
 * @param list 
 * @param id some identifier of the searched item.
 * @param compare_id function used to verify the eligibility of an item; expected to return true if the item is the one being searched for and false otherwise.
 * @return the first found item or NULLL if the item is not on the list.
 */
void* list_find(List *list, void *id, bool (*compare_id)(void *item, void *id))
{
    Node *current = list->head, *previous = NULL;
    seek(&current, &previous, id, compare_id);
    
    if(current != NULL)
        return current->item;

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
 * Inverts the order of the list's elements.
 */
void list_invert(List *list) {
    for(int i = 0; i < list->size/2; i++)
        list_switch_items(list, i, (list->size - i - 1));
}


/**
 * Returns the head node of the list. Returns NULL if the list is empty.
 */
Node* list_head(List *list) {
    return list->head;
}


/**
 * Returns the last node of the list. Returns NULL if the list is empty.
 */
Node* list_end(List *list) {
    return list->end;
}


/**
 * Returns the node pointed by n (its next node). If n is the last node of the list, NULL is returned. This function can be used to help iterating through the list.
 */
Node* list_next_node(Node *n) {
    return n->next;
}


/**
 * Returns the item stored by the node n.
 */
void* list_node_item(Node *n) {
    return n->item;
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