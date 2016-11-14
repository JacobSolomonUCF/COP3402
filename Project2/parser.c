// Compiler Builder 1:
// Matthew Carlino
// Jacob Solomon
// Joseph Freeman
// Alex Arwin
// Ryan Rossbach

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "tokens.h"

#define MAX_SYMBOL_TABLE_SIZE 100

typedef struct
{
	int kind;	// const = 1, var = 2, proc = 3
	char name[12];  // name up to 11 chars
	int val;	// number (ASCII value)
	int level;	// L level
	int addr;	// M address
}symbol;

typedef struct
{
    int op;
    int l;
    int m;
}instruction;

symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];
instruction instructions[MAX_SYMBOL_TABLE_SIZE*3];

// function prototypes
void program();
void block();
void statement();
void condition();
void expression();
void term();
void factor();
void number();
void ident();
void digit();
void letter();
void error();
void advance();
void readIn(FILE * input);
int getToken();

// Global Variables
int fileInput[MAX_SYMBOL_TABLE_SIZE];
int numOfInstructions = 0;
int tokenCounter = 0;
int tok = 0;


void parser(FILE* input, FILE* vmoutput)
{       
    if (input == NULL)
    {
	printf("Error in opening the file");
	exit(0);
    }
    
    readIn(input); //Reads in the input from file
    
    // main calls program
    program();
    //program(input, symbol_table, instructions);
    
    // print pm0 code for vm
    int i;
    for (i = 0; i < numOfInstructions-1; i++)
    {
	fprintf(vmoutput, "%d %d %d\n", instructions[i].op, instructions[i].l, instructions[i].m);
    }
    
}

void readIn(FILE * input)
{ 
    while (!feof(input))
    {
	// puts every char from input file into the array
	fscanf(input, "%d", &fileInput[numOfInstructions++]);
    }
    
    fclose(input);
}

// Appendix D
void program()
{
    advance();
    block();
    if (tok != periodsym)
        error(9);
    // advance();
    // printf("Tiny PL/0 program is syntactically correct\n");
}

// Appendix D
void block()
{

    if(tok == constsym) {
        do {
            advance();
            if(tok != identsym)     // Error: expected identifer in constant declaration
                error(4);
            advance();
            if(tok != eqsym)
                error(3);            // Expected '=' after 
            advance();
            if(tok != numbersym)
                error(2);
            advance();
        } while(tok == commasym);

        if(tok != semicolonsym)
            error(5);
        advance();
    }

    if(tok == varsym) {
        do {
          advance();

          if(tok != identsym)
                error(4);            // Expected identifer in variable declaration
	  advance();
        } while(tok == commasym);

        if(tok != semicolonsym)
            error(5);                // Expected ';' at the end of variable declaration
        advance();
    }

    while(tok == procsym) {
        advance();

        if(tok != identsym)
            error(6);
        advance();

        if(tok != semicolonsym)
            error();
        advance();
        block();

        if(tok != semicolonsym)
            error();
        advance();
    }

    statement();

}

// Appendix D
void statement()
{
    if (tok == identsym) {
        advance();
        if (tok != becomessym)
            error(3);
        advance();
        expression();
    }

    else if(tok == callsym) {
        advance();
        if(tok != identsym)
            error(14);
        advance();
    }

    else if (tok == beginsym) {
        advance();
        statement();

        while (tok == semicolonsym) {
            advance();
            statement();
        }

        if(tok != endsym)
            error();
        advance();
    }

    else if (tok == ifsym) {
        advance();
        condition();

        if (tok != thensym)
            error(16);
        advance();
        statement();
    }

    else if (tok == whilesym) {
        advance();
        condition();

        if (tok != dosym)
            error(18);
        advance();
        statement();
    }

    else if (tok == readsym) {
        advance();
        if (tok != identsym)
            error();
        advance();
    }

    else if (tok == writesym) {
        advance();
        if (tok != identsym)
            error();
        advance();
    }
}

// Appendix D
void condition()
{
    if (tok == oddsym) {
        advance();
        expression();
    }

    else {
        expression();
	
	// condensed rel_op into this switch statement
	switch (tok)
	{
	    case eqsym:
		break;
	    case neqsym:
		break;
	    case lessym:
		break;
	    case leqsym:
		break;
	    case gtrsym:
		break;
	    case geqsym:
		break;
	    default:
		error(20);
	}
        advance();
        expression();
    }
}

