#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3

//Contains each mnemonic
char *const OP[] = {" ","LIT","OPR","LOD","STO","CAL","INC","JMP","JPC","SIO"};

//Contains each valid operations
char *const OPR[] = {"RET","NEG","ADD","SUB","MUL","DIV","ODD","MOD","EQL","NEQ","LSS","LEQ","GTR","GEQ"};

typedef struct {
    int op; // opcode
    int l;  // L
    int m;  // M
} instruction;

instruction instructions[MAX_CODE_LENGTH];

//Function Prototype(s)
void readInput(FILE * input, int lines);

int main()
{
    int lines = 0; //Number of instruction lines
    FILE * input = fopen("/Users/Jacob/Desktop/School/Fall16/COP3402/COP3402/Project1/Project1/mcode.pl0", "r");
    if(input == NULL){ //Checks for no file
        printf("Error in opening the file");
        exit(0);
    }
    readInput(input, lines); //Reads input form file, Converts it and outputs to screen
    
    return 0;
}

void readInput(FILE * input, int lines){
    
    //Cycles through the file until it reachs EOF. Saves each instruction to the struct
    while(!feof(input)){
        fscanf(input,"%d%d%d", &instructions[lines].op, &instructions[lines].l, &instructions[lines].m);
        lines++;
    }
    
    //Header for output
    printf("PL/0 code:\n\n");
    
    int i = 0;
    for(i=0; i <lines-1;i++){
        
        //Checks if the instruction is one that needs a level
        if(instructions[i].op == 3 ||instructions[i].op == 4 || instructions[i].op == 5  ){
            printf("%4d%4s%3d%4d\n", i,OP[instructions[i].op],instructions[i].l, instructions[i].m);
        }else{
            //Checks for Halt
            if(instructions[i].op == 9 && instructions[i].m == 2){
                printf("%4d%4s\n", i, "HLT");
            }else if(instructions[i].op == 2){              //Checks for OPR instruction and determine which operation it is.
                printf("%4d%4s\n", i, OPR[instructions[i].l]);
            }else{
                printf("%4d%4s%7d\n", i,OP[instructions[i].op], instructions[i].m);
            }
        }
        
    }
}

