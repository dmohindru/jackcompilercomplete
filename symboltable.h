/* symboltable.h
 * This file implements Symbol Table Module of Jack Compiler
 * for Hack computer
 * Author: Dhruv Mohindru
 * Date: 14/06/2016
 */
#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
typedef enum kind
{
	STATIC_SMBL, FIELD_SMBL, ARG_SMBL, VAR_SMBL
}kind;
typedef struct symbolTable
{
	char name[100];
	char type[20];
	kind kindType;
	int indexSymbol;
	struct symbolTable *next;
}symbolTable;
struct symbolTable *classSmblTable, *methodSmblTable;
void constructorSymbolTable();
void startSubroutine();
void define(char *name, char *type, kind kindType);
int varCount(kind kindType);
kind kindOf(char *name);
char* typeOf(char *name);
int indexOf(char *name);
void clearSymbolTable(symbolTable *smblTable);

#endif
