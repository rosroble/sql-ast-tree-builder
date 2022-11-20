//
// Created by rosroble on 15.11.22.
//

#ifndef SQLPARSER_DATA_H
#define SQLPARSER_DATA_H

typedef enum predicate_arg_type predicate_arg_type;
typedef enum predicate_type predicate_type;
typedef struct predicate_arg predicate_arg;
typedef struct predicate predicate;
typedef struct select_stmt select_stmt;
typedef struct insert_stmt insert_stmt;
typedef struct update_stmt update_stmt;
typedef struct statement statement;
typedef struct set_value set_value;
typedef struct set_value_list set_value_list;
typedef struct literal literal;
typedef struct literal_list literal_list;
typedef enum literal_type littype;
typedef struct columnref columnref;


enum predicate_arg_type {
    LITERAL, REFERENCE
};

enum predicate_type {
    TRIVIAL, COMPOUND
};

enum literal_type {
    STIRNG, NUMBER
};


struct columnref {
    char* table_name;
    char* col_name;
    columnref* next;
};

struct literal {
    littype type;
    union {
        int num;
        char* string;
    } value;
};

struct literal_list {
    literal* value;
    literal_list* next;
};

struct predicate_arg {
    predicate_arg_type type;
    union {
        literal* literal;
        columnref* ref;
        void* unknown;
    } arg;
};

struct predicate {
    predicate_type type;
    columnref* column;
    int cmp_type;
    predicate_arg arg;

    int predicate_op;
    predicate* left;
    predicate* right;
};

struct select_stmt {
    columnref* columns;
    predicate* predicate;
};

struct insert_stmt {
    columnref* columns;
    literal_list* literals;
};

struct set_value {
    columnref* col;
    literal* lit;
};

struct set_value_list {
    set_value* setval;
    set_value_list* next;
};

struct update_stmt {
    set_value_list* set_value_list;
    predicate* predicate;
};

struct statement {
    int stmt_type;
    char* table_name;
    union {
        select_stmt *select_stmt;
        insert_stmt *insert_stmt;
        update_stmt* update_stmt;
        // other types of statements;
    } stmt;
};

//typedef struct {
//    int stmt_type;
//    char* table_name;
//    statement stmt;
//} root;


// root* new_root(int stmt_type, char* table_name, statement stmt);
literal* new_num_literal(int num);
literal* new_str_literal(char* str);
literal_list* new_literal_list(literal_list* prev, literal* literal);
columnref* new_column_ref(columnref* prev, char* column_name);
statement* new_select_statement(char* table_name, columnref* colref, predicate* pred);
statement* new_insert_statement(char* table_name, columnref* cols, literal_list* vals);
statement* new_update_statement(char* table_name, set_value_list* sets, predicate* pred);
predicate* new_literal_predicate(columnref* col, int cmp_type, literal* liter);
predicate* new_reference_predicate(columnref* left, int cmp_type, columnref* right);
predicate* new_compound_predicate(predicate* left, int predicate_op, predicate* right);
set_value* new_set_value(columnref* col, literal* literal);
set_value_list* new_set_value_list(set_value_list* prev, set_value* val);


#endif //SQLPARSER_DATA_H
