sql:	sql.l sql.y
	bison -d sql.y -Wcounterexamples
	flex sql.l
	cc -o $@ sql.tab.c lex.yy.c -lfl

