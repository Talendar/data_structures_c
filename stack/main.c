/*
 * Simple program to test my stack implementation by converting infix expressions to postfix and prefix expressions.
 * Created by Gabriel Nogueira (Talendar).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "dynamic_stack.h"

#define MAX_OPERAND_SIZE 8
#define MAX_INPUT_SIZE 80
const char *const OPERATORS = "-+*/^";
const int OP_WEIGHTS[] = {1, 1, 2, 2, 3};

/*
 * Returns the weight of op1 minus the weight of op2.
 */
int precedence(char op1, char op2) 
{
    int op1_weight = 0, op2_weight = 0;
    for(int i = 0; i < strlen(OPERATORS); i++) {
        if(op1 == OPERATORS[i])
            op1_weight = OP_WEIGHTS[i];
        if(op2 == OPERATORS[i])
            op2_weight = OP_WEIGHTS[i];
    }

    return op1_weight - op2_weight;
}

/*
 * Returns true if c is an operator. Returns false otherwise.
 */
bool is_operator(char c) 
{
    for(int i = 0; i < strlen(OPERATORS); i++) {
        if(c == OPERATORS[i])
            return true;
    }
    return false;
}

/*
 * Returns the posfix version of an infix expression.
 */
char* infix_to_posfix(char *in)
{
    char *out = malloc(sizeof(char) * MAX_INPUT_SIZE);
    Stack *opstack = stack_create();
    int out_index = 0, op_read = 0;

    for(int i = 0; i < strlen(in); i++) 
    {
        //open parenthesis
        if(in[i] == '(') 
            stack_push(opstack, &in[i]);
        //operator or close parenthesis
        else if(is_operator(in[i]) || in[i] == ')') {
            op_read++;
            while( !stack_empty(opstack) && *((char*) stack_top(opstack)) != '(' 
                        && precedence(*((char*) stack_top(opstack)), in[i]) >= 0 ) {
                char *p = stack_pop(opstack);
                out[out_index++] = ' ';
                out[out_index++] = *p;
            }

            if(in[i] == ')') 
                stack_pop(opstack);
            else
                stack_push(opstack, &in[i]);
        }
        //operand
        else if(in[i] != '\n' && in[i] != ' ') {
            if(op_read >= 1) {
                out[out_index++] = ' ';
                op_read--;
            }

            out[out_index++] = in[i];
        }
    }

    while(!stack_empty(opstack)) {
        char *p = stack_pop(opstack);
        out[out_index++] = ' ';
        out[out_index++] = *p;
    }

    stack_free(&opstack, NULL);
    out[out_index] = '\0';
    return out;
}

/*
 * Returns the prefix version of an infix expression.
 */
char* infix_to_prefix(char *in)
{
    Stack *opstack = stack_create(), *varstack = stack_create();
    char *out = malloc(sizeof(char) * MAX_INPUT_SIZE);
    int out_index = MAX_INPUT_SIZE - 2, ops_read = 0;

    for(int i = strlen(in) - 1; i >= 0; i--) 
    {
        //open parenthesis (reverse)
        if(in[i] == ')') 
            stack_push(opstack, &in[i]);
        //operator or close parenthesis (reverse)
        else if(is_operator(in[i]) || in[i] == '(') {
            ops_read++;
            if( !stack_empty(opstack) && (precedence(*((char*) stack_top(opstack)), in[i]) > 0 || in[i] == '(') ) {
                stack_invert(varstack);
                while(!stack_empty(varstack)) {
                    char *v = stack_pop(varstack);
                    out[out_index--] = *v;
                }
                
                while( !stack_empty(opstack) && (precedence(*((char*) stack_top(opstack)), in[i]) > 0 
                        || (in[i] == '(' && *((char*) stack_top(opstack)) != ')')) ) {
                    char *p = stack_pop(opstack);
                    out[out_index--] = ' ';
                    out[out_index--] = *p;
                }
            }

            if(in[i] == '(') 
                stack_pop(opstack);
            else
                stack_push(opstack, &in[i]);
        }
        //operand
        else if(in[i] != '\n' && in[i] != ' ') {
            if(ops_read >= 1) {
                char *e = malloc(sizeof(char)), s = ' ';
                memcpy(e, &s, sizeof(char));
                stack_push(varstack, e);
                ops_read--;
            }

            stack_push(varstack, &in[i]);
        }

    }

    stack_invert(varstack);
    while(!stack_empty(varstack)) {
        char *v = stack_pop(varstack);
        out[out_index--] = *v;
    }

    while(!stack_empty(opstack)) {
        char *p = stack_pop(opstack);
        out[out_index--] = ' ';
        out[out_index--] = *p;
    }

    stack_free(&varstack, NULL);
    stack_free(&opstack, NULL);

    char *out2 = malloc(sizeof(char) * MAX_INPUT_SIZE);
    out_index++;
    int i;


    for(i = 0; out_index < MAX_INPUT_SIZE - 1; i++, out_index++)
        out2[i] = out[out_index];
    out2[i+1] = '\0';

    free(out);
    return out2;
}

//MAIN
int main(void)
{
    printf("\nEnter an INFIX expression:\n");
    char in[MAX_INPUT_SIZE];  fgets(in, MAX_INPUT_SIZE, stdin);
    
    printf("\nDo you want convert it to the POSFIX[1] notation or to the PREFIX[2] notation?\n");
    char opt = -1;  scanf(" %c", &opt);

    char *out;
    if(opt == '1')
        out = infix_to_posfix(in);
    else if(opt == '2')
        out = infix_to_prefix(in);
    else {
        printf("Invalid option!\n");
        return 0;
    }
    
    printf("\nRESULT:\n%s\n\n", out);
    free(out);
    return 0;
}