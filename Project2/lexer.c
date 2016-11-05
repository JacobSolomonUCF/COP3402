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

typedef enum token
{
    	nulsym = 1, identsym = 2, numbersym = 3, plussym = 4, minussym = 5,
    	multsym = 6, slashsym = 7, oddsym = 8, eqsym = 9, neqsym = 10, lessym = 11, leqsym = 12,
    	gtrsym = 13, geqsym = 14, lparentsym = 15, rparentsym = 16, commasym = 17, semicolonsym = 18,
    	periodsym = 19, becomessym = 20, beginsym = 21, endsym = 22, ifsym = 23, thensym = 24,
    	whilesym = 25, dosym = 26, callsym = 27, constsym = 28, varsym = 29, procsym = 30, writesym = 31,
    	readsym = 32, elsesym = 33
}token_type;

char fileInput[10000];

//Number of chars in the input file
int numOfChars = 0;
//Current position in char array
int currentPos = 0;

int test;
int tflag = 0;
int singleLength;

// Function Prototype(s)
void readInput(FILE * input); //Reads input from file
void printSource(char *argv[]); // print source code when --source used as argv
char *inputClean(char *argv[], char cleanInput[]); // print source code w/o comments when --clean used as argv
char *singleToken(char cleanInput[]);
int tokenID(char *token);
int isID(char c);

int main(int argc, char *argv[])
{
    FILE * input = fopen(argv[1], "r");
    if (input == NULL)
    {
	printf("Error in opening the file");
	exit(0);
    }

    readInput(input); //Reads in the input from file
    printSource(argv); //Prints the --source

    char *cleanInput;
    cleanInput = (char*)malloc(numOfChars);
    cleanInput = inputClean(argv, cleanInput); //Generates the clean input

    printf("\ntokens:\n-------\n");
    char *single;
    while(currentPos < numOfChars){
        singleLength = 0;
        single = singleToken(cleanInput);

        // exits if identifer is longer than 12 characters
        if(strlen(single) > 12) {
            printf("Error: identifier \"%s\" too long\n", single);
            exit(0);
        }

        int i;
            if(tflag ==  0){
                test = tokenID(single);
                if(test != 0) {
                    for(i = 0; i < 12; i++)
			if (i < singleLength)
			    if(isID(single[i]))
				printf("%c", single[i]);
                            else
                                printf(" ");
			else
			printf(" ");
                    printf("%8d\n",test);
                }
            }
    }
}

// returns 1 c is [a-zA-Z0-9] OR an operator or other approved characters
int isID(char c) {

    switch(c) {
        case 'a'...'z':
            return 1;
        case '0'...'9':
            return 1;
        case '+':
            return 1;
        case '-':
            return 1;
        case '*':
            return 1;
        case '/':
            return 1;
        case '<':
            return 1;
        case '=':
            return 1;
        case '>':
            return 1;
        case ':':
            return 1;
        case '(':
            return 1;
        case ')':
            return 1;
        case '.':
            return 1;
        case ';':
            return 1;
        case ',':
            return 1;
        default:
            return 0;
    }

    return 0;
}

