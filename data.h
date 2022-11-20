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
typedef struct statement statement;
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

struct statement {
    int stmt_type;
    char* table_name;
    union {
        select_stmt *select_stmt;
        insert_stmt *insert_stmt;
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
predicate* new_literal_predicate(columnref* col, int cmp_type, literal* liter);
predicate* new_reference_predicate(columnref* left, int cmp_type, columnref* right);
predicate* new_compound_predicate(predicate* left, int predicate_op, predicate* right);



#endif //SQLPARSER_DATA_H
