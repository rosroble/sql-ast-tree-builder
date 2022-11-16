//
// Created by rosroble on 15.11.22.
//

#include "data.h"

root* new_root(int stmt_type, char* table_name, statement stmt);
statement* new_statement(int stmt_type);
predicate* new_literal_predicate(char* column_name, int cmp_type);
predicate* new_reference_predicate(char* column_name_left, char* column_name_right, int cmp_type);
predicate* new_compound_predicate(predicate* left, predicate* right);