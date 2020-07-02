/**
 * Implementation of a singly linked list.
 * @author Gabriel Nogueira (Talendar).
 */


#ifndef SIMPLY_LINKED_LIST_H
    #define SIMPLY_LINKED_LIST_H
    #include <stdbool.h>
    
    /* Struct */
    typedef struct Node Node;
    typedef struct List List;

    /* Create/free */
    List* list_create(void);
    void list_free(List **list, void (*item_free)(void *item));

    /* Query */
    int list_size(List *list);
    bool list_empty(List *list);
    bool list_full(List *list);
    void* list_top(List *list);

    /* Insertion */
    bool list_append(List *list, void *item);
    bool list_insert_at(List *list, void *item, int index);
    bool list_enqueue(List *list, void *item);
    bool list_push(List *list, void *item);

    /* Removal */
    void* list_remove_at(List *list, int index);
    void* list_pop(List *list);
    void* list_dequeue(List *list);
    void* list_remove(List *list, void *id, bool (*compare_id)(void *item, void *id));
    int list_remove_all(List *list, void *id, bool (*compare_id)(void *item, void *id), void (*free_item)(void *item));

    /* Search */
    void* list_find(List *list, void *id, bool (*compare_id)(void *item, void *id));

    /* Operations on/with nodes */
    Node* list_head(List *list);
    Node* list_end(List *list);
    Node* list_next_node(Node *n);
    void* list_node_item(Node *n);

    /* Others */
    bool list_switch_items(List *list, int i1, int i2);
    void list_invert(List *list);
    void list_print(List *list, void (*item_print)(void *item));
#endif