sql:	sql.l sql.y data.h data.c
	bison -d sql.y -Wcounterexamples
	flex sql.l
	cc -o $@ sql.tab.c lex.yy.c data.c -lfl

