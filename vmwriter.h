/* vmwriter.h
 * This file implements VM Writer Module of Jack Compiler
 * for Hack computer
 * Author: Dhruv Mohindru
 * Date: 14/06/2016
 */
#ifndef VMWRITER_H
#define VMWRITER_H
#include <stdio.h>
typedef enum segment
{
	CONST, ARG, LOCAL, STATIC, THIS, THAT, POINTER, TEMP
}segment; 
typedef enum command
{
	ADD, SUB, NEG, EQ, GT, LT, AND, OR, NOT
}command;
void constructorVMWriter(FILE *vmFile);
void writePush(segment segType, int index);
void writePop(segment segType, int index);
void writeArithmetic(command commType);
void writeLabel(char *label);
void wrtieGoto(char *label);
void writeIf(char *label);
void writeCall(char *name, int nArgs);
void writeFunction(char *name, int nLocals);
void writeReturn();
void close();
#endif
