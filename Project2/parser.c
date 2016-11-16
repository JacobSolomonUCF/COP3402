// Compiler Builder 1:
// Matthew Carlino
// Jacob Solomon
// Joseph Freeman
// Alex Arwin
// Ryan Rossbach

#include "parser.h"
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

typedef struct{
    int id;
    char name[13];
}sToken;

symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];
instruction instructions[MAX_SYMBOL_TABLE_SIZE*3];
sToken singletoken;

FILE *input;

void program();
void advance();
void block();
void statement();
void condition();
void expression();
void term();
void factor();
void error(int error);


void parser(int flag)
{
    input = fopen("/Users/Jacob/Desktop/Old/Project3COP3402/Project3COP3402/lexoutput", "r");
    FILE *output = fopen("/Users/Jacob/Desktop/Old/Project3COP3402/Project3COP3402/parseroutput", "w");
    if (input == NULL)
    {
        printf("Error in opening the file");
        exit(0);
    }
    
    //readIn(input); //Reads in the input from file
    
    // main calls program
    program();
    
    printf("\nProgram is Correct\n");
    
    
    fclose(input);
    fclose(output);
}

void program(){
    
    advance();
    block();
    if (singletoken.id != periodsym)
        error(9);
    
}

void advance(){
    fscanf(input, "%d", &singletoken.id);
    if (singletoken.id == 2 || singletoken.id
         == 3) {
        fscanf(input ,"%s", singletoken.name);
    }
}
void block(){
    
    if(singletoken.id == constsym) {
        do {
            advance();
            if(singletoken.id != identsym)     // Error: expected identifer in constant declaration
                error(4);
            advance();
            if(singletoken.id != eqsym)
                error(3);            // Expected '=' after
            advance();
            if(singletoken.id != numbersym)
                error(2);
            advance();
        } while(singletoken.id == commasym);
        
        if(singletoken.id != semicolonsym)
            error(5);
        advance();
    }
    
    if(singletoken.id == varsym) {
        do {
            advance();
            
            if(singletoken.id != identsym)
                error(4);            // Expected identifer in variable declaration
            advance();
        } while(singletoken.id == commasym);
        
        if(singletoken.id != semicolonsym)
            error(5);                // Expected ';' at the end of variable declaration
        advance();
    }
    
    while(singletoken.id == procsym) {
        advance();
        
        if(singletoken.id != identsym)
            error(6);
        advance();
        
        if(singletoken.id != semicolonsym)
            error(10);
        advance();
        block();
        
        if(singletoken.id!= semicolonsym)
            error(10);
        advance();
    }
    
    statement();
    
}
void statement()
{
    if (singletoken.id == identsym) {
        advance();
        if (singletoken.id != becomessym)
            error(3);
        advance();
        expression();
    }
    
    else if(singletoken.id == callsym) {
        advance();
        if(singletoken.id != identsym)
            error(14);
        advance();
    }
    
    else if (singletoken.id == beginsym) {
        advance();
        statement();
        
        while (singletoken.id == semicolonsym) {
            advance();
            statement();
        }
        
        if(singletoken.id != endsym)
            error(0);
        advance();
    }
    
    else if (singletoken.id == ifsym) {
        advance();
        condition();
        
        if (singletoken.id != thensym)
            error(16);
        advance();
        statement();
    }
    
    else if (singletoken.id == whilesym) {
        advance();
        condition();
        
        if (singletoken.id != dosym)
            error(18);
        advance();
        statement();
    }
    
    else if (singletoken.id == readsym) {
        advance();
        if (singletoken.id != identsym)
            error(0);
        advance();
    }
    
    else if (singletoken.id == writesym) {
        advance();
        if (singletoken.id != identsym)
            error(0);
        advance();
    }
}
void condition()
{
    if (singletoken.id == oddsym) {
        advance();
        expression();
    }
    
    else {
        expression();
        
        // condensed rel_op into this switch statement
        switch (singletoken.id)
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

void expression()
{
    
    if(singletoken.id == plussym || singletoken.id == minussym)
        advance();
    term();
    while(singletoken.id == plussym || singletoken.id == minussym) {
        advance();
        term();
    }
}

void term()
{
    factor();
    
    while(singletoken.id == multsym || singletoken.id == slashsym) {
        advance();
        factor();
    }
}
void factor()
{
    
    if(singletoken.id == identsym)
        advance();
    
    else if(singletoken.id == numbersym)
        advance();
    
    else if(singletoken.id == lparentsym) {
        advance();
        expression();
        if(singletoken.id != rparentsym)
            error(22);
        advance();
    }
    
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


