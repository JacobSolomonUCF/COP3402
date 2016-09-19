// Compiler Builder 1:
// Matthew Carlino
// Jacob Solomon
// 
//
//

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

int stack[MAX_STACK_HEIGHT];
instruction instructions[MAX_CODE_LENGTH];

// Four registers
int sp = 0; // stack pointer
int bp = 1; // base pointer
int pc = 0; // program counter
instruction ir; // current instruction register

stack[1] = 0;
stack[2] = 0;
stack[3] = 0;
ir.op = 0;
ir.l = 0;
ir.m = 0;

//Function Prototype(s)
void readInput(FILE * input, int lines);
void fetchCycle();
void executeCycle();
void OPR();
int base(int level, int b);

int main()
{
    int lines = 0; //Number of instruction lines
    FILE * input = fopen("mcode.pm0", "r");
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
	    OPR();
	    break;
	case LOD:
	    sp = sp + 1;
	    stack[sp] = stack[base(ir.l, bp) + ir.m];
	    break;
	case STO:
	    stack[base(ir.l, bp) + ir.m] = stack[sp];
	    sp = sp - 1;
	    break;
	case CAL:
	    stack[sp + 1] = 0;
	    stack[sp + 2] = base(ir.l, bp);
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
	    if (ir.m == 0) { // output
	    	//print stack[sp]
	    	sp = sp - 1;
	    }
	    else if (ir.m == 1) { // input
	    	sp = sp + 1;
	    	// read 
	    }
	    else if (ir.m == 2) { // halt
	    	
	    }
	    break;
    }
}

void OPR() {
    switch(ir.m) {
	case RET:
	    sp = bp - 1;
	    pc = stack[sp + 4];
	    bp = stack[sp + 3];
	    break;
	case NEG:
	    stack[sp] = -stack[sp];
	    break;
	case ADD:
	    sp = sp - 1;
	    stack[sp] = stack[sp] + stack[sp + 1];
	    break;
	case SUB:
	    sp = sp - 1;
	    stack[sp] = stack[sp] - stack[sp + 1];
	    break;
	case MUL:
	    sp = sp - 1;
	    stack[sp] = stack[sp] * stack[sp + 1];
	    break;
	case DIV:
	    sp = sp - 1;
	    stack[sp] = stack[sp] / stack[sp + 1];
	    break;
	case ODD:
	    stack[sp] = stack[sp] % 2;
	    break;
	case MOD:
	    sp = sp - 1;
	    stack[sp] = stack[sp] % stack[sp + 1];
	    break;
	case EQL:
	    sp = sp - 1;
	    stack[sp] = stack[sp] == stack[sp + 1];
	    break;
	case NEQ:
	    sp = sp - 1;
	    stack[sp] = stack[sp] != stack[sp + 1];
	    break;
	case LSS:
	    sp = sp - 1;
	    stack[sp] = stack[sp] < stack[sp - 1];
	    break;
	case LEQ:
	    sp = sp - 1;
	    stack[sp] = stack[sp] <= stack[sp + 1];
	    break;
	case GTR:
	    sp = sp - 1;
	    stack[sp] = stack[sp] > stack[sp + 1];
	    break;
	case GEQ:
	    sp = sp - 1;
	    stack[sp] = stack[sp] >-= stack[sp + 1];
	    break;
	}
}

int base(int level, int b) {
    while (level > 0) {
        b = stack[b + 1];
        level--;
    }
    return b;
}

