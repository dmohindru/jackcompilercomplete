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
	clearSymbolTable(classSmblTable);
	clearSymbolTable(methodSmblTable);
	//set them to null
	classSmblTable = 0;
	methodSmblTable = 0;
}
void startSubroutine()
{
	//clear old method symbol table
	clearSymbolTable(methodSmblTable);
	//set it to null
	methodSmblTable = 0;
}
void define(char *name, char *type, kind kindType)
{
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
void clearSymbolTable(symbolTable *smblTable)
{
}
