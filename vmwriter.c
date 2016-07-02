/* vmwriter.c
 * This file implements VM Writer Module of Jack Compiler
 * for Hack computer
 * Author: Dhruv Mohindru
 * Date: 23/03/2016
 */
#include "vmwriter.h"
void constructorVMWriter(char *fileName)
{
	//create vm file for writing
	vmFile = fopen ( fileName, "w" );
}
void writePush(segment segType, int index)
{
	switch(segType)
	{
		case CONST_SEG:
			fprintf(vmFile, "push constant %d\n", index);
			break;
		case ARG_SEG:
			fprintf(vmFile, "push argument %d\n", index);
			break;
		case LOCAL_SEG:
			fprintf(vmFile, "push local %d\n", index);
			break;
		case STATIC_SEG:
			fprintf(vmFile, "push static %d\n", index);
			break;
		case THIS_SEG:
			fprintf(vmFile, "push this %d\n", index);
			break;
		case THAT_SEG:
			fprintf(vmFile, "push that %d\n", index);
			break;
		case POINTER_SEG:
			fprintf(vmFile, "push pointer %d\n", index);
			break;
		case TEMP_SEG:
			fprintf(vmFile, "push temp %d\n", index);
			break;
		default: //we may never land into this block of code by just in case
			close();
			exit(0);
	}
}
void writePop(segment segType, int index)
{
	switch(segType)
	{
		case CONST_SEG:
			fprintf(vmFile, "pop constant %d\n", index);
			break;
		case ARG_SEG:
			fprintf(vmFile, "pop argument %d\n", index);
			break;
		case LOCAL_SEG:
			fprintf(vmFile, "pop local %d\n", index);
			break;
		case STATIC_SEG:
			fprintf(vmFile, "pop static %d\n", index);
			break;
		case THIS_SEG:
			fprintf(vmFile, "pop this %d\n", index);
			break;
		case THAT_SEG:
			fprintf(vmFile, "pop that %d\n", index);
			break;
		case POINTER_SEG:
			fprintf(vmFile, "pop pointer %d\n", index);
			break;
		case TEMP_SEG:
			fprintf(vmFile, "pop temp %d\n", index);
			break;
		default: //we may never land into this block of code by just in case
			close();
			exit(0);
	}
}
void writeArithmetic(command commType)
{
	switch(commType)
	{
		case ADD:
			fprintf(vmFile, "add\n");
			break;
		case SUB:
			fprintf(vmFile, "sub\n");
			break;
		case NEG:
			fprintf(vmFile, "neg\n");
			break;
		case EQ:
			fprintf(vmFile, "eq\n");
			break;
		case GT:
			fprintf(vmFile, "gt\n");
			break;
		case LT:
			fprintf(vmFile, "lt\n");
			break;
		case AND:
			fprintf(vmFile, "and\n");
			break;
		case OR:
			fprintf(vmFile, "or\n");
			break;
		case NOT:
			fprintf(vmFile, "not\n");
			break;
		default: //we may never land into this block of code by just in case
			close();
			exit(0);
	}
}
void writeLabel(char *label)
{
	fprintf(vmFile, "label %s\n", label);
}
void writeGoto(char *label)
{
	fprintf(vmFile, "goto %s\n", label);
}
void writeIf(char *label)
{
	fprintf(vmFile, "if-goto %s\n", label);
}
void writeCall(char *name, int nArgs)
{
	fprintf(vmFile, "call %s %d\n", name, nArgs);
}
void writeFunction(char *name, int nLocals)
{
	fprintf(vmFile, "function %s %d\n", name, nLocals);
}
void writeReturn()
{
	fprintf(vmFile, "return\n");
}
void close()
{
	//close the vm file after writing to it
	fclose(vmFile);
}
