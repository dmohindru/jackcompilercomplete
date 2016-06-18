/* compilationengine.h
 * This file implements CompilationEngine Module of Jack Compiler
 * for Hack computer
 * Author: Dhruv Mohindru
 * Date: 23/03/2016
 */
#ifndef COMPILATIONENGINE_H
#define COMPILATIONENGINE_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "jacktokenizer.h"
#include "symboltable.h"
FILE *vmFile; //current vm file pointer for reading
char indentString[100];
//char currentFunction[50];
//int incrementer;
void constructorCompilationEngine(char *filename);
void compileClass();
void compileClassVarDec();
void compileSubroutine();
void compileParameterList();
void compileVarDec();
void compileStatements();
void compileDo();
void compileLet();
void compileWhile();
void compileReturn();
void compileIf();
void compileExpression();
void compileTerm();
void compileExpressionList();
#endif