// Still have a few functions I need to finish / double check
// Think the majority of them are pretty set though

// Appendix D .
void expression()
{

    if(tok == plussym || tok == minussym)
        advance();
    term();
    while(tok == plussym || tok == minussym) {
        advance();
        term();
    }
}

// Appendix D
void term()
{
    factor();

    while(tok == multsym || tok == slashsym) {
        advance();
        factor();
    }
}

// factor ::= ident | number | "(" expression ")" .
void factor()
{

    if(tok == identsym)
        advance();

    else if(tok == numbersym)
        advance();

    else if(tok == lparentsym) {
        advance();
        expression();
        if(tok != rparentsym)
            error(22);
	advance();
    }

}

// number ::= digit {digit}.
// can't be void if being called as a function in factor
// void number()
// {
// 
//     digit();
//     while(digit()) {
//         advance();
//     }
// 
// }

void advance()
{
	tok = fileInput[tokenCounter++];
}

void enter()
{
    
}

void error(int error)
{
    
    // list of errors:
    
    switch(error)
    {
	case 1: // use = instead of :=
	    printf("Error number 1, use = instead of :=\n");
	    break;
	case 2: // = must be followed by a number
	    printf("Error number 2, = must be followed by a number\n");
	    break;
	case 3: // Identifier must be followed by =
	    printf("Error number 3, identifier must be followed by =\n");
	    break;
	case 4: // 'const', 'var', 'procedure' must be followed by identifier
	    printf("Error number 4, 'const', 'var', 'procedure' must be followed by identifier\n");
	    break;
	case 5: // semicolon or comma missing
	    printf("Error number 5, semicolon or comma missing\n");
	    break;
	case 6: // incorrect symbol after procedure declaration
	    printf("Error number 6, incorrect symbol after procedure declaration\n");
	    break;
	case 7: // statement expected
	    printf("Error number 7, statement expected\n");
	    break;
	case 8: // incorrect symbol after statement part in block
	    printf("Error number 8, incorrect symbol after statement part in block\n");
	    break;
	case 9: // period is expected
	    printf("Error number 9, period is expected\n");
	    break;
	case 10: // semicolon between statements missing
	    printf("Error number 10, semicolon between statements missing\n");
	    break;
	case 11: // undeclared identifier
	    printf("Error number 11, undeclared identifier\n");
	    break;
	case 12: // assignment to constant or procedure is not allowed
	    printf("Error number 12, assignment to constant or procedure is not allowed\n");
	    break;
	case 13: // assignment operator expected
	    printf("Error number 13, assignment operator expected\n");
	    break;
	case 14: // 'call' must be followed by an identifier
	    printf("Error number 14, 'call' must be followed by an identifier\n");
	    break;
	case 15: // call of a constant or variable is meaningless
	    printf("Error number 15, call of a constant or variable is meaningless\n");
	    break;
	case 16: // 'then' expected
	    printf("Error number 16, 'then' expected\n");
	    break;
	case 17: // semicolon or '}' expected
	    printf("Error number 17, semicolon or '}' expected\n");
	    break;
	case 18: // 'do' expected
	    printf("Error number 18, 'do' expected\n");
	    break;
	case 19: // incorrect symbol following statement
	    printf("Error number 19, incorrect symbol following statement\n");
	    break;
	case 20: // relational operator expected
	    printf("Error number 20, relational operator expected\n");
	    break;
	case 21: // expression must not contain a procedure identifier
	    printf("Error number 21, expression must not contain a procedure identifier\n");
	    break;
	case 22: // right parenthesis missing
	    printf("Error number 22, right parenthesis missing\n");
	    break;
	case 23: // the preceding factor cannot begin with this symbol
	    printf("Error number 23, the preceding factor cannot begin with this symbol\n");
	    break;
	case 24: // an expression cannot begin with this symbol
	    printf("Error number 24, an expression cannot begin with this symbol\n");
	    break;
	case 25: // this number is too large
	    printf("Error number 25, this number is too large\n");
	    break;
	default:
	    printf("There has been an error\n");
    }
    
    exit(0);

}