int tokenID(char* token){
    int ASCII = 0;
    int largestNum = 65535;
    int num = 0;
    int i = 0;
    for(i = 0; token[i]; i++){ //Converts to lower case
        token[i] = tolower(token[i]);
    }
    i = 0;
    switch (*token) {
        case '+':
            return plussym;
        case '-':
            return minussym;
        case '*':
            return multsym;
        case '/':
            return slashsym;
        case '<':
            if(token[i+1] == '=')
                return leqsym;
            else if(token[i+1] == '>')
                return neqsym;
            else
                return lessym;
        case '=':
            return eqsym;
        case '>':
            if(token[i+1] == '=')
                return geqsym;
            else
                return gtrsym;
        case ':':
            if(token[i+1] == '='){
                return becomessym;
            }else
                return 0;
        case '(':
            return lparentsym;
        case ')':
            return rparentsym;
        case ',':
            return commasym;
        case '.':
            return periodsym;
        case ';':
            return semicolonsym;
        case 'a'...'z':
            if(strcmp(token, "odd") == 0)
                return oddsym;
            if(strcmp(token, "begin") == 0)
                return beginsym;
            if(strcmp(token, "end")== 0)
                return endsym;
            if(strcmp(token, "if") == 0)
                return ifsym;
            if(strcmp(token, "then") == 0)
                return thensym;
            if(strcmp(token, "while") == 0)
                return whilesym;
            if(strcmp(token, "do") == 0)
                return dosym;
            if(strcmp(token, "call") == 0)
                return callsym;
            if(strcmp(token, "const") == 0)
                return constsym;
            if(strcmp(token, "var") == 0)
                return varsym;
            if(strcmp(token, "procedure") == 0)
                return procsym;
            if(strcmp(token, "write") == 0)
                return writesym;
            if(strcmp(token, "read") == 0)
                return readsym;
            if(strcmp(token, "else") == 0)
                return elsesym;
            return identsym;
        case '1'...'9':
            for(i = 0; i<strlen(token);i++){
                ASCII = (int)token[i];
                if(ASCII>=97 && ASCII<= 122){
                    printf("\nerror '%s' is invalid identifier\n", token);
                    exit(0);
                }
            }

            if(strlen(token) > 12)
                printf("Error: Identifier is too long");
            num = atoi(token);
            if(abs(num) > largestNum){
                printf("\nThe number '%d' is greater than 2^16-1 or less than -2^16-1\n", num);
                exit(0);
            }else{
                return numbersym;
            }
        default:
            break;
    }

    return 0;

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

char *inputClean(char *argv[], char cleanInput[]){ //Trims the comments from source code and prints to terminal if required

    int flag = 0, i = 0;
    int cflag = 0;
    char *clean;
    clean = (char*)malloc(numOfChars);

    if(argv[2] != NULL && strcmp(argv[2], "--clean") == 0){
        flag = 1;
    }else if(argv[3] != NULL && strcmp(argv[3], "--clean") == 0){
        flag = 1;
    }

    for(i =0; i<numOfChars; i++){
        if(fileInput[i] == '/' && fileInput[i+1] == '*'){ //Checks for start of comment
            clean[i] = ' ';
            clean[i+1] = ' ';
            i = i+2;

            while (cflag != 1) { //Cycles through putting blank space where the comment text is
                if(fileInput[i] == '*' && fileInput[i+1] == '/'){ //Checks for closing comment
                    cflag = 1;
                    fileInput[i] = ' ';
                    fileInput[i+1] = ' ';
                }else{
                    clean[i] = ' ';
                    i++;
                }

                }
            }
        clean[i] = fileInput[i];

    }
    if(flag == 1){
        printf("\nsource code without comments:\n------------\n%s",clean);
    }

    return (char*)clean;
}

void readInput(FILE * input){

    	while (!feof(input))
    	{
		// puts every char from input file into the array
		fscanf(input, "%c", &fileInput[numOfChars++]);
    	}

    fclose(input);
}

char *singleToken(char cleanInput[]){ //Trims the clean input to only 1 token
    tflag = 0;
    char *single;
    single = (char*)malloc(numOfChars);
    int i = 0;
    singleLength = 0;
    do{
        switch (cleanInput[currentPos]) {

            //The following cases all have the same output.
            case '(':
            case ')':
            case ',':
            case '.':
            case ';':
            case '+':
            case '-':
            case '*':
            case '/':
                if (i != 0) {
                    return (char*)single;
                }else{
		    singleLength = 1;
                    single[i] = cleanInput[currentPos];
                    currentPos++;
                    return (char*)single;
                }

            //The following cases have specific output based on the next char
            case '<':
                if (i != 0) {
                    return (char*)single;
                }else{
                    if (cleanInput[currentPos+1] != '=' || cleanInput[currentPos+1] != '>') {
                        single[i] = cleanInput[currentPos];
                        currentPos++;
			singleLength = 1;
                        return (char*)single;
                    }else{
			singleLength = 2;
                        single[i] = cleanInput[currentPos];
                        single[i+1] = cleanInput[currentPos+1];
                        currentPos = currentPos+2;
                        return (char*)single;
                    }
                }

            case ':':
                if (i != 0) {
                    return (char*)single;
                }else{
                    if (cleanInput[currentPos+1] != '=') {
                        single[i] = cleanInput[currentPos];
                        currentPos++;
                        singleLength = 1;
                        return (char*)single;
                    }else{
                        single[i] = cleanInput[currentPos];
                        single[i+1] = cleanInput[currentPos+1];
                        currentPos = currentPos+2;
                        singleLength = 2;
                        return (char*)single;
                    }
                }
            case '>':
                if (i != 0) {
                    return (char*)single;
                }else{
                    if (cleanInput[currentPos+1] != '=') {
                        singleLength = 1;
                        single[i] = cleanInput[currentPos];
                        currentPos++;
                        return (char*)single;
                    }else{
			singleLength = 2;
                        single[i] = cleanInput[currentPos];
                        single[i+1] = cleanInput[currentPos+1];
                        currentPos = currentPos+2;
                        return (char*)single;
                    }
                }

            //Skips the following cases
            case '\t':
                if(strcmp(single, "\t") == 0){
                    tflag = 1;
                    currentPos++;
                    return (char*)single;
                }
                currentPos++;
                return (char*)single;
            case '\r':
                if(strcmp(single, "\r") == 0){
                    tflag = 1;
                    currentPos++;
                    return (char*)single;
                }
                currentPos++;
                return (char*)single;
            case '\n':
                if(strcmp(single, "\n") == 0){
                    tflag = 1;
                    currentPos++;
                    return (char*)single;
                }
                currentPos++;
                return (char*)single;


            //Base case to handle all letters.
            case 'a'...'z':
            case 'A'...'Z':
            case '1'...'9':
                single[i] = cleanInput[currentPos];
                currentPos++;
                i++;
                singleLength++;
                break;
            default:
                tflag = 1;
                currentPos++;
                return (char*)single;

        }
    }while(cleanInput[currentPos] != ' ');

    currentPos++;
    return (char*)single;
}


