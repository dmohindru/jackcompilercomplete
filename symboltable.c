/* symboltable.c
 * This file implements Symbol Table Module of Jack Compiler
 * for Hack computer
 * Author: Dhruv Mohindru
 * Date: 14/06/2016
 */
#include "symboltable.h"
void constructorSymbolTable()
{
	//clear any existing symbol table if not clear earlier
	clearSymbolTable(0); //clear class symbol table
	clearSymbolTable(1); //clear method symbol table
	//set them to null
	classSmblTable = methodSmblTable = currentSmblTblEntry = 0;
	staticIndex = fieldIndex = argIndex = varIndex = 0;
}
void startSubroutine()
{
	clearSymbolTable(1); //clear method symbol table
	argIndex = varIndex = 0;
}
void define(char *name, char *type, kind kindType)
{
	//first create an entry node and set its data
	symbolTable *currentEntry;
	currentEntry = (symbolTable *)malloc(sizeof(symbolTable));
	strcpy(currentEntry->name, name);
	strcpy(currentEntry->type, type);
	currentEntry->kindType = kindType;
	switch(kindType)
	{
		case STATIC_SMBL:
			staticIndex++;
			currentEntry->indexSymbol = staticIndex;
			break;
		case FIELD_SMBL:
			fieldIndex++;
			currentEntry->indexSymbol = fieldIndex;
			break;
		case ARG_SMBL:
			argIndex++;
			currentEntry->indexSymbol = argIndex;
			break;
		case VAR_SMBL:
			varIndex++;
			currentEntry->indexSymbol = varIndex;
			break;
		default: //we may never possibly land in this code but just in case
			printf("Unknown symbol type found\n");
			exit(1);
	}
	currentEntry->next = 0;
	
	//check if its for class symbol table or for method symbol table
	if(kindType == STATIC_SMBL || kindType == FIELD_SMBL)
	{
		//its a class symbol table
		if(!classSmblTable)
		{
			classSmblTable = currentSmblTblEntry = currentEntry;
		}
		else
		{
			currentSmblTblEntry->next = currentEntry;
			currentSmblTblEntry = currentEntry;
		}
	}
	else if(kindType == ARG_SMBL || kindType == VAR_SMBL)
	{
		//its a method symbol table
		if(!methodSmblTable)
		{
			methodSmblTable = currentSmblTblEntry = currentEntry;
		}
		else
		{
			currentSmblTblEntry->next = currentEntry;
			currentSmblTblEntry = currentEntry;
		}
	}
}
int varCount(kind kindType)
{
	return 0;
}
kind kindOf(char *name)
{
	return STATIC_SMBL;
}
char* typeOf(char *name)
{
	return 0;
}
int indexOf(char *name)
{
	return 0;
}
void clearSymbolTable(int smblTable) //smblTable=0 for class symbol table, smblTable=1 for method symbol table
{
	symbolTable *entry, *next;
	if(smblTable == 0)
	{
		entry = classSmblTable->next;
		free(classSmblTable);
		classSmblTable = 0;
	}
	else
	{
		entry = methodSmblTable->next;
		free(methodSmblTable);
		methodSmblTable = 0;
	}
	while(entry!=0)
	{
		next = entry->next;
		free(entry);
		entry = next;
	}
	//clear the currentSmblTblEntry
	currentSmblTblEntry = 0;
}
void displaySymbolTable(int smblTable) //smblTable=0 for class symbol table, smblTable=1 for method symbol table
{
	symbolTable *entry;
	char kindSymbol[10];
	if(smblTable == 0)
	{
		entry = classSmblTable;
	}
	else
	{
		entry = methodSmblTable;
	}
	//print headers here
	printf("Name                Type                Kind      #  \n");
	while(entry!=0)
	{
		memset(kindSymbol, 0, 10);
		switch(entry->kindType)
		{
			case STATIC_SMBL:
				strcpy(kindSymbol, "static");
				break;
			case FIELD_SMBL:
				strcpy(kindSymbol, "field");
				break;
			case ARG_SMBL:
				strcpy(kindSymbol, "argument");
				break;
			case VAR_SMBL:
				strcpy(kindSymbol, "var");
				break;
			default:
				strcpy(kindSymbol, "unknown");
				break;
		}
		//print entry data
		printf("%-20s%-20s%-10s%3d\n", entry->name, entry->type, kindSymbol, entry->indexSymbol); 
		entry = entry->next;
	}
}
