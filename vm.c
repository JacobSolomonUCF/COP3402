// Compiler Builder 1:
// Matthew Carlino
// Jacob Solomon
// Joseph Freeman
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

int lines; // made lines a global for easier access

//Function Prototype(s)
void readInput(FILE * input);
void fetchCycle();
void executeCycle();
void operation();
int base(int level, int b);
void printStack();

int main()
{
    stack[1] = 0;
    stack[2] = 0;
    stack[3] = 0;
    ir.op = 0;
    ir.l = 0;
    ir.m = 0;
    
    
    FILE * input = fopen("mcode.pl0", "r");
    if(input == NULL){ //Checks for no file
        printf("Error in opening the file");
        exit(0);
    }
    readInput(input); //Reads input form file, Converts it and outputs to screen
    //fetchCycle();
    
    int i;
    for(i = 0; i < lines - 1; i++) {
    	fetchCycle();
    	executeCycle();
    	printStack();
    }
    
    return 0;
}

void readInput(FILE * input){
    
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
    printf("\nExecution:\n\t\t\t\t\tpc  bp  sp   stack\n\t\t\t\t     0   1   0\n");
}

void fetchCycle(){
    ir = instructions[pc];
    pc = pc + 1;
}

void executeCycle() {
    switch (ir.op) {
	case 1: // LIT
	    sp = sp + 1;
	    stack[sp] = ir.m;
	    break;
	case 2: // OPR
	    operation();
	    break;
	case 3: // LOD
	    sp = sp + 1;
	    stack[sp] = stack[base(ir.l, bp) + ir.m];
	    break;
	case 4: // STO
	    stack[base(ir.l, bp) + ir.m] = stack[sp];
	    sp = sp - 1;
	    break;
	case 5: // CAL
	    stack[sp + 1] = 0;
	    stack[sp + 2] = base(ir.l, bp);
	    stack[sp + 3] = bp;
	    stack[sp + 4] = pc;
	    bp = sp + 1;
	    pc = ir.m;
	    break;
	case 6: // INC
	    sp = sp + ir.m;
	    break;
	case 7: // JMP
	    pc = sp + ir.m;
	    break;
	case 8: // JPC
	    if (stack[sp] == 0)
	        pc = ir.m;
	    sp = sp - 1;
	    break;
	case 9: // SIO
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

void operation() {
    switch(ir.m) {
	case 0: // RET
	    sp = bp - 1;
	    pc = stack[sp + 4];
	    bp = stack[sp + 3];
	    break;
	case 1: // NEG
	    stack[sp] = -stack[sp];
	    break;
	case 2: // ADD
	    sp = sp - 1;
	    stack[sp] = stack[sp] + stack[sp + 1];
	    break;
	case 3: // SUB
	    sp = sp - 1;
	    stack[sp] = stack[sp] - stack[sp + 1];
	    break;
	case 4: // MUL
	    sp = sp - 1;
	    stack[sp] = stack[sp] * stack[sp + 1];
	    break;
	case 5: // DIV
	    sp = sp - 1;
	    stack[sp] = stack[sp] / stack[sp + 1];
	    break;
	case 6: //ODD
	    stack[sp] = stack[sp] % 2;
	    break;
	case 7: // MOD
	    sp = sp - 1;
	    stack[sp] = stack[sp] % stack[sp + 1];
	    break;
	case 8: // EQL
	    sp = sp - 1;
	    stack[sp] = stack[sp] == stack[sp + 1];
	    break;
	case 9: // NEQ
	    sp = sp - 1;
	    stack[sp] = stack[sp] != stack[sp + 1];
	    break;
	case 10: // LSS
	    sp = sp - 1;
	    stack[sp] = stack[sp] < stack[sp - 1];
	    break;
	case 11: // LEQ
	    sp = sp - 1;
	    stack[sp] = stack[sp] <= stack[sp + 1];
	    break;
	case 12: // GTR
	    sp = sp - 1;
	    stack[sp] = stack[sp] > stack[sp + 1];
	    break;
	case 13: // GEQ
	    sp = sp - 1;
	    stack[sp] = stack[sp] >= stack[sp + 1];
	    break;
	}
}

// This is just a huge mess at the moment. It should be cleaned up soon.
// Just need to add the operation being executed, line #, l, m
void printStack() {

    printf("\t\t\t\t    %2d  %2d  %2d   ", pc, bp, sp);

    int i;
    for(i = 1; i <= sp; i++){
    	if(i == 7 && sp > 7)		// not sure of exact criteria for separation of stack?
    	    printf("| ");
        printf("%d ", stack[i]);
    }
    printf("\n");

	
}


int base(int level, int b) {
    while (level > 0) {
        b = stack[b + 1];
        level--;
    }
    return b;
}

