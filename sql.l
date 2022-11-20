%option noyywrap
%option caseless

%{
#include <stdio.h>
#include <string.h>
#include "data.h"
#include "sql.tab.h"
void showError();
%}


numbers ([0-9])+
alpha ([a-zA-Z]+)
word ([a-zA-Z0-9_]+)


%%

"*"         {return ALL;}
"select"    {return SELECT;}
"insert"    {return INSERT;}
"update"    {return UPDATE;}
"create"    {return CREATE;}
"delete"    {return DELETE;}
"from"      {return FROM;}
"where"     {return WHERE;}
"into"      {return INTO;}
"values"    {return VALUES;}
"and"       {yylval.predicate_op = 1; return AND;}
"or"        {yylval.predicate_op = 2; return OR;}
">"         {yylval.cmp = 1; return CMP;}
"<"         {yylval.cmp = 2; return CMP;}
"!="        {yylval.cmp = 3; return CMP;}
"="         {yylval.cmp = 4; return CMP;}
">="        {yylval.cmp = 5; return CMP;}
"<="        {yylval.cmp = 6; return CMP;}
"("         {return LP;}
")"         {return RP;}
"'"         {return QUOTE;}
";"         {return SEMICOLON;}
"."         {return DOT;}
","         {return COMMA;}

{alpha}     {sscanf(yytext, "%s", yylval.name); return (STRING);}
{numbers}   {yylval.number = atoi(yytext); return (NUM);}
[ \t]   { /* ignore white space */ }
\n          {return EOL; }
.           {showError(); return (OTHER);}


%%


void showError() {
    printf("Other input");
}