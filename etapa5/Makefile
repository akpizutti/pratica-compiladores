#
# UFRGS - Compiladores B - Marcelo Johann - 2009/2 - Etapa 1
#
# Makefile for single compiler call
# All source files must be included from code embedded in scanner.l
# In our case, you probably need #include "hash.c" at the beginning
# and #include "main.c" in the last part of the scanner.l
#

etapa5: main.o lex.yy.o y.tab.o hash.o semantic.o tac.o
	gcc main.o lex.yy.o y.tab.o hash.o semantic.o tac.o -o etapa5

main.o: main.c 
	gcc -c main.c

semantic.o: semantic.c
	gcc -c semantic.c

hash.o: hash.c 
	gcc -c hash.c
	
tac.o: tac.c
	gcc -c tac.c

lex.yy.o: lex.yy.c 
	gcc -c lex.yy.c

y.tab.o: y.tab.c 
	gcc -c y.tab.c

y.tab.c: parser.y
	yacc -d parser.y 

lex.yy.c: scanner.l
	lex scanner.l

clean:
	rm lex.yy.c y.tab.c *.o etapa5
