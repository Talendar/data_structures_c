/*
 * Singly linked list implementation.
 * Created by Gabriel Nogueira (Talendar).
 */

#ifndef SIMPLY_LINKED_LIST_H
    #define SIMPLY_LINKED_LIST_H
    #include <stdbool.h>
    
    typedef struct Node Node;
    typedef struct List List;

    List* list_create(void);
    void list_free(List **list, void (*item_free)(void *item));

    int list_size(List *list);
    bool list_empty(List *list);
    bool list_full(List *list);

    bool list_append(List *list, void *item);
    bool list_insert_at(List *list, void *item, int index);

    void* list_remove_at(List *list, int index);
    void* list_pop(List *list);

    void* list_remove(List *list, void *id, bool (*compare_id)(void *item, void *id));
    void* list_find(List *list, void *id, bool (*compare_id)(void *item, void *id));

    bool list_switch_items(List *list, int i1, int i2);
    void list_invert(List *list);
    void list_print(List *list, void (*item_print)(void *item));
#endif