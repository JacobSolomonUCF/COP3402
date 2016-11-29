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
    FILE* input;
    char *inputFileName = malloc(sizeof(char) * 1024); //Input file name
    strcpy(inputFileName,"empty"); //Setting file name to empty
    
    int l = 0 , v = 0, a = 0, i = 0;
    
    if(argc != 0){
        for(i=1; i<argc; i++){
            if(strcmp(argv[i], "-l") == 0)
                l = 1;//print singletoken.iden list
            else if(strcmp(argv[i], "-a") == 0)
                a = 1;//print assembly code
            else if(strcmp(argv[i], "-v") == 0)
                v = 1; //print virtual machine exec. trace
            else
                strcpy(inputFileName,argv[i]); //optional input filename, if no filename we assume input is input.pl0
        }
        
        if (strcmp(inputFileName, "empty") == 0) {
            input = fopen("input.pl0","r");
        }else{
            input = fopen(inputFileName,"r");
        }
        
        printf("\nEntering Lexer\n");
        lexer(l, input); //To lexer
        printf("Finished Lexer\n\n");
        printf("Entering Parser\n");
        parse();
        printf("Finished Parser\n");
        
    
    }
    return 0;
}
