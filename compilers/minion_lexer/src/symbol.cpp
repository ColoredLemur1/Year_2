#include "symbol.h"
#include "string.h"
#include "stdio.h"

Symbol St[100];  // symbol table
int ns; // number of symbols in the table
int addreesCounter;

void InsertSymbol(char* name, DataTypes type)
{
	strcpy (St[ns].name , name);
	St[ns].type = type;
	St[ns].address = addreesCounter++;
	ns++;
}

int FindSymbol(char* name)
{
	for (int i = 0; i < ns; i++)
	{
		if (!strcmp(St[i].name, name))
		{
			return i; //Return idnex
		}

	}
	return -1; //Index was not found
}

void InitSymbolTable()
{
	ns = 0;
	addreesCounter = 0;
}

const char* DataTypeToString(DataTypes t)
{
	if (t == banana)
		return "banana";
	else if (t == bapple)
		return "bapple";
	else if (t == gelato)
		return "gelato";
	else return "unknown data type";
}
void PrintAllSymbols()
{
	printf("\n\nThe symbols of this program:\n");
	for (int i = 0; i < ns; i++)
		printf("%s: %s, %i\n", St[i].name, DataTypeToString(St[i].type), St[i].address);
}

int GetAddress(int r) // return the memory adress of the symbol given by its index r withing the symbol table
{
	return St[r].address;
}