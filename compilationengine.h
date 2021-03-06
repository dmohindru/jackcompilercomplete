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
#include "vmwriter.h"
char className[100], functionName[100];
int numOfParameter, numOfLocals, isVoid, loopIfIndex, functionType;
//functionType = 0 for function, 1 for constructor, 2 for method 
void constructorCompilationEngine();
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
