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
#include <ctype.h>

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
int tokens[10000];

//Number of chars in the input file
int numOfChars = 0;
int numOfTokens = 0;

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
	
	// runs through input array and stores token values in tokens array
	for (i = 0; i < numOfChars; i++)
	{
	    // continues past comments
	    if (fileInput[i] == '/' && fileInput[i + 1] == '*')
	    {
		j = i + 1;

		while (!(fileInput[j] == '*' && fileInput[j + 1] == '/'))
		    j++;

		i = j + 1;
		continue;
	    }

	    // continues past white space
	    if (fileInput[i] == ' ')
		continue;

	    // continues past newline
	    if (fileInput[i] == '\n')
		continue;

	    // continues past tab space
	    if (fileInput[i] == '\t')
		continue;

	    // puts reserved words token values in tokens array
	    if (isalpha(fileInput[i]))
	    {
		switch (fileInput[i])
		{
		    case 'n':
			if ((fileInput[i + 1] == 'u') &&
			    (fileInput[i + 2] == 'l') &&
			    (fileInput[i + 3] == 'l'))
			{
			    i = i + 3;
			    tokens[numOfTokens] = nulsym;
			    numOfTokens++;
			    continue;
			}

		    case 'b':
			if ((fileInput[i + 1] == 'e') &&
			    (fileInput[i + 2] == 'g') &&
			    (fileInput[i + 3] == 'i') &&
			    (fileInput[i + 4] == 'n'))
			{
			    i = i + 4;
			    tokens[numOfTokens] = beginsym;
			    numOfTokens++;
			    continue;
			}

		    case 'c':
			if ((fileInput[i + 1] == 'a') &&
			    (fileInput[i + 2] == 'l') &&
			    (fileInput[i + 3] == 'l'))
			    {
				i = i + 3;
				tokens[numOfTokens] = callsym;
				numOfTokens++;
				continue;
			    }
			else if ((fileInput[i + 1] == 'o') &&
			    (fileInput[i + 2] == 'n') &&
			    (fileInput[i + 3] == 's') &&
			    (fileInput[i + 4] == 't'))
			    {
				i = i + 4;
				tokens[numOfTokens] = constsym;
				numOfTokens++;
				continue;
			    }

		    case 'd':
			if (fileInput[i + 1] == 'o')
			{
			    i = i + 1;
			    tokens[numOfTokens] = dosym;
			    numOfTokens++;
			    continue;
			}

		    case 'e':
			if ((fileInput[i + 1] == 'l') &&
			    (fileInput[i + 2] == 's') &&
			    (fileInput[i + 3] == 'e'))
			    {
				i = i + 3;
				tokens[numOfTokens] = elsesym;
				numOfTokens++;
				continue;
			    }
			else if ((fileInput[i + 1] == 'n') &&
			    (fileInput[i + 2] == 'd'))
			    {
				i = i + 2;
				tokens[numOfTokens] = endsym;
				numOfTokens++;
				continue;
			    }

		    case 'i':
			if (fileInput[i + 1] == 'f')
			{
			    i = i + 1;
			    tokens[numOfTokens] = ifsym;
			    numOfTokens++;
			    continue;
			}

		    case 'o':
			if ((fileInput[i + 1] == 'd') &&
			    (fileInput[i + 2] == 'd'))
			{
			    i = i + 2;
			    tokens[numOfTokens] = oddsym;
			    numOfTokens++;
			    continue;
			}

		    case 'p':
			if ((fileInput[i + 1] == 'r') &&
			    (fileInput[i + 2] == 'o') &&
			    (fileInput[i + 3] == 'c') &&
			    (fileInput[i + 4] == 'e') &&
			    (fileInput[i + 5] == 'd') &&
			    (fileInput[i + 6] == 'u') &&
			    (fileInput[i + 7] == 'r') &&
			    (fileInput[i + 8] == 'e'))
			{
			    i = i + 8;
			    tokens[numOfTokens] = procsym;
			    numOfTokens++;
			    continue;
			}

		    case 'r':
			if ((fileInput[i + 1] == 'e') &&
			    (fileInput[i + 2] == 'a') &&
			    (fileInput[i + 3] == 'd'))
			{
			    i = i + 3;
			    tokens[numOfTokens] = readsym;
			    numOfTokens++;
			    continue;
			}

		    case 't':
			if ((fileInput[i + 1] == 'h') &&
			    (fileInput[i + 2] == 'e') &&
			    (fileInput[i + 3] == 'n'))
			{
			    i = i + 3;
			    tokens[numOfTokens] = thensym;
			    numOfTokens++;
			    continue;
			}

		    case 'v':
			if ((fileInput[i + 1] == 'a') &&
			    (fileInput[i + 2] == 'r'))
			{
			    i = i + 2;
			    tokens[numOfTokens] = varsym;
			    numOfTokens++;
			    continue;
			}

		    case 'w':
			if ((fileInput[i + 1] == 'h') &&
			    (fileInput[i + 2] == 'i') &&
			    (fileInput[i + 3] == 'l') &&
			    (fileInput[i + 4] == 'e'))
			{
			    i = i + 4;
			    tokens[numOfTokens] = whilesym;
			    numOfTokens++;
			    continue;
			}
			else if ((fileInput[i + 1] == 'r') &&
			    (fileInput[i + 2] == 'i') &&
			    (fileInput[i + 3] == 't') &&
			    (fileInput[i + 4] == 'e'))
			{
			    i = i + 4;
			    tokens[numOfTokens] = writesym;
			    numOfTokens++;
			    continue;
			}

		    default:
			break;
		}
	    }
    
	}
}
