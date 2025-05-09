#include "lexer.h"
#include "parser.h"
#include "symbol.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>




void stmt();
void declar();
DataTypes type();
void assign();
void expr();
void term();
void factor();
void print();
void loop();
void error(const char*, Token);


void error(const char* msg, Token t)
{
	printf("Line %i at or near %s: %s\n", t.ln, t.x, msg);
	exit(0);
}

void Emit(const char* code)
{
	printf("%s ", code);
}



void prog()
{
	Token t = GetToken();
	if (t.t == key && !strcmp(t.x, "bello"))
		;
	else
		error("bello expected",t);
	t = PeekToken();
	while (t.t != eof && strcmp (t.x , "bank" ))
	{
		stmt();
		t = PeekToken();
	}
	t = GetToken();
	if (t.t == key && !strcmp(t.x, "bank"))
		;
	else
		error("bank expected",t);
	t = GetToken();
	if (t.t == key && !strcmp(t.x, "yu"))
		;
	else
		error("yu expected",t);
	t = GetToken();
	if (t.t == eof )
		;
	else
		error("eof expected",t);
	printf("TAMAM\n");
}

void stmt()
{
	Token t = PeekToken();
	if (!strcmp(t.x, "minion"))
		declar();
	else if (t.t == id)
		assign();
	else if (!strcmp(t.x, "para"))
		print();
	else if (!strcmp(t.x, "bee"))
		loop();
	else
		error("a keyword or id is expected",t);
}

void declar()
{
	DataTypes given_type;
	Token t = GetToken();
	char lexeme[128];

	if (t.t == key && !strcmp(t.x, "minion"))
		;
	else
		error("minion expected",t);
	t = GetToken();
	if (t.t == id)
		strcpy(lexeme, t.x);
	else
		error("id expected",t);
	t = GetToken();
	if (!strcmp (t.x , ":") )
		;
	else
		error(": expected",t);
	given_type = type();
	if (FindSymbol(lexeme) != -1)
	{ //Error
		error("symbol already exists near token, you fool!", t);
	}
	InsertSymbol(lexeme, given_type);
}

DataTypes type()
{
	Token t = GetToken();
	if (!strcmp(t.x, "banana"))
	{
		return banana;
	}
	else if (!strcmp(t.x, "gelato"))
	{
		return gelato;
	}
	else if (!strcmp(t.x, "bapple"))
	{
		return bapple;
	}
	else
		error("not a good type",t);
}

void assign()
{
	Token t = GetToken();
	int r;
	if (t.t == id)
	{
		r = FindSymbol(t.x);
		if (r == -1)
			error("Undeclared variable", t);
		// r contains the address of this variable in the static segments
	}
	else
		error("id expected",t);
	t = GetToken();
	if (!strcmp (t.x , "@"))
		;
	else
		error("@ expected",t);
	expr();  // call the parser for the expr on the RHS
	int address = GetAddress(r);
	char temp[128];
	sprintf(temp,"%i\n",address); // convert address to array of char, i.e. string
	Emit("pop static ");
	Emit(temp);
}

void expr()
{
	int sign;
	term();
	Token t = PeekToken();
	while (!strcmp(t.x,"+") || !strcmp(t.x, "-"))
	{ 
		// consume the operator
		t = GetToken(); // 
		term();
		if (!strcmp(t.x, "+"))
			Emit("add\n");
		else
			Emit("sub\n");
		// i can also generate the code here
		t = PeekToken();
		// at this point both term parsers have put their code into the outpur file	
	}
}

void term()
{
	factor();
	Token t = PeekToken();
	while (!strcmp(t.x, "*") )
	{
		// consume the operator
		t = GetToken();
		factor();
		t = PeekToken();
		Emit("call Math.multiply 2\n");
	}
}

void factor()
{
	int r;
	Token t = GetToken();
	if (t.t == id)
	{
		if ((r = FindSymbol(t.x)) == -1)
		{ //If symbol doesn't exist
			error("symbol does not exist, how could you?", t);
		}
		int address = GetAddress(r);
		Emit("push static ");
		char temp[128];
		sprintf(temp, "%i\n", address);
		Emit(temp);
	}
	else if (t.t == num)
	{
		;
	}
	else
		error("id or num expected",t);

}

void print()
{
	Token t = GetToken();
	if (!strcmp(t.x, "para"))
		;
	else
		error("para expected",t);

	t = GetToken();
	if (!strcmp(t.x, "tu"))
		;
	else
		error("tu expected",t);

	expr();
}

void loop()
{
	Token t = GetToken();
	if (!strcmp(t.x, "bee"))
		;
	else
		error("bee expected",t);

	t = GetToken();
	if (!strcmp(t.x, "do"))
		;
	else
		error("do expected",t);
	expr();
	t = PeekToken();
	while (t.t != eof && strcmp(t.x, "stopa"))
	{
		stmt();
		t = PeekToken();
	}
	t = GetToken();
	if (!strcmp(t.x, "stopa"))
		;
	else
		error("stopa expected",t);

}