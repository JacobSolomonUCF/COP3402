// TODO: Code up <program> to <rel-op>

int main(void)
{
    // main calls program
    // program calls block recursively 
    program();
    return 0;
}

void program()
{
    block();
    if (tok != periodsym) 
	error();
    advance();
}

void block()
{
    const_declaration();
    var_declaration();
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
    
    while (tok == commasym)
    {
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

void var_declaration()
{
    if (tok != varsym)
	return;
    advance();
    
    if (tok != identsym)
	error();
    advance();
    
    while (tok == commasym)
    {
	advance();
	if (tok != identsym)
	    error();
	advance();
    }
    
    if (tok != semicolonsym)
	error();
    advance();
}

void statement()
{
    if (tok == identsym)
    {
	advance();
	ident();
	
	if (tok != becomessym)
	    error();
	advance();
	// need to call advance() in expression
	expression();	    
    }
    else if (tok == beginsym)
    {
	advance();
	statement();
	
	while (tok == commasym)
	{
	    advance();
	    statement();
	}
	
	if (tok != endsym)
	    error();
	advance();
    }
    else if (tok == ifsym)
    {
	advance();
	condition();
	
	if (tok != thensym)
	    error();
	advance();
	statement();
    }
    else if (tok == whilesym)
    {
	advance();
	condition();
	
	if (tok != dosym)
	    error();
	advance();
	statement();
    }
    else if (tok == readsym)
    {
	advance();
	if (tok != identsym)
	    error();
	indent();
    }
    else if (tok == writesym)
    {
	advance();
	if (tok != identsym)
	    error();
	ident();
    }
}

void condition()
{
    if (tok == oddsym)
    {
	advance();
	expression();
    }
    else
    {
	expression();
	rel_op();
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

// expression ::= [ "+"| "-" ] term { ("+" | "-") term } .
void expression() {

    if(tok != plussym || tok != minussym) return;
    advance();
    term(); // ?
    while(tok == plussym || tok == minussym) {
        advance();
        if(tok != identsym) error();
        advance();
    }
    term(); // ?
    advance();
}

// term ::= factor { ( "*" | "/" ) factor } .
void term() {
    factor();
    while(tok == multsym || tok == slashsym) {
        factor();
        advance();
    }
}

// factor ::= ident | number | "(" expression ")" .
void factor() {
    if(tok == identsym)
        advance();
    if(tok == numsym)
        advance();
    if(tok == lparensym) {
        advance();
        expression();
        advance();
        if(tok != rparensym)
            error();
    }

    advance();

}

// number ::= digit {digit}.
void number() {

    digit();
    while(false) {
        digit();
    }

}

//ident ::= letter {letter | digit}.
void ident() {
    letter();

}

//digit ::= “0” | “1” | “2” | “3” | “4” | “5” | “6” | “7” | “8” | “9” .
void digit() {

   if(isdigit(tok))
        advance();

}

//letter ::= “a” | “b” | ... | “y” | “z” | “A” | “B” | ... | “Y” | “Z”.
void letter() {

    if(isalpha(tok))
        advance();

}

