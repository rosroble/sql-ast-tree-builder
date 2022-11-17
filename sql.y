%{
#include <stdio.h>
#include "data.h"
extern int yylex();
int yyerror(char* s);

%}


%token STRING NUM OTHER SEMICOLON COMMA DOT QUOTE
%token SELECT INSERT DELETE CREATE TABLE FROM INTO WHERE AND OR ALL VALUES
%token CMP


%type <name> STRING
%type <name> tableref
%type <number> NUM
%type <number> CMP
%type <predicate_op> AND
%type <predicate_op> OR
%type <literal> literal
%type <colref> columnref
%type <stmt> select_stmt
%type <predicate> trivial_predicate
%type <predicate> predicate

%left AND
%left OR

%union {
    char name[20];
    int number;
    int cmp;
    int predicate_op;
    literal* literal;
    columnref* colref;
    statement* stmt;
    predicate* predicate;
}

%%
stmt: /* empty */
|	select_stmt
/*
|	insert_stmt
|	create_stmt
|	delete_stmt
*/
;

select_stmt: 	SELECT columnref FROM tableref SEMICOLON {$$ = new_select_statement($4, $2, NULL);}
|		SELECT columnref FROM tableref WHERE predicate SEMICOLON {$$ = new_select_statement($4, $2, $6);}
;

tableref: STRING
;

predicate: predicate AND trivial_predicate { $$ = new_compound_predicate($1, $2, $3); }
|	predicate OR trivial_predicate { $$ = new_compound_predicate($1, $2, $3); }
|	trivial_predicate
;

trivial_predicate: 	columnref CMP literal { $$ = new_literal_predicate($1, $2, $3); }
|			columnref CMP columnref { $$ = new_reference_predicate($1, $2, $3); }
;

columnref: 	columnref COMMA STRING { $$ = new_column_ref($1, $3); }
|		STRING { $$ = new_column_ref(NULL, $1); }
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
