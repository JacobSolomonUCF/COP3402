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

// Four registers
int sp; // stack pointer
int bp; // base pointer
int pc; // program counter
instruction ir; // current instruction register

int stack[MAX_STACK_HEIGHT];
instruction instructions[MAX_CODE_LENGTH];

//Function Prototype(s)
void readInput(FILE * input, int lines);
void fetchCycle();
void executeCycle();

int main()
{
    int lines = 0; //Number of instruction lines
    FILE * input = fopen("mcode.pl0", "r");
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

void fetchCycle(){
    ir = instructions[pc];
    pc = pc + 1;
}

void executeCycle() {
    switch (ir.op) {
	case LIT:
	    sp = sp + 1;
	    stack[sp] = ir.m;
	    break;
	case OPR:
	    
	case LOD:
	    sp = sp + 1;
	    stack[sp] = stack[base(l, bp) + ir.m];
	    break;
	case STO:
	    stack[base(l, bp) + ir.m] = stack[sp];
	    sp = sp - 1;
	    break;
	case CAL:
	    stack[sp + 1] = 0;
	    stack[sp + 2] = base(l, bp);
	    stack[sp + 3] = bp;
	    stack[sp + 4] = pc;
	    bp = sp + 1;
	    pc = ir.m;
	    break;
	case INC:
	    sp = sp + ir.m;
	    break;
	case JMP:
	    pc = sp + ir.m;
	    break;
	case JPC:
	    if (stack[sp] == 0)
	      pc = ir.m;
	    sp = sp - 1;
	    break;
	case SIO:
	  if (ir.m == 0)
	  else if (ir.m == 1)
	  else if (ir.m == 2)
	    
     
    }
}

