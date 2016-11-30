// Compiler Builder 1:
// Matthew Carlino
// Jacob Solomon
// Joseph Freeman
// Alex Arwin
// Ryan Rossbach

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"

int main(int argc, char* argv[])
{
    FILE *input;
    FILE *output;
    
    if (argc != 3)
    {
	printf("need 2 arguments: input and output\n");
	exit(1);
    }
    
    input = fopen(argv[1], "r");
    output = fopen(argv[2], "w");
        
    lexer(input); // To lexer
    parse(output); // To parser
    
    return 0;
}
