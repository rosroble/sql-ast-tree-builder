//
// Created by rosroble on 15.11.22.
//

#ifndef SQLPARSER_DATA_H
#define SQLPARSER_DATA_H

typedef enum predicate_arg_type predicate_arg_type;
typedef struct predicate_arg predicate_arg;
typedef struct predicate predicate;
typedef struct select_stmt select_stmt;
typedef struct statement statement;
typedef struct literal literal;
typedef enum literal_type littype;
typedef struct columnref columnref;


enum predicate_arg_type {
    LITERAL, REFERENCE
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

struct predicate_arg {
    predicate_arg_type type;
    union {
        literal* literal;
        columnref* ref;
    };
};

struct predicate {
    columnref* column;
    int cmp_type;
    predicate_arg arg;

    int predicate_op;
    predicate* next;
};

struct select_stmt {
    int column_count;
    char** columns; // maybe linked list ??
    predicate* predicate;
};

struct statement {
    int stmt_type;
    char* table_name;
    union {
        select_stmt *select_stmt;
        // other types of statements;
    } stmt;
} ;

//typedef struct {
//    int stmt_type;
//    char* table_name;
//    statement stmt;
//} root;


// root* new_root(int stmt_type, char* table_name, statement stmt);
literal* new_num_literal(int num);
literal* new_str_literal(char* str);
columnref* new_column_ref(columnref* prev, char* column_name);
statement* new_select_statement(char* table_name);
predicate* new_literal_predicate(columnref* col, int cmp_type, literal* liter);
predicate* new_reference_predicate(columnref* left, int cmp_type, columnref* right);
predicate* new_compound_predicate(predicate* left, int predicate_op, predicate* right);


#endif //SQLPARSER_DATA_H
