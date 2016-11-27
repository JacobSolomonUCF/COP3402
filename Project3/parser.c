// PL/0 COMPILER
// Matthew Carlino
// Jacob Solomon
// Joseph Freeman
// Alex Arwin
// Ryan Rossbach

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "parser.h"
#define MAX_LEXI_LEVELS 3
#define MAX_SYMBOL_TABLE_SIZE 100
#define CODE_SIZE 1000

//Structure for the symbol.
typedef struct {
    int kind; 		// Type const,var or proc
    char name[12];	// Name
    int val; 		// Num
    int level; 		// Level
    int addr; 		// Address
} symbol;

typedef struct {
    int op; //Op
    int l;  //L
    int m;  //M
} instruction;

typedef struct{
    int tokenID;
    int numVal;
    char name[12];
}sToken;

sToken singToken; //Hold the current token.
symbol table[MAX_SYMBOL_TABLE_SIZE];
instruction code[CODE_SIZE];

FILE *ifp; // Input FIle


// Function Prototype(s)
void program();
void block(int lev, int tx);
void constdec(int lev, int *ptx, int *pdx);
void vardec(int lev, int *ptx, int *pdx);
void statement(int lev, int *ptx);
void condition(int lev, int *ptx);
void expression(int lev, int *ptx);
void term(int lev, int *ptx);
void factor(int lev, int *ptx);
void emit(int op, int l, int m);
void enterToSymbolTable(int k, int *ptx, int *pdx, int lev);
void error(int error);
void advance();
int position(char *id, int *ptx, int lev);



//Globals Defined
int cx, diff, kind;
int lexemeListIndex=0;
int pastdiff=0;

void parse() {

    FILE* ofp;
    
    ifp = fopen("lexoutput", "r");
    ofp = fopen("parserout", "w");
    
    if(ifp == NULL){
        printf("ERROR in opening the file");
    }
    program();

    for (int i=0; i<cx;i++) { //Ouputs to file
        fprintf(ofp, "%d %d %d\n", code[i].op, code[i].l, code[i].m);
    }
    fclose(ifp);
    fclose(ofp);
}
void advance() {
    
    fscanf(ifp, "%d", &singToken.tokenID);      //Reads in next token from input file
    
    if(singToken.tokenID==2)                    //If token is ident, saves the name
        fscanf(ifp, "%s", singToken.name);
    else if(singToken.tokenID==3)
        fscanf(ifp, "%d", &singToken.numVal);//If token is num, saves the num
    
}

int position(char *id, int *ptx, int lev) {
    int i=*ptx;
    int diffcount = 0;
    int curr = 0;
    
    while(i!=0) {
        if (strcmp(table[i].name,id) == 0) {
            if(table[i].level<=lev) {
                if (diffcount!=0)
                    pastdiff = diff;
                
                diff = lev-table[i].level;
                
                if(diffcount==0)
                    curr=i;
                if (diff<pastdiff)
                    curr=i;
                diffcount++;
            }
        }
        i--;
    }
    return curr;
}

void program() {
    advance();
    block(0,0);
    
    if (singToken.tokenID!=periodsym)
        error(9);
}

void block(int lev, int tx) {
    
    if(lev > MAX_LEXI_LEVELS) //Checking for max level
        error(26);
    int dx, tx0, cxNew;
    
    dx=4;
    tx0=tx;
    table[tx].addr=cx;
    emit(7,0,0); //JMP
    
    do {
        if (singToken.tokenID==constsym) {
            advance();
            do {
                constdec(lev,&tx,&dx);
                while(singToken.tokenID==commasym) {
                    advance();
                    constdec(lev,&tx,&dx);
                }
                if(singToken.tokenID==semicolonsym)
                    advance();
                else
                    error(5);
            } while (singToken.tokenID==identsym);
        }
        if (singToken.tokenID==varsym) {
            advance();
            do {
                vardec(lev,&tx,&dx);
                while (singToken.tokenID==commasym) {
                    advance();
                    vardec(lev,&tx,&dx);
                }
                if(singToken.tokenID==semicolonsym)
                    advance();
                else
                    error(5);
                
            } while(singToken.tokenID==identsym);
        }
        while(singToken.tokenID==procsym) {
            advance();
            if(singToken.tokenID==identsym) {
                enterToSymbolTable(3, &tx, &dx, lev);
                advance();
            }
            else {
                error(4);
            }
            if(singToken.tokenID==semicolonsym)
                advance();
            else
                error(5);
            
            
            block(lev+1, tx); //Go to the block one level higher
            if(singToken.tokenID==semicolonsym)
                advance();
            else
                error(5);
        }
    }while((singToken.tokenID==varsym) || (singToken.tokenID==procsym)||(singToken.tokenID==constsym));
    code[table[tx0].addr].m=cx; //JMP ADDR
    table[tx0].addr=cx;
    cxNew=cx;
    
    emit(6, 0, dx); //INC
    statement(lev, &tx);
    emit(2, 0, 0); //OPR
}

void constdec(int lev, int *ptx, int *pdx) {
    if (singToken.tokenID==identsym) {
        advance();
        if ((singToken.tokenID==eqsym) || (singToken.tokenID==becomessym)) {
            if (singToken.tokenID==becomessym)
                error(1);
            advance();
            if (singToken.tokenID==numbersym) {
                enterToSymbolTable(1,ptx,pdx,lev); //For CONST
                advance();
            }
        }
    }
}

void vardec(int lev, int *ptx, int *pdx) {
    
    if (singToken.tokenID==identsym) {
        enterToSymbolTable(2,ptx,pdx,lev);//For VAR
        advance();
    }
    else
        error(4);
}

void statement(int level, int *ptx) {
    
    int temp, cx1, cx2;
    
    if (singToken.tokenID==identsym){
        temp = position(singToken.name,ptx, level);
        if(temp == 0)
            error(11);
        else if (table[temp ].kind!=2) {
            error(12);
            temp = 0;
        }
        
        advance();
        if (singToken.tokenID ==becomessym)
            advance();
        else
            error(13);
        
        expression(level, ptx);
        if (temp !=0)
            emit(4, level-table[temp ].level, table[temp ].addr); //STO
    }
    else if (singToken.tokenID==callsym) {
        advance();
        if (singToken.tokenID!=identsym) {
            error(14);
        }
        else {
            temp =position(singToken.name, ptx, level);
            if(temp ==0)
                error(11);
            else if (table[temp ].kind==3)
                emit(5,level-table[temp ].level, table[temp ].addr); //CAL
            else
                error(15);
            
            advance();
        }
    }
    else if (singToken.tokenID == ifsym) {
        advance();
        condition(level, ptx);
        if(singToken.tokenID == thensym)
            advance();
        else
            error(16);  // then expected
        
        cx1 = cx;
        emit(8, 0, 0); //JPC
        
        statement(level, ptx);
        
        if(singToken.tokenID == elsesym) {
            advance();
            
            code[cx1].m = cx+1; //Jumps pas the if
            cx1 = cx;
            emit(7, 0, 0); // JMP
            statement(level, ptx);
        }
        code[cx1].m = cx; //jumps past else or there isn't one jump past if
    }
    
    else if (singToken.tokenID == beginsym) {
        advance();
        statement(level, ptx);
        
        while (singToken.tokenID== semicolonsym) {
            advance();
            statement(level, ptx);
        }
        
        if (singToken.tokenID == endsym)
            advance();
        else
            error(17);

    }
    else if (singToken.tokenID == whilesym) {
        cx1 =cx;
        advance();
        condition(level,ptx);
        cx2 = cx;
        emit(8, 0, 0); //JPC
        
        if(singToken.tokenID== dosym)
            advance();
        else
            error(18);
        
        statement(level, ptx);
        emit(7, 0, cx1); //JMP
        code[cx2].m = cx;
    }
    
    //write needs to write
    else if (singToken.tokenID == writesym) {
        advance();
        expression(level, ptx);
        emit(9,0,1); //SIO#1
    }
    
    //read needs to read and STO
    else if (singToken.tokenID == readsym) {
        advance();
        emit(10,0,2); //SIO#2
        temp = position(singToken.name,ptx, level);
        if(temp ==0)
            error(11);
        else if (table[temp ].kind!=2) { //var
            error(12);
            temp =0;
        }
        
        if (temp !=0) {
            emit(4, level-table[temp ].level, table[temp ].addr); //STO
        }
        
        advance();
    }
    
}

void condition(int lev, int *ptx) {
    
    int rSwitch;
    
    if (singToken.tokenID == oddsym) {
        advance();
        expression(lev,ptx);
        emit(2, 0, 6); //OPR
        
    }
    else {
        expression(lev,ptx);
        if ((singToken.tokenID!=leqsym) && (singToken.tokenID!=gtrsym) && (singToken.tokenID!=geqsym)&&(singToken.tokenID!=eqsym) && (singToken.tokenID!=neqsym) && (singToken.tokenID!=lessym)) {
            error(20);
        }
        else {  //Operators
            rSwitch = singToken.tokenID;
            advance();
            expression(lev, ptx);
            switch(rSwitch) {
                case 9:
                    emit(2,0,8); //EQ
                    break;
                case 10:
                    emit(2,0,9); //NEQ
                    break;
                case 11:
                    emit(2,0,10); //LSS
                    break;
                case 12:
                    emit(2,0,11); //LEQ
                    break;
                case 13:
                    emit(2,0,12); //GTR
                    break;
                case 14:
                    emit(2,0,13); //GEQ
                    break;
                default:
                    break;
            }
        }
    }
}

void expression(int lev, int *ptx) {
    
    int add;
    if (singToken.tokenID== plussym || singToken.tokenID == minussym) {
        
        add = singToken.tokenID;
        advance();
        term(lev, ptx);
        if(add == minussym) {
            emit(2, 0, 1); //NEG
        }
    }
    else
        term (lev, ptx);
    
    while (singToken.tokenID == plussym || singToken.tokenID == minussym) {
        add = singToken.tokenID;
        advance();
        term(lev, ptx);
        if (add == plussym)
            emit(2, 0, 2); //ADD
        else
            emit(2, 0, 3); //SUB
    }
}

void term(int lev, int *ptx) {
    
    int op;
    factor(lev, ptx);
    while(singToken.tokenID == multsym || singToken.tokenID == slashsym) {
        op = singToken.tokenID;
        advance();
        factor(lev, ptx);
        if(op == multsym)
            emit(2, 0, 4); //MUL
        else
            emit(2, 0, 5); //DIV
    }
}

void factor(int lev, int *ptx) {
    
    int i, level, adr, val;
    
    while ((singToken.tokenID==identsym)||(singToken.tokenID==numbersym)||(singToken.tokenID==lparentsym)){
        if (singToken.tokenID==identsym) {
            i=position(singToken.name,ptx, lev);
            if (i==0) {
                error(11);
            }
            else {
                kind=table[i].kind;
                level=table[i].level;
                adr=table[i].addr;
                val=table[i].val;
                if (kind==1)                    //For CONST
                    emit(1,0,val);              //LIT
                else if (kind==2)               //For VAR
                    emit(3,lev-level,adr);      //LOD
                else
                    error(21);
            }
            advance();
        }
        else if(singToken.tokenID==numbersym) {
            if (singToken.numVal>2047) { //Max ADDR
                error(25);
                singToken.numVal=0;
            }
            emit(1,0,singToken.numVal); //LIT
            advance();
        }
        else if(singToken.tokenID==lparentsym) {
            advance();
            expression(lev,ptx);
            if (singToken.tokenID==rparentsym)
                advance();
            else
                error(22);
        }
    }
}

void emit(int op, int l, int m) {
    
    if (cx > CODE_SIZE)
        exit(0);
    else {
        code[cx].op = op; 	//opcode
        code[cx].l = l;	// lexicographical level
        code[cx].m = m;	// modifier
        cx++;
    }
}

//This enters a symbol into the table
void enterToSymbolTable(int k, int *ptx, int *pdx, int lev) {
    int len;
    (*ptx)++;                       //Table index++
    char *name = singToken.name;    //Last Ident
    len = strlen(singToken.name);
    for (int i=0;i<=len;i++) {
        table[*ptx].name[i]=*name;
        name++;
    }
    
    table[*ptx].kind=k;
    
    if (k==1) {                     //For Const
        table[*ptx].val=singToken.numVal;
    }
 
    else if (k==2) {                //For Var
        table[*ptx].level=lev;
        table[*ptx].addr=*pdx;
        (*pdx)++;
    }

    else {                          //For Proc
        table[*ptx].level=lev;
    }
}

void error(int error)
{
    
    // list of errors:
    
    switch(error)
    {
        case 1: // use = instead of :=
            printf("Error number 1, use = instead of :=\n");
            break;
        case 2: // = must be followed by a number
            printf("Error number 2, = must be followed by a number\n");
            break;
        case 3: // Identifier must be followed by =
            printf("Error number 3, identifier must be followed by =\n");
            break;
        case 4: // 'const', 'var', 'procedure' must be followed by identifier
            printf("Error number 4, 'const', 'var', 'procedure' must be followed by identifier\n");
            break;
        case 5: // semicolon or comma missing
            printf("Error number 5, semicolon or comma missing\n");
            break;
        case 6: // incorrect symbol after procedure declaration
            printf("Error number 6, incorrect symbol after procedure declaration\n");
            break;
        case 7: // statement expected
            printf("Error number 7, statement expected\n");
            break;
        case 8: // incorrect symbol after statement part in block
            printf("Error number 8, incorrect symbol after statement part in block\n");
            break;
        case 9: // period is expected
            printf("Error number 9, period is expected\n");
            break;
        case 10: // semicolon between statements missing
            printf("Error number 10, semicolon between statements missing\n");
            break;
        case 11: // undeclared identifier
            printf("Error number 11, undeclared identifier\n");
            break;
        case 12: // assignment to constant or procedure is not allowed
            printf("Error number 12, assignment to constant or procedure is not allowed\n");
            break;
        case 13: // assignment operator expected
            printf("Error number 13, assignment operator expected\n");
            break;
        case 14: // 'call' must be followed by an identifier
            printf("Error number 14, 'call' must be followed by an identifier\n");
            break;
        case 15: // call of a constant or variable is meaningless
            printf("Error number 15, call of a constant or variable is meaningless\n");
            break;
        case 16: // 'then' expected
            printf("Error number 16, 'then' expected\n");
            break;
        case 17: // semicolon or '}' expected
            printf("Error number 17, semicolon or '}' expected\n");
            break;
        case 18: // 'do' expected
            printf("Error number 18, 'do' expected\n");
            break;
        case 19: // incorrect symbol following statement
            printf("Error number 19, incorrect symbol following statement\n");
            break;
        case 20: // relational operator expected
            printf("Error number 20, relational operator expected\n");
            break;
        case 21: // expression must not contain a procedure identifier
            printf("Error number 21, expression must not contain a procedure identifier\n");
            break;
        case 22: // right parenthesis missing
            printf("Error number 22, right parenthesis missing\n");
            break;
        case 23: // the preceding factor cannot begin with this symbol
            printf("Error number 23, the preceding factor cannot begin with this symbol\n");
            break;
        case 24: // an expression cannot begin with this symbol
            printf("Error number 24, an expression cannot begin with this symbol\n");
            break;
        case 25: // this number is too large
            printf("Error number 25, this number is too large\n");
            break;
        default:
            printf("There has been an error\n");
    }
    
    exit(0);
    
}

