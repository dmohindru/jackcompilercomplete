/* vmwriter.h
 * This file implements VM Writer Module of Jack Compiler
 * for Hack computer
 * Author: Dhruv Mohindru
 * Date: 14/06/2016
 */
#ifndef VMWRITER_H
#define VMWRITER_H
#include <stdio.h>
#include <stdlib.h>
FILE *vmFile; //current vm file pointer for writing to
typedef enum segment
{
	CONST_SEG, ARG_SEG, LOCAL_SEG, STATIC_SEG, THIS_SEG, THAT_SEG, POINTER_SEG, TEMP_SEG
}segment; 
typedef enum command
{
	ADD, SUB, NEG, EQ, GT, LT, AND, OR, NOT
}command;
void constructorVMWriter(char *fileName);
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
