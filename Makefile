sql:	sql.l sql.y data.h data.c
	bison -d sql.y -Wcounterexamples
	flex sql.l
	cc -g -o $@ sql.tab.c lex.yy.c data.c -lfl

.PHONY: clean

clean:
	rm -f sql lex.yy.c sql.tab.*

