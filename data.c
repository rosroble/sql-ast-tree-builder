//
// Created by rosroble on 15.11.22.
//

#include "data.h"
#include "sql.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



predicate_arg new_predicate_arg(predicate_arg_type type, void* arg) {
    predicate_arg predarg;
    predarg.type = type;
    predarg.arg.unknown = arg;
    return predarg;
}

literal* new_num_literal(int num) {
    literal* lit = malloc(sizeof(literal));
    if (lit) {
        lit->type = NUMBER;
        lit->value.num = num;
    }
    return lit;
}

literal* new_str_literal(char* str) {
    literal* lit = malloc(sizeof (literal));
    if (lit) {
        lit->type = STIRNG;
        lit->value.string = malloc(strlen(str) + 1);
        strcpy(lit->value.string, str);
    }
    return lit;
};

columnref* new_column_ref(columnref* prev, char* column_name) {
    columnref* cref = malloc(sizeof(columnref));
    if (cref) {
        cref->col_name = malloc(strlen(column_name) + 1);
        strcpy(cref->col_name, column_name);
        cref->next = NULL;
        if (prev) {
            prev->next = cref;
        }
    }
    return cref;
}

statement* new_basic_statement (char* table_name, int type) {
    statement *basic = malloc(sizeof(statement));
    if (basic) {
        basic->table_name = malloc(strlen(table_name) + 1);
        strcpy(basic->table_name, table_name);
        basic->stmt_type = SELECT;
    }
    return basic;
}

statement* new_select_statement(char* table_name, columnref* colref, predicate* pred) {
    statement *basic = new_basic_statement(table_name, SELECT);
    if (basic) {
        basic->stmt.select_stmt = malloc(sizeof (select_stmt));
        if (basic->stmt.select_stmt) {
            basic->stmt.select_stmt->predicate = pred;
            basic->stmt.select_stmt->columns = colref;
        }
    }
    return basic;
}

predicate* new_literal_predicate(columnref* col, int cmp_type, literal* liter) {
    predicate* pred = malloc(sizeof(predicate));
    predicate_arg predarg = new_predicate_arg(LITERAL, liter);
    if (pred) {
        pred->type = TRIVIAL;
        pred->column = col;
        pred->cmp_type = cmp_type;
        pred->arg = predarg;
        pred->predicate_op = 0;
    }
    return pred;
}

predicate* new_reference_predicate(columnref* left, int cmp_type, columnref* right) {
    predicate* pred = malloc(sizeof(predicate));
    predicate_arg predarg = new_predicate_arg(REFERENCE, right);
    if (pred) {
        pred->type = TRIVIAL;
        pred->column = left;
        pred->cmp_type = cmp_type;
        pred->arg = predarg;
        pred->predicate_op = 0;
    }
    return pred;
}

predicate* new_compound_predicate(predicate* left, int predicate_op, predicate* right) {
    predicate* pred = malloc(sizeof(predicate));
    if (pred) {
        pred->type = COMPOUND;
        pred->column = NULL;
        pred->cmp_type = 0;
        pred->predicate_op = predicate_op;
        pred->left = left;
        pred->right = right;
    }
    return pred;
}

void cmp_type_to_str(int cmp_type, char str[3]) {
    switch (cmp_type) { // ask Yuko
        case 1:
            str[0] = '>'; str[1] = '\0';
            break;
        case 2:
            str[0] = '<'; str[1] = '\0';
            break;
        case 3:
            str[0] = '!'; str[1] = '='; str[2] = '\0';
            break;
        case 4:
            str[0] = '='; str[1] = '\0';
            break;
        case 5:
            str[0] = '>'; str[1] = '='; str[2] = '\0';
            break;
        case 6:
            str[0] = '<'; str[1] = '='; str[2] = '\0';
            break;
        default:
            str[0] = '\0';
    }
}

void predicate_op_to_str(int predicate_op, char str[3]) {
    switch (predicate_op) {
        case 1:
            str[0] = '&'; str[1] = '&'; str[2] = '\0';
            break;
        case 2:
            str[0] = '|'; str[1] = '|'; str[2] = '\0';
            break;
        default:
            str[0] = '\0';
    }

}

void print_predicate_arg_literal(predicate_arg* arg) {
    switch (arg->arg.literal->type) {
        case STIRNG:
            printf("[ PREDARG: { type: literal_str, value: %s } ", arg->arg.literal->value.string);
            break;
        case NUMBER:
            printf("[ PREDARG: { type: literal_num, value: %d } ", arg->arg.literal->value.num);
            break;
    }
}

void print_predicate_arg_ref(predicate_arg* arg) {
    printf("[ PREDARG: { type: reference, value: %s } ", arg->arg.ref->col_name);
}

void print_predicate_arg(predicate_arg* arg) {
    switch (arg->type) {
        case LITERAL:
            print_predicate_arg_literal(arg);
            break;
        case REFERENCE:
            print_predicate_arg_ref(arg);
            break;
    }
}

void print_trivial_predicate(predicate* pred) {
    if (pred == NULL) return;
    char cmp_type_str[3];
    cmp_type_to_str(pred->cmp_type, cmp_type_str);
    printf("[ PREDICATE: ");
    printf("{type: trivial, column: %s, cmp_type: %s, predicate_arg: ",
           pred->column->col_name,
           cmp_type_str);
    print_predicate_arg(&pred->arg);
    printf(" }]");
}

void print_compound_predicate(predicate* pred) {
    if (pred == NULL) return;
    char pred_op_as_str[3];
    predicate_op_to_str(pred->predicate_op, pred_op_as_str);
    printf("[ PREDICATE: ");
    printf("{type: compound, pred_op: %s, ", pred_op_as_str);
    printf("left: ");
    print_predicate(pred->left);
    printf(", right: ");
    print_predicate(pred->right);
}

void print_predicate(predicate* pred) {
    if (pred == NULL) return;
    switch (pred->type) {
        case TRIVIAL:
            print_trivial_predicate(pred);
            break;
        case COMPOUND:
            print_compound_predicate(pred);
            break;
    }
}