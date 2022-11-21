%{
#include <stdio.h>
#include "data.h"
#include "printer.h"
extern int yylex();
int yyerror(char* s);

%}


%token STRING NUM OTHER SEMICOLON COMMA DOT QUOTE EOL LP RP SET EQ ON JOIN
%token SELECT INSERT UPDATE DELETE CREATE DROP TABLE FROM INTO WHERE AND OR ALL VALUES
%token CMP
%token TYPE


%type <name> STRING
%type <name> tableref
%type <number> NUM
%type <number> CMP
%type <type> TYPE
%type <predicate_op> AND
%type <predicate_op> OR
%type <literal> literal
%type <lit_list> value_list
%type <colref> columnref
%type <colref> short_columnref
%type <colref> full_columnref
%type <coldef> column_def
%type <coldef> column_defs
%type <stmt> basic_select_stmt
%type <stmt> select_predicate_stmt
%type <stmt> select_join_stmt
%type <stmt> select_stmt
%type <stmt> insert_stmt
%type <stmt> update_stmt
%type <stmt> create_stmt
%type <stmt> drop_stmt
%type <stmt> stmt
%type <jstmt> join_stmt
%type <svallist> set_value_list
%type <sval> set_value
%type <predicate> trivial_predicate
%type <predicate> predicate

%left OR
%left AND

%union {
    char name[20];
    int number;
    int cmp;
    int predicate_op;
    int type;
    literal_list* lit_list;
    literal* literal;
    columnref* colref;
    columndef* coldef;
    set_value_list* svallist;
    set_value* sval;
    statement* stmt;
    join_stmt* jstmt;
    predicate* predicate;
}

%%

input: stmt SEMICOLON {print_stmt($1);}
;

stmt: 	select_stmt
|	insert_stmt
|	update_stmt
|	create_stmt
|	drop_stmt
/*
|	delete_stmt
*/
;


select_stmt: 	basic_select_stmt
|		select_join_stmt
|		select_predicate_stmt
;

select_predicate_stmt: 	basic_select_stmt WHERE predicate {$$ = add_predicate_statement($1, $3); }
|			select_join_stmt WHERE predicate {$$ = add_predicate_statement($1, $3); }
;

select_join_stmt: basic_select_stmt JOIN join_stmt {$$ = add_join_statement($1, $3);}
;

basic_select_stmt: SELECT columnref FROM tableref {$$ = new_basic_select_statement($4, $2);}
;

join_stmt: 	tableref ON predicate {$$ = new_join_stmt($1, $3);}
;

insert_stmt:	INSERT INTO tableref LP columnref RP VALUES LP value_list RP {$$ = new_insert_statement($3, $5, $9);}
;

update_stmt:	UPDATE tableref SET set_value_list { $$ = new_update_statement($2, $4, NULL); }
|		UPDATE tableref SET set_value_list WHERE predicate {$$ = new_update_statement($2, $4, $6); }
;

set_value_list:	set_value {$$ = new_set_value_list(NULL, $1); }
|		set_value COMMA set_value_list {$$ = new_set_value_list($3, $1); }
;

set_value: columnref EQ literal {$$ = new_set_value($1, $3); }
;

tableref: STRING
;

predicate: predicate AND predicate { $$ = new_compound_predicate($1, $2, $3); }
|	predicate OR predicate { $$ = new_compound_predicate($1, $2, $3); }
|	trivial_predicate
;

trivial_predicate: 	columnref CMP literal { $$ = new_literal_predicate($1, $2, $3); }
|			literal CMP columnref { $$ = new_literal_predicate($3, reverse_cmp($2), $1); }
|			columnref CMP columnref { $$ = new_reference_predicate($1, $2, $3); }
;

columnref: 	short_columnref COMMA columnref { $$ = $1; $1->next = $3; }
|		full_columnref COMMA columnref { $$ = $1; $1->next = $3; }
|		short_columnref
|		full_columnref
;

short_columnref: STRING { $$ = new_column_ref(NULL, $1, NULL); }
;

full_columnref: STRING DOT STRING { $$ = new_column_ref(NULL, $3, $1);}
;

create_stmt: CREATE TABLE tableref LP column_defs RP { $$ = new_create_statement($3, $5); }
;

drop_stmt: DROP TABLE tableref { $$ = new_drop_statement($3); }
;

column_defs: 	column_def COMMA column_defs { $$ = $1; $1->next = $3; }
|		column_def
;

column_def: STRING TYPE { $$ = new_column_def(NULL, $1, $2); }
;


value_list: literal { $$ = new_literal_list(NULL, $1); }
|	value_list COMMA literal { $$ = new_literal_list($1, $3); }
;

literal:	NUM { $$ = new_num_literal($1); }
|		QUOTE STRING QUOTE { $$ = new_str_literal($2); }
;
%%



int yyerror(char* s) {
	printf("Syntax error line %s\n", s);
	return 0;
}

int main() {
	yyparse();
	return 0;
}
