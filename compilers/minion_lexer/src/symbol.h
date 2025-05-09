#pragma once


typedef enum {banana , gelato, bapple} DataTypes;

typedef struct {
	char name[128];
	DataTypes type;
	int address;
} Symbol;


void InsertSymbol(char* name, DataTypes type);
int FindSymbol(char* name); // returns the index of the symbol if found, otherwise returns -1 !!!
void InitSymbolTable();
void PrintAllSymbols();
int GetAddress(int);