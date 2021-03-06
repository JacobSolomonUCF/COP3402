//
//  parser.h
//  Project3COP3402
//
//  Created by Jacob Solomon on 11/16/16.
//  Copyright © 2016 COP3402. All rights reserved.
//

#ifndef parser_h
#define parser_h

#include <stdio.h>

void program();
void advance();
void block();
void statement();
void condition();;
void expression();
void term();
void factor();
void error(int error);
void put_symbol(int kind, char name[], int num, int level, int modifier);
void emit(int op, int l, int m);

void parser(int flag);

#endif /* parser_h */
