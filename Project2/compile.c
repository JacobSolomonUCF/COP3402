// Compiler Builder 1:
// Matthew Carlino
// Jacob Solomon
// Joseph Freeman
// Alex Arwin
// Ryan Rossbach

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");
    FILE* lexoutput = fopen("lexoutput", "w");
    if (input == NULL || output == NULL)
    {
	printf("Error in opening the file\n");
	exit(1);
    }
    
    printf("Entering lexer\n");
    // lexoutput will contain the lexed token values for the parser
    lexer(input, lexoutput);
    fclose(lexoutput);
    lexoutput = fopen("lexoutput", "r");
    printf("Returned from lexer\n");
    printf("Entering parser\n");
    parser(lexoutput, output); 
    printf("Returned from parser\n");
    
    printf("Back in compile.c\n");
    
    return 0;
}