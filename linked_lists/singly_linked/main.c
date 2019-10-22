/*
 * Simple program to test my singly linked list implementation.
 * Created by Gabriel Nogueira (Talendar).
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
            "  0 - Append\n"
            "  1 - Insert at index\n"
            "  2 - Remove at index\n"
            "  3 - POP\n"
            "  4 - Remove by ID\n"
            "  5 - Find by ID\n"
            "  6 - Switch items\n"
            "  7 - Invert\n"
            "  8 - Print\n"
            "  9 - Exit\n"
            "Enter an option: "
        );

        scanf(" %c", &opt);
        clear_screen();

        //no checking for the input validity is done
        switch(opt) {
            case '0': {
                printf("\nHow many integers would you like to add to the list?\n");
                int qnt; scanf(" %d", &qnt);
                
                printf("\nEnter %d integers:\n", qnt);
                for(int i = 0; i < qnt; i++) {
                    int *n = malloc(sizeof(int));
                    scanf(" %d", n);
                    list_append(list, n);
                }
                printf("\nDone.");
                break;
            }

            case '1': {
                printf("\nWhat integer would you to add to the list?\n");
                int *n = malloc(sizeof(int));
                scanf(" %d", n);
                
                printf("\nAt what index should the item be inserted?\n");
                int i; scanf(" %d", &i);

                if(list_insert_at(list, n, i))
                    printf("\nDone.");
                else
                    printf("\nOperation failure.");
                break;
            }

            case '2': {
                printf("\nWhat's the index of the item to be removed?\n");
                int i; scanf(" %d", &i);

                int *n = list_remove_at(list, i);
                if(n != NULL) {
                    printf("\nInteger {%d} removed from the index [%d] of the list.\n", *n, i);
                    free(n);
                }
                else
                    printf("\nOperation failure.");
                break;
            }

            case '3': {
                int *n = list_pop(list);
                if(n != NULL) {
                    printf("\nInteger {%d} popped from the list.", *n);
                    free(n);
                }
                else
                    printf("\nOperation failure.");
                break;
            }

            case '4': {
                printf("\nWhat integer would you like to remove from the list? Only the first found is removed.\n");
                int *id = malloc(sizeof(int)); scanf(" %d", id);
                int *n = list_remove(list, id, &compare_ints);

                if(n != NULL) {
                    printf("\nThe integer {%d} was removed from the list.", *n);
                    free(n);
                }
                else
                    printf("\nInteger not found on the list!");

                free(id);
                break;
            }

            case '5': {
                printf("\nWhat integer would you like to find on the list?\n");
                int *id = malloc(sizeof(int)); scanf(" %d", id);
                int *n = list_find(list, id, &compare_ints);

                if(n != NULL) 
                    printf("\nThe integer {%d} found the list.", *n);
                else
                    printf("\nInteger not found on the list!");

                free(id);
                break;
            }

            case '6': {
                printf("\nIn what indices are the integers you want to swap positions?\n");
                int i1, i2; scanf(" %d %d", &i1, &i2);

                if(list_switch_items(list, i1, i2))
                    printf("\nDone.");
                else
                    printf("\nOperation failure.");

                break;
            }

            case '7': 
                list_invert(list);
                printf("\nDone.");
                break;

            case '8':
                printf("\n[ "); list_print(list, &print_int); printf("]\n");
                printf("SIZE: %d\n", list_size(list));
                break;
        }

        if(opt != '9') {
            printf("\nPress ENTER to go back to the main menu.\n");
            clean_input_buffer(); getchar();
        }
    } while(opt != '9');

    printf("\nLeaving...\n");
    list_free(&list, &free);
    return 0;
}