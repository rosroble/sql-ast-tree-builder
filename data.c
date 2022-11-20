//
// Created by rosroble on 15.11.22.
//

#include "data.h"
#include "sql.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAB_PRINTF(tabs, ...) do{ print_tabs(tabs), fprintf(stdout, __VA_ARGS__ ); } while(0)


static int tabs = 0;

void print_tabs(int amount) {
    for (int i = 0; i < amount; ++i) {
        printf("\t");
    }
}


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
}

columnref* new_column_ref(columnref* prev, char* column_name) {
    columnref* cref = malloc(sizeof(columnref));
    if (cref) {
        cref->col_name = malloc(strlen(column_name) + 1);
        strcpy(cref->col_name, column_name);
        cref->next = prev;
        if (prev) {
            // prev->next = cref;
        }
    }
    return cref;
}

statement* new_basic_statement (char* table_name, int type) {
    statement *basic = malloc(sizeof(statement));
    if (basic) {
        basic->table_name = malloc(strlen(table_name) + 1);
        strcpy(basic->table_name, table_name);
        basic->stmt_type = type;
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

statement* new_insert_statement(char* table_name, columnref* cols, literal_list* vals) {
    statement* basic = new_basic_statement(table_name, INSERT);
    if (basic) {
        basic->stmt.insert_stmt = malloc(sizeof (insert_stmt));
        if (basic->stmt.insert_stmt) {
            basic->stmt.insert_stmt->columns = cols;
            basic->stmt.insert_stmt->literals = vals;
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

literal_list* new_literal_list(literal_list* prev, literal* literal) {
    literal_list* llist = malloc(sizeof(literal_list));
    llist->value = literal;
    llist->next = prev;
    return llist;
}

void cmp_type_to_str(int cmp_type, char str[3]) {
    const char* cmp_to_str[6] = {">", "<", "!=", "=", ">=", "<="};
    if (cmp_type >= 1 && cmp_type <= 6) {
        strcpy(str, cmp_to_str[cmp_type - 1]);
    } else {
        str[0] = '\0';
    }
}

void predicate_op_to_str(int predicate_op, char str[4]) {
    const char* predop_to_str[2] = {"AND", "OR"};
    if (predicate_op >= 1 && predicate_op <= 2) {
        strcpy(str, predop_to_str[predicate_op - 1]);
    } else {
        str[0] = '\0';
    }
}

void print_predicate_arg_literal(predicate_arg* arg) {
    switch (arg->arg.literal->type) {
        case STIRNG:
            TAB_PRINTF(++tabs, "type: literal_str,\n");
            TAB_PRINTF(tabs, "value: %s\n", arg->arg.literal->value.string);
            break;
        case NUMBER:
            TAB_PRINTF(++tabs,"type: literal_num,\n");
            TAB_PRINTF(tabs, "value: %d\n", arg->arg.literal->value.num);
            break;
    }
}

void print_predicate_arg_ref(predicate_arg* arg) {
    TAB_PRINTF(++tabs, "type: reference,\n");
    TAB_PRINTF(tabs, "value: %s\n", arg->arg.ref->col_name);
}

void print_predicate_arg(predicate_arg* arg) {
    printf("{\n");
    switch (arg->type) {
        case LITERAL:
            print_predicate_arg_literal(arg);
            break;
        case REFERENCE:
            print_predicate_arg_ref(arg);
            break;
    }
    TAB_PRINTF(--tabs, "}\n");
}

void print_trivial_predicate(predicate* pred) {
    if (pred == NULL) return;
    char cmp_type_str[3];
    cmp_type_to_str(pred->cmp_type, cmp_type_str);
    printf("{\n");
    print_tabs(++tabs);
    printf("type: trivial,\n");
    print_tabs(tabs);
    printf("column: %s,\n", pred->column->col_name);
    print_tabs(tabs);
    printf("cmp_type: %s,\n", cmp_type_str);
    print_tabs(tabs);
    printf("predicate_arg: ");
    print_predicate_arg(&pred->arg);
//    printf("type: trivial, column: %s, cmp_type: %s, predicate_arg: ",
//           pred->column->col_name,
//           cmp_type_str);
    print_tabs(--tabs);
    printf("}");
}

void print_compound_predicate(predicate* pred) {
    if (pred == NULL) return;
    char pred_op_as_str[4];
    predicate_op_to_str(pred->predicate_op, pred_op_as_str);
    printf("{\n");
    TAB_PRINTF(++tabs, "type: compound\n");
    TAB_PRINTF(tabs, "binop: %s, \n", pred_op_as_str);
    TAB_PRINTF(tabs, "left: ");
    print_predicate(pred->left);
    printf(", \nright: ");
    print_predicate(pred->right);
    TAB_PRINTF(--tabs, "}\n");
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

void print_literal(literal* lit) {
    TAB_PRINTF(tabs, "{\n");
    const char* literals_to_str[2] = {"STIRNG", "NUMBER"};
    TAB_PRINTF(++tabs, "type: %s,\n", literals_to_str[lit->type]);
    switch (lit->type) {
        case STIRNG:
            TAB_PRINTF(tabs, "value: %s\n", lit->value.string);
            break;
        case NUMBER:
            TAB_PRINTF(tabs, "value: %d\n", lit->value.num);
        default:
            break;
    }
    TAB_PRINTF(--tabs, "}");
}

void print_columns(columnref* ref) {
    printf("[\n");
    print_tabs(++tabs);
    while (ref && ref->next) {
        printf("%s, \n", ref->col_name);
        print_tabs(tabs);
        ref = ref->next;
    }
    printf("%s\n", ref->col_name);
    print_tabs(--tabs);
    printf("]\n");
}

void print_values(literal_list* list) {
    printf("[\n");
    while (list && list->next) {
        print_literal(list->value);
        printf(",\n");
        list = list->next;
    }
    print_literal(list->value);
    printf("\n");
    TAB_PRINTF(tabs, "]\n");
}

void print_select_stmt(statement* stmt) {
    printf("{\n");
    print_tabs(++tabs);
    printf("type: select\n");
    print_tabs(tabs);
    printf("table: %s\n", stmt->table_name);
    print_tabs(tabs);
    printf("columns: ");
    print_columns(stmt->stmt.select_stmt->columns);
    print_tabs(tabs);
    printf("predicate: ");
    print_predicate(stmt->stmt.select_stmt->predicate);
    print_tabs(--tabs);
    printf("}\n");
}

void print_insert_stmt(statement* stmt) {
    TAB_PRINTF(tabs, "{\n");
    TAB_PRINTF(++tabs, "type: insert,\n");
    TAB_PRINTF(tabs, "table: %s,\n", stmt->table_name);
    TAB_PRINTF(tabs, "columns: ");
    print_columns(stmt->stmt.insert_stmt->columns);
    TAB_PRINTF(tabs, "values: ");
    print_values(stmt->stmt.insert_stmt->literals);
    TAB_PRINTF(--tabs, "}\n");
}

void print_stmt(statement* stmt) {
    switch (stmt->stmt_type) {
        case SELECT:
            print_select_stmt(stmt);
            break;
        case INSERT:
            print_insert_stmt(stmt);
            break;
        case UPDATE:
           // print_update_stmt();
            break;
        case DELETE:
           // print_delete_stmt();
            break;
        case CREATE:
           // print_create_stmt();
        default:
            break;
    }
}