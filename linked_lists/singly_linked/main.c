/**
 * Simple program to test my singly linked list implementation.
 * @author Gabriel Nogueira (Talendar).
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "singly_linked_list.h"


const bool USE_SCREEN_CLEANER = true; //clear the screen between menus; turn off in case of incompatibility


/* 
 * Clears the terminal.
 */
void clear_screen() {
    if(USE_SCREEN_CLEANER)
        system("@cls||clear");
    else
        printf("\n\n\n\n");
}


/*
 * Prints an integer.
 */
void print_int(void *integer) {
    printf("%d ", *((int*) integer));
}


/*
 * Cleans the input buffer.
 */
void clean_input_buffer() {
    char c;  while((c = getchar()) != '\n' && c != EOF);
}


/*
 * Returns true if the given integers are equal or false otherwise.
 */
bool compare_ints(void *a, void *b) {
    return *((int*) a) == *((int*) b);
}


// MAIN
int main(void) 
{
    List *list = list_create();

    char opt = -1;
    do {
        clear_screen(); 
        printf(
            "\n< Linked List tester by Talendar >\n"
            "  0 - Insertions\n"
            "  1 - Removals\n"
            "  2 - Find by ID\n"
            "  3 - Switch items\n"
            "  4 - Invert\n"
            "  5 - Print\n"
            "  6 - Exit\n"
            "Enter an option: "
        );

        scanf(" %c", &opt);
        clear_screen();

        switch(opt) {
            /* INSERTIONS */
            case '0': {
                printf(
                    "\n> Insertion methods:\n" 
                    "   [0] Append\n"
                    "   [1] Insert at index\n" 
                    "   [2] Enqueue\n"  
                    "   [3] Push\n"
                    "Enter an option: "
                );
                scanf(" %c", &opt);

                int qnt = 1;
                if(opt != '1') {
                    printf("\nHow many integers would you like to add to the list?\n");
                    scanf(" %d", &qnt);
                }

                printf("\nEnter %d integer(s):\n", qnt);
                for(int i = 0; i < qnt; i++) {
                    int *n = malloc(sizeof(int));
                    scanf(" %d", n);

                    //APPEND
                    if(opt == '0')
                        list_append(list, n);
                    //INSERT AT INDEX
                    else if(opt == '1') {
                         printf("\nAt what index should the item be inserted?\n");
                        int i; scanf(" %d", &i);

                        if(list_insert_at(list, n, i))
                            printf("\nDone.");
                        else
                            printf("\nOperation failure.");
                    }
                    //ENQUEUE
                    else if(opt == '2')
                        list_enqueue(list, n);
                    //PUSH
                    else if(opt == '3')
                        list_push(list, n);
                }

                break;
            }

            /* REMOVALS */
            case '1': {
                printf(
                    "\n> Removal methods:\n" 
                    "   [0] Remove by index\n"
                    "   [1] Pop\n" 
                    "   [2] Dequeue\n"  
                    "   [3] Remove by ID\n"
                    "Enter an option: "
                );
                scanf(" %c", &opt);

                //REMOVE BY INDEX
                if(opt == '0') {
                    printf("\nWhat's the index of the item to be removed?\n");
                    int i; scanf(" %d", &i);

                    int *n = list_remove_at(list, i);
                    if(n != NULL) {
                        printf("\nInteger {%d} removed from the index [%d] of the list.\n", *n, i);
                        free(n);
                    }
                    else
                        printf("\nOperation failure.");
                }
                //POP & DEQUEUE
                else if(opt == '1' || opt == '2') {
                    int *n = (opt == '1') ? list_pop(list) : list_dequeue(list);

                    if(n != NULL) {
                        printf("\nInteger {%d} %s from the list.", *n, 
                                            (opt == '1') ? "popped" : "dequeued");
                        free(n);
                    }
                    else
                        printf("\nOperation failure.");
                }
                //REMOVE BY ID
                else if(opt == '3') {
                    printf("\nWhat integer would you like to remove from the list?\n");
                    int id; scanf(" %d", &id);

                    printf("\nRemove the first found[0] or all found[1]?\n");
                    int rem_all;  scanf(" %d", &rem_all);
                    
                    // remove first found
                    if(!rem_all) {
                        int *n = list_remove(list, &id, &compare_ints);

                        if(n != NULL) {
                            printf("\nThe integer {%d} was removed from the list.", *n);
                            free(n);
                        }
                        else
                            printf("\nInteger not found on the list!");
                    }
                    // remove all found
                    else {
                        int count = list_remove_all(list, &id, &compare_ints, &free);
                        printf("\n%d items removed from the list.", count);
                    }
                }
                break;
            }

            /* FIND BY ID */
            case '2': {
                printf("\nWhat integer would you like to find on the list?\n");
                int id; scanf(" %d", &id);
                int *n = list_find(list, &id, &compare_ints);

                if(n != NULL) 
                    printf("\nThe integer {%d} is in the list.", *n);
                else
                    printf("\nInteger not found on the list!");

                break;
            }

            /* SWITCH ITEMS */
            case '3': {
                printf("\nIn what indices are the integers you want to swap positions?\n");
                int i1, i2; scanf(" %d %d", &i1, &i2);

                if(list_switch_items(list, i1, i2))
                    printf("\nDone.");
                else
                    printf("\nOperation failure.");

                break;
            }

            /* INVERT */
            case '4': 
                list_invert(list);
                printf("\nDone.");
                break;

            /* PRINT */
            case '5':
                printf("\n[ "); list_print(list, &print_int); printf("]\n");
                printf("SIZE: %d\n", list_size(list));
                break;
        }

        if(opt != '6') {
            printf("\nPress ENTER to go back to the main menu.\n");
            clean_input_buffer(); getchar();
        }
    } while(opt != '6');

    printf("\nLeaving...\n");
    list_free(&list, &free);
    return 0;
}