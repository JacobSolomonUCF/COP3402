//
//  lexer.h
//  Project3COP3402
//
//  Created by Jacob Solomon on 11/16/16.
//  Copyright © 2016 COP3402. All rights reserved.
//

#ifndef lexer_h
#define lexer_h

#include <stdio.h>

void readInput(FILE * input);
char *inputClean(char cleanInput[]);
char *singleToken(char cleanInput[]);
int tokenID(char *token);
int isID(char c);
void lexer(int l, FILE *input);

#endif /* lexer_h */
