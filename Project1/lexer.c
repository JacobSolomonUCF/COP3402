// Compiler Builder 1:
// Matthew Carlino
// Jacob Solomon
// Joseph Freeman
// Alex Arwin
// Ryan Rossbach

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

typedef enum token
{
    nulsym = 1, identsym = 2, numbersym = 3, plussym = 4, minussym = 5,
    multsym = 6, slashsym = 7, oddsym = 8, eqsym = 9, neqsym = 10, lessym = 11, leqsym = 12,
    gtrsym = 13, geqsym = 14, lparentsym = 15, rparentsym = 16, commasym = 17, semicolonsym = 18,
    periodsym = 19, becomessym = 20, beginsym = 21, endsym = 22, ifsym = 23, thensym = 24,
    whilesym = 25, dosym = 26, callsym = 27, constsym = 28, varsym = 29, procsym = 30, writesym = 31,
    readsym = 32, elsesym = 33
}token_type;

// Function Prototype(s)
void readInput(FILE * input);
void printSource();
void printClean();

int main(int argc, char *argv[]) 
{
    FILE * input = fopen(argv[1], "r");
    if (input == NULL)
    {
	printf("Error in opening the file");
	exit(0);
    }
    readInput(input);
}

void readInput(FILE * input)
{
    while (!feof(input))
    {
    }
}
