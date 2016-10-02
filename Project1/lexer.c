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

// names of reserved words
char *keyword[] = {
    	"null", "begin", "call", "const", "do", "else", "end", "if",
    	"odd", "procedure", "read", "then", "var", "while", "write"
};


//This was giving compile errors. Commented out for now.
/*int keyword_type[] = {
    	nul, beginsym, callsym, constsym, dosym, elsesym, endsym,
    	ifsym, oddsym, procsym, readsym, thensym, varsym, whilesym, writesym
};*/

typedef enum token
{
    	nulsym = 1, identsym = 2, numbersym = 3, plussym = 4, minussym = 5,
    	multsym = 6, slashsym = 7, oddsym = 8, eqsym = 9, neqsym = 10, lessym = 11, leqsym = 12,
    	gtrsym = 13, geqsym = 14, lparentsym = 15, rparentsym = 16, commasym = 17, semicolonsym = 18,
    	periodsym = 19, becomessym = 20, beginsym = 21, endsym = 22, ifsym = 23, thensym = 24,
    	whilesym = 25, dosym = 26, callsym = 27, constsym = 28, varsym = 29, procsym = 30, writesym = 31,
    	readsym = 32, elsesym = 33
}token_type;

// chose arbitrary length 
// TODO find max input size that can be giving to us
char fileInput[10000];

//Number of chars in the input file
int numOfChars = 0;

// Function Prototype(s)
void readInput(FILE * input);
void printSource(char *argv[]); // print source code when --source used as argv
void printClean(char *argv[]); // print source code w/o comments when --clean used as argv

int main(int argc, char *argv[]) 
{
    FILE * input = fopen(argv[1], "r");
    if (input == NULL)
    	{
		printf("Error in opening the file");
		exit(0);
    	}
    readInput(input);
    printSource(argv);
    printClean(argv);
    
    
    
}

void printSource(char *argv[]){ //Prints the source input
    int flag = 0;
    
    if(argv[2] != NULL && strcmp(argv[2], "--source") == 0){ //Checks for --source in argv
        flag = 1;
    }else if(argv[3] != NULL && strcmp(argv[3], "--source") == 0){
        flag = 1;
    }
    
    if(flag == 1){
        printf("\nsource code:\n------------\n%s",fileInput);
    }
}

void printClean(char *argv[]){ //Prints the input without comments
    int flag = 0, i = 0;
    int cflag = 0;
    char cleanInput[numOfChars];
    
    if(argv[2] != NULL && strcmp(argv[2], "--clean") == 0){
        flag = 1;
    }else if(argv[3] != NULL && strcmp(argv[3], "--clean") == 0){
        flag = 1;
    }
    
    if(flag == 1){
        for(i =0; i<numOfChars; i++){
            if(fileInput[i] == '/' && fileInput[i+1] == '*'){ //Checks for start of comment
                cleanInput[i] = ' ';
                cleanInput[i+1] = ' ';
                i = i+2;
                
                while (cflag != 1) { //Cycles through putting blank space where the comment text is
                    if(fileInput[i] == '*' && fileInput[i+1] == '/'){ //Checks for closing comment
                        cflag = 1;
                        fileInput[i] = ' ';
                        fileInput[i+1] = ' ';
                    }else{
                        cleanInput[i] = ' ';
                        i++;
                    }
                    
                }
                
            }
            cleanInput[i] = fileInput[i];
        }
        printf("\nsource code without comments:\n------------\n%s",cleanInput);
    
    }
}
void readInput(FILE * input){
	
    	while (!feof(input))
    	{
		// puts every char from input file into the array
		fscanf(input, "%c", &fileInput[numOfChars++]);
    	}
    
    fclose(input);
}
