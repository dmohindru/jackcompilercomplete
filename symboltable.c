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
	symbolTable *entry;
	int count = 0;
	if(kindType == STATIC_SMBL || kindType == FIELD_SMBL) //class symbol table kind Types
	{
		if(!classSmblTable) //if class symbol table is empty
		{
			return 0;
		}
		entry = classSmblTable;
	}
	else if(kindType == VAR_SMBL || kindType == ARG_SMBL) //class symbol table kind Types
	{
		if(!methodSmblTable) //if method symbol table is empty
		{
			return 0;
		}
		entry = methodSmblTable;
	}
	else //we won't land into this code by just in case
	{
		return 0;
	}
	while(entry!=0)
	{
		if(entry->kindType == kindType)
		{
			count++;
		}
		entry = entry->next;
	}
	return count;
}
kind kindOf(char *name)
{
	/* Basic idea will be using here is that first we look
	 * into method symbol table and find a named identifer type
	 * if it is not found there or method symbol table is not 
	 * defined. Then we look into class symbol table, if named
	 * identifier is not found in class symbol table we return 
	 * NONE
	 * Same concept will be used for function kindOf, typeOf, indexOf
	 * */
	symbolTable *entry;
	//variable to track if we are scanning method or class symbol table
	// 0 for method symbol table and 1 for class symbol table
	int methodOrClassTable = 0; 
	if(methodSmblTable)
	{
		entry = methodSmblTable;
		methodOrClassTable = 0; //we are scanning method symbol table
	}
	else if(classSmblTable)
	{
		entry = classSmblTable;
		methodOrClassTable = 1; //we are scanning class symbol table
	}
	else //no method or class symbol table found return NONE
	{
		return NONE;
	}
	while(entry!=0)
	{
		if(!strcmp(entry->name, name))
		{
			return entry->kindType;
		}
		entry = entry->next;
	}
	//we have reached here may be named identifer is not in method symbol table
	//or class symbol table, or class symbol table is to be scanned
	if(methodOrClassTable)
	{
		//we have already scanned class symbol table nothing found
		//return NONE
		return NONE;
	}
	else
	{
		//check class symbol table for occurance of named identifier
		if(!classSmblTable)
		{
			//class symbol table is empty return NONE
			return NONE;
		}
		entry = classSmblTable;
	}
	while(entry!=0)
	{
		if(!strcmp(entry->name, name))
		{
			return entry->kindType;
		}
		entry = entry->next;
	}
	return NONE; //nothing found
}
char* typeOf(char *name)
{
	symbolTable *entry;
	//variable to track if we are scanning method or class symbol table
	// 0 for method symbol table and 1 for class symbol table
	int methodOrClassTable = 0; 
	char *typeSymbol = 0; //initalize it to NULL
	if(methodSmblTable)
	{
		entry = methodSmblTable;
		methodOrClassTable = 0; //we are scanning method symbol table
	}
	else if(classSmblTable)
	{
		entry = classSmblTable;
		methodOrClassTable = 1; //we are scanning class symbol table
	}
	else //no method or class symbol table found return NULL
	{
		return typeSymbol; //return NULL
	}
	while(entry!=0)
	{
		if(!strcmp(entry->name, name))
		{
			typeSymbol = (char*)malloc(sizeof(char)*20);
			strcpy(typeSymbol, entry->type);
			return typeSymbol;
		}
		entry = entry->next;
	}
	//we have reached here may be named identifer is not in method symbol table
	//or class symbol table, or class symbol table is to be scanned
	if(methodOrClassTable)
	{
		//we have already scanned class symbol table nothing found
		//return NULL
		return typeSymbol;
	}
	else
	{
		//check class symbol table for occurance of named identifier
		if(!classSmblTable)
		{
			//class symbol table is empty return NULL
			return typeSymbol;
		}
		entry = classSmblTable;
	}
	while(entry!=0)
	{
		if(!strcmp(entry->name, name))
		{
			typeSymbol = (char*)malloc(sizeof(char)*20);
			strcpy(typeSymbol, entry->type);
			return typeSymbol;
		}
		entry = entry->next;
	}
	return typeSymbol; //nothing found
}
int indexOf(char *name)
{
	symbolTable *entry;
	//variable to track if we are scanning method or class symbol table
	// 0 for method symbol table and 1 for class symbol table
	int methodOrClassTable = 0; 
	if(methodSmblTable)
	{
		entry = methodSmblTable;
		methodOrClassTable = 0; //we are scanning method symbol table
	}
	else if(classSmblTable)
	{
		entry = classSmblTable;
		methodOrClassTable = 1; //we are scanning class symbol table
	}
	else //no method or class symbol table found return NONE
	{
		return -1;
	}
	while(entry!=0)
	{
		if(!strcmp(entry->name, name))
		{
			return entry->indexSymbol;
		}
		entry = entry->next;
	}
	//we have reached here may be named identifer is not in method symbol table
	//or class symbol table, or class symbol table is to be scanned
	if(methodOrClassTable)
	{
		//we have already scanned class symbol table nothing found
		return -1;
	}
	else
	{
		//check class symbol table for occurance of named identifier
		if(!classSmblTable)
		{
			//class symbol table is empty return -1
			return -1;
		}
		entry = classSmblTable;
	}
	while(entry!=0)
	{
		if(!strcmp(entry->name, name))
		{
			return entry->indexSymbol;
		}
		entry = entry->next;
	}
	return -1; //nothing found
}
void clearSymbolTable(int smblTable) //smblTable=0 for class symbol table, smblTable=1 for method symbol table
{
	symbolTable *entry, *next;
	if(smblTable == 0)
	{
		if(!classSmblTable) //if not class symbol table already exists
		{
			return;
		}
		entry = classSmblTable->next;
		free(classSmblTable);
		classSmblTable = 0;
	}
	else
	{
		if(!methodSmblTable) //if not method symbol table already exists
		{
			return;
		}
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
