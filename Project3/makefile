compile: compile.o lexer.o parser.o
	gcc -o compile compile.o lexer.o parser.o

compile.o: compile.c lexer.h parser.h
	gcc -c compile.c

lexer.o: lexer.c lexer.h
	gcc -c lexer.c

parser.o: parser.c parser.h
	gcc -c parser.c