// TODO: Code up <program> to <rel-op>

int main(void)
{
    // main calls program
    // program calls block recursively
    program();
    return 0;
}

// Appendix D
void program()
{
    advance();
    block();
    if (tok != periodsym)
        error();
    // advance();
    // printf("Tiny PL/0 program is grammatically correct\n");
}

// Appendix D
void block()
{

    if(tok == constsym) {
        do {
            advance();
            if(tok != identsym)     // Error: expected identifer in constant declaration
                error();
            advance();
            if(tok != eqsym)
                error();            // Expected '=' after 
            advance();
            if(tok !+ numbersym)
                error();
            advance();
        } while(tok == commasym)

        if(tok != semicolonsym)
            error();
        advance();
    }

    if(tok == varsym) {
        do {
          advance();

          if(tok != identsym)
                error();            // Expected identifer in variable declaration
        } while(tok == commasym)

        if(tok != semicolonsym)
            error();                // Expected ';' at the end of variable declaration
        advance();
    }

    while(tok == procsym) {
        advance();

        if(tok != identsym)
            error();
        advance();

        if(tok != semicolonsym)
            error();
        advance();
        block();

        if(tok != semicolonsym)
            error();
        advance();
    }

    statement();

}

void const_declaration()
{
    if (tok != constsym)
        return;
    advance();

    if (tok != identsym)
        error();
    advance();
    ident();

    if (tok != equalsym) // "="
        error();
    advance();

    if (tok != numbersym)
        error();
    advance();

    while (tok == commasym) {
        advance();

        if (tok != identsym)
            error();
        advance();
        ident();

        if (tok != equalsym) // "="
            error();
        advance();

        if (tok != numbersym)
            error();
        advance();
        number();
    }

    if (tok != semicolonsym)
        error();
    advance();
}

void var_declaration() {

    if (tok != varsym)
        return;
    advance();

    if (tok != identsym)
	error();
    advance();

    while (tok == commasym) {
        advance();
        if (tok != identsym)
            error();
        advance();
    }

    if (tok != semicolonsym)
        error();
    advance();
}

// Appendix D
void statement()
{
    if (tok == identsym) {
        advance();
        if (tok != becomessym)
            error();
        advance();
        // need to call advance() in expression
        expression();
    }

    else if(tok == callsym) {
        advance();
        if(tok != identsym)
            error();
        advance();
    }

    else if (tok == beginsym) {
        advance();
        statement();

        while (tok == semicolonsym) {
            advance();
            statement();
        }

        if(tok != endsym)
            error();
        advance();
    }

    else if (tok == ifsym) {
        advance();
        condition();

        if (tok != thensym)
            error();
        advance();
        statement();
    }

    else if (tok == whilesym) {
        advance();
        condition();

        if (tok != dosym)
            error();
        advance();
        statement();
    }

    else if (tok == readsym) {
        advance();
        if (tok != identsym)
            error();
        indent();
    }

    else if (tok == writesym) {
        advance();
        if (tok != identsym)
            error();
        ident();
    }
}

// Appendix D
void condition()
{
    if (tok == oddsym) {
        advance();
        expression();
    }

    else {
        expression();
        if(tok != relation())
            error();
        advance();
        expression();
    }
}

void rel_op()
{
    switch(tok)
	case equalsym:
	    advance();
	    return;
	case neqsym:
	    advance();
	    return;
	case lesssym:
	    advance();
	    return;
	case leqsym:
	    advance();
	    return;
	case gtrsym:
	    advance();
	    return;
	case geqsym:
	    advance();
	    return;
	default:
	    error();
}

// Still have a few functions I need to finish / double check
// Think the majority of them are pretty set though

// Appendix D .
void expression()
{

    if(tok == plussym || tok == minussym)
        advance();
    term();
    while(tok == plussym || tok == minussym) {
        advance();
        term();
    }
}

// Appendix D
void term()
{
    factor();

    while(tok == multsym || tok == slashsym) {
        advance();
        factor();
    }
}

// factor ::= ident | number | "(" expression ")" .
void factor()
{

    if(tok == identsym)
        advance();

    else if(tok == number())
        advance();

    else if(tok == lparensym) {
        advance();
        expression();
        if(tok != rparensym)
            error();
    }

}

// number ::= digit {digit}.
void number()
{

    digit();
    while(digit()) {
        advance();
    }

}

//ident ::= letter {letter | digit}.
void ident()
{
    letter();

}

//digit ::= “0” | “1” | “2” | “3” | “4” | “5” | “6” | “7” | “8” | “9” .
void digit()
{

   if(isdigit(tok))
        advance();

}

//letter ::= “a” | “b” | ... | “y” | “z” | “A” | “B” | ... | “Y” | “Z”.
void letter()
{

    if(isalpha(tok))
        advance();

}

void error() {
    printf("There has been an error\n");
    exit(0);

}

