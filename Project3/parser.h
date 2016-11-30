// PL/0 COMPILER
// Matthew Carlino
// Jacob Solomon
// Joseph Freeman
// Alex Arwin
// Ryan Rossbach


#ifndef parser_h
#define parser_h
#include "tokens.h"

void parse(FILE *ofp);
void block(int lev, int tx);
void constdeclaration(int lev, int *ptx, int *pdx);
void vardeclaration(int lev, int *ptx, int *pdx);
void statement(int lev, int *ptx);
void condition(int lev, int *ptx);
void expression(int lev, int *ptx);
void term(int lev, int *ptx);
void factor(int lev, int *ptx);
void emit(int op, int l, int m);
void enter(int k, int *ptx, int *pdx, int lev);
void error(int errorCase);
void advance();
int position(char *id, int *ptx, int lev);
void program();

#endif
