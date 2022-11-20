%{
#include <stdio.h>
#include "data.h"
extern int yylex();
int yyerror(char* s);

%}


%token STRING NUM OTHER SEMICOLON COMMA DOT QUOTE EOL LP RP
%token SELECT INSERT UPDATE DELETE CREATE TABLE FROM INTO WHERE AND OR ALL VALUES
%token CMP


%type <name> STRING
%type <name> tableref
%type <number> NUM
%type <number> CMP
%type <predicate_op> AND
%type <predicate_op> OR
%type <literal> literal
%type <lit_list> value_list
%type <colref> columnref
%type <stmt> select_stmt
%type <stmt> insert_stmt
%type <stmt> stmt
%type <predicate> trivial_predicate
%type <predicate> predicate

%left OR
%left AND

%union {
    char name[20];
    int number;
    int cmp;
    int predicate_op;
    literal_list* lit_list;
    literal* literal;
    columnref* colref;
    statement* stmt;
    predicate* predicate;
}

%%

input:
|	input stmt SEMICOLON EOL {print_stmt($2);}
|	input EOL
;

stmt: 	select_stmt {$$ = $1;}
|	insert_stmt {$$ = $1;}
/*
|	update_stmt
|	create_stmt
|	delete_stmt
*/
;

select_stmt: 	SELECT columnref FROM tableref {$$ = new_select_statement($4, $2, NULL);}
|		SELECT columnref FROM tableref WHERE predicate {$$ = new_select_statement($4, $2, $6); }
;

insert_stmt:	INSERT INTO tableref LP columnref RP VALUES LP value_list RP {$$ = new_insert_statement($3, $5, $9);}
;

tableref: STRING
;

predicate: predicate AND predicate { $$ = new_compound_predicate($1, $2, $3); }
|	predicate OR predicate { $$ = new_compound_predicate($1, $2, $3); }
|	trivial_predicate
;

trivial_predicate: 	columnref CMP literal { $$ = new_literal_predicate($1, $2, $3); }
|			columnref CMP columnref { $$ = new_reference_predicate($1, $2, $3); }
;

columnref: 	STRING COMMA columnref { $$ = new_column_ref($3, $1); }
|		STRING { $$ = new_column_ref(NULL, $1); }
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
