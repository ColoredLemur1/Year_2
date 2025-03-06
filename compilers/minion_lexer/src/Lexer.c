#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "lexer.h"

const char* keywords[NumberKeywords] = { "bello" , "minion" , "banana", "gelato" , "bapple" , "bee" , "do" , "stopa" , "para" , "tu" , "bank", "yu"};
FILE* f;
int TokenReady; 
Token t;
int LineCount;

int Init() // init the lexer
{
	f = fopen("bob.txt", "r");
	if (f == 0)
		return 0;
	TokenReady = 0;
	LineCount = 1;
	return 1;
}

int IsKeyWord(char* str)
{
	for (int i = 0; i < NumberKeywords; i++)
		if (!strcmp(keywords[i], str))
			return 1;
	return 0;
}

int EatWC()
{
	int c;
	c = getc(f);
	while (c != EOF && isspace(c))
	{
		if ((char) c == '\n') LineCount++;
		c = getc(f);
	}
	
	if (c == '!')
	{
		c = getc(f);
		if ((char)c == '\n') LineCount++;
		while (c != EOF && c != '\n')
		{
			c = getc(f);
			if ((char)c == '\n') LineCount++;
		}
		if (c == EOF)
			return c;
		c = getc(f);
		if (isspace(c) || c == '!')
		{
			ungetc(c, f);
			return EatWC();
		}
	}
	return c;
}

Token BuildToken()
{
	t.t = sym;
	int c = EatWC();

	if (c == EOF)
	{
		t.t = eof;
		t.ln = LineCount;
		return t;
	}
	char temp[128];
	int i = 0;
	if (isalpha(c))
	{
		while (c != EOF && isalpha(c))
		{
			temp[i++] = c;
			c = getc(f);
		}
		temp[i] = '\0';
		ungetc(c, f);

		strcpy(t.x, temp);
		if (IsKeyWord(temp))
			t.t = key;
		else
			t.t = id;
		t.ln = LineCount;
		return t;
	}
	else if (isdigit(c))
	{
		while (c != EOF && isdigit(c))
		{
			temp[i++] = c;
			c = getc(f);
		}
		if (isalpha(c) && (c == 'b' || c == 'p'))
		{
			temp[i++] = c;
			temp[i] = '\0';
		}
		else
		{
			temp[i] = '\0';
			ungetc(c, f);
		}
		strcpy(t.x, temp);
		t.t = num;
		t.ln = LineCount;
		return t;

	}
	else
	{
		temp[0] = c;
		temp[1] = '\0';
		strcpy(t.x, temp);
		t.ln = LineCount;
		return t;
	}
}

Token GetToken()
{
	if (TokenReady)
	{
		TokenReady = 0;
		return t;
	}
	t = BuildToken();
	TokenReady = 0;
	return t;
}

Token PeekToken()
{
	if (TokenReady)
		return t;
	t = BuildToken();
	TokenReady = 1;
	return t;
}