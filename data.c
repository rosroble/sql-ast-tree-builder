//
// Created by rosroble on 15.11.22.
//

#include "data.h"
#include "sql.tab.h"
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
}

columnref* new_column_ref(columnref* prev, char* column_name) {
    columnref* cref = malloc(sizeof(columnref));
    if (cref) {
        cref->col_name = malloc(strlen(column_name) + 1);
        strcpy(cref->col_name, column_name);
        cref->next = prev;
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

statement* new_update_statement(char* table_name, set_value_list* sets, predicate* pred) {
    statement* basic = new_basic_statement(table_name, UPDATE);
    if (basic) {
        basic->stmt.update_stmt = malloc(sizeof(update_stmt));
        if (basic->stmt.update_stmt) {
            basic->stmt.update_stmt->set_value_list = sets;
            basic->stmt.update_stmt->predicate = pred;
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



set_value* new_set_value(columnref* col, literal* literal) {
    set_value* set_val = malloc(sizeof(set_value));
    if (set_val) {
        set_val->lit = literal;
        set_val->col = col;
    }
    return set_val;
}

set_value_list* new_set_value_list(set_value_list* prev, set_value* val) {
    set_value_list* list = malloc(sizeof(set_value_list));
    if (list) {
        list->setval = val;
        list->next = prev;
    }
    return list;
}

