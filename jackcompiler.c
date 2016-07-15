/* jackcompiler.c
 * This file main() function for jack compiler
 * for Hack computer
 * Author: Dhruv Mohindru
 * Date: 23/03/2016
 */
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include "jacktokenizer.h"
#include "compilationengine.h"
int isFile(const char* name)
{
    dir = opendir(name);

    if(dir != NULL)
    {
		return 0;
    }

    return 1;
}
/*
void readDir() //and display only files with vm entension
{
	struct dirent *ent;
	char *filePeriod;

	//print all the files and directories within directory 
	while ((ent = readdir (dir)) != NULL) {
		filePeriod = strstr(ent->d_name,".");
		if(!strcmp(filePeriod+1,"vm"))//check for .vm extention
		{
			printf ("%s\n", ent->d_name);
		}
	}
	closedir (dir);
}*/

int readJackFile() //return line no if error occurs else return 0
{
	/*char symbolTemp;
	while(hasMoreTokens())
	{
		advance();
		switch(tokenType())
		{
			case KEYWORD:
				switch(keyWord())
				{
					case CLASS:
						fprintf(vmFile, "<keyword> class </keyword>\n");
						break;
					case METHOD:
						fprintf(vmFile, "<keyword> method </keyword>\n");
						break;
					case FUNCTION:
						fprintf(vmFile, "<keyword> function </keyword>\n");
						break;
					case CONSTRUCTOR:
						fprintf(vmFile, "<keyword> constructor </keyword>\n");
						break;
					case INT:
						fprintf(vmFile, "<keyword> int </keyword>\n");
						break;
					case BOOLEAN:
						fprintf(vmFile, "<keyword> boolean </keyword>\n");
						break;
					case CHAR:
						fprintf(vmFile, "<keyword> char </keyword>\n");
						break;
					case VOID:
						fprintf(vmFile, "<keyword> void </keyword>\n");
						break;
					case VAR:
						fprintf(vmFile, "<keyword> var </keyword>\n");
						break;
					case STATIC:
						fprintf(vmFile, "<keyword> static </keyword>\n");
						break;
					case FIELD:
						fprintf(vmFile, "<keyword> field </keyword>\n");
						break;
					case LET:
						fprintf(vmFile, "<keyword> let </keyword>\n");
						break;
					case DO:
						fprintf(vmFile, "<keyword> do </keyword>\n");
						break;
					case IF:
						fprintf(vmFile, "<keyword> if </keyword>\n");
						break;
					case ELSE:
						fprintf(vmFile, "<keyword> else </keyword>\n");
						break;
					case WHILE:
						fprintf(vmFile, "<keyword> while </keyword>\n");
						break;
					case RETURN:
						fprintf(vmFile, "<keyword> return </keyword>\n");
						break;
					case TRUE:
						fprintf(vmFile, "<keyword> true </keyword>\n");
						break;
					case FALSE:
						fprintf(vmFile, "<keyword> false </keyword>\n");
						break;
					case NULL_KEYWORD:
						fprintf(vmFile, "<keyword> null </keyword>\n");
						break;
					case THIS:
						fprintf(vmFile, "<keyword> this </keyword>\n");
						break;
					case ERROR_KEYWORD:
						return currentToken->line;
				};
				break;
			case SYMBOL:
				symbolTemp = symbol();
				if(symbolTemp == '>')
				{
					fprintf(vmFile, "<symbol> &gt; </symbol>\n");
				}
				else if(symbolTemp == '<')
				{
					fprintf(vmFile, "<symbol> &lt; </symbol>\n");
				}
				else if(symbolTemp == '&')
				{
					fprintf(vmFile, "<symbol> &amp; </symbol>\n");
				}
				else
				{
					fprintf(vmFile, "<symbol> %c </symbol>\n", symbolTemp);
				}
				break;
			case IDENTIFIER:
				fprintf(vmFile, "<identifier> %s </identifier>\n", identifier());
				break;
			case INT_CONST:
				fprintf(vmFile, "<integerConstant> %d </integerConstant>\n", intVal());
				break;
			case STRING_CONST:
				fprintf(vmFile, "<stringConstant> %s </stringConstant>\n", stringVal());
				break;
			case ERROR_TOKEN:
				return currentToken->line; 
		};
	}
	return 0;*/
}
void testVMCommands()
{
	//testing push all segements
	writePush(LOCAL_SEG, 2);
	writePush(ARG_SEG, 0);
	writePush(STATIC_SEG, 1);
	writePush(CONST_SEG, 6);
	writePush(THIS_SEG, 4);
	writePush(THAT_SEG, 3);
	writePush(POINTER_SEG, 4);
	writePush(TEMP_SEG, 3);
	//testing pop all segments
	writePop(LOCAL_SEG, 0);
	writePop(ARG_SEG, 0);
	writePop(STATIC_SEG, 10);
	writePop(CONST_SEG, 0);
	writePop(THIS_SEG, 4);
	writePop(THAT_SEG, 0);
	writePop(POINTER_SEG, 0);
	writePop(TEMP_SEG, 0);
	//test all arthmatic and logical commands
	writeArithmetic(ADD);
	writeArithmetic(SUB);
	writeArithmetic(NEG);
	writeArithmetic(EQ);
	writeArithmetic(GT);
	writeArithmetic(LT);
	writeArithmetic(AND);
	writeArithmetic(OR);
	writeArithmetic(NOT);
	// other commands
	writeFunction("Dummy", 4);
	writeCall("DummyCall", 3);
	writeIf("DummyLabel");
	writeLabel("DummyLabel");
	writeGoto("DummyLabel");
	writeReturn();
}
void testSymbolTable() //just a temporary function to test symbol api
{
	char *str;
	//first populate symbol tables
	define("nAccounts", "int", STATIC_SMBL);
	define("bankCommission", "int", STATIC_SMBL);
	define("id", "int", FIELD_SMBL);
	define("owner", "String", FIELD_SMBL);
	define("balance", "int", FIELD_SMBL);
	define("this", "BankAccount", ARG_SMBL);
	define("sum", "int", ARG_SMBL);
	define("from", "BankAccount", ARG_SMBL);
	define("when", "Date", ARG_SMBL);
	define("i", "int", VAR_SMBL);
	define("j", "int", VAR_SMBL);
	define("due", "Date", VAR_SMBL);
	//then display them
	printf("Displaying class symbol table\n");
	printf("--------------------------------------\n");
	displaySymbolTable(0); //display class symbol table
	printf("--------------------------------------\n");
	printf("Displaying method symbol table\n");
	printf("--------------------------------------\n");
	displaySymbolTable(1); //display class symbol table
	printf("--------------------------------------\n");
	//now test other api's
	//varCount
	printf("Testing varCount function\n");
	printf("--------------------------------------\n");
	printf("Number of static variables: %d\n", varCount(STATIC_SMBL));
	printf("Number of field variables: %d\n", varCount(FIELD_SMBL));
	printf("Number of argument variables: %d\n", varCount(ARG_SMBL));
	printf("Number of var variables: %d\n", varCount(VAR_SMBL));
	printf("--------------------------------------\n");
	//kindOf
	printf("Testing kindOf function\n");
	printf("--------------------------------------\n");
	printf("Kind of variable named 'balance' ");
	switch(kindOf("balance"))
	{
		case STATIC_SMBL:
			printf("static\n");
			break;
		case FIELD_SMBL:
			printf("field\n");
			break;
		case ARG_SMBL:
			printf("argument\n");
			break;
		case VAR_SMBL:
			printf("var\n");
			break;
		default:
			printf("Unknown type\n");
	}
	printf("Kind of variable named 'sum' ");
	switch(kindOf("sum"))
	{
		case STATIC_SMBL:
			printf("static\n");
			break;
		case FIELD_SMBL:
			printf("field\n");
			break;
		case ARG_SMBL:
			printf("argument\n");
			break;
		case VAR_SMBL:
			printf("var\n");
			break;
		default:
			printf("Unknown type\n");
	}
	printf("--------------------------------------\n");
	printf("Testing typeOf function\n");
	printf("--------------------------------------\n");
	str = typeOf("owner");
	printf("Type of variable 'owner' is %s\n", str);
	free(str);
	str = typeOf("nAccounts");
	printf("Type of variable 'nAccounts' is %s\n", str);
	free(str);
	str = typeOf("from");
	printf("Type of variable 'from' is %s\n", str);
	free(str);
	str = typeOf("j");
	printf("Type of variable 'j' is %s\n", str);
	free(str);
	printf("--------------------------------------\n");
	printf("Testing indexOf function\n");
	printf("--------------------------------------\n");
	printf("Index of variable 'nAccounts' is %d\n", indexOf("nAccounts"));
	printf("Index of variable 'id' is %d\n", indexOf("id"));
	printf("Index of variable 'sum' is %d\n", indexOf("sum"));
	printf("Index of variable 'when' is %d\n", indexOf("when"));
	printf("Index of variable 'j' is %d\n", indexOf("j"));
	printf("--------------------------------------\n");
}
int compileJackFile()
{
	compileClass();
	return 1; //change this appropriately
}
int main( int argc, char *argv[] )  {

	if( argc == 2 ) {
		
		
		//FILE *asmFile;
		struct dirent *ent;
		//char asmFileName[30], *fileName, *filePeriod, vmFileName[50];
		char *filePeriod, vmFileName1[50], jackFileName[50],vmFileName[50];
		int errorLine;
		printf("The argument supplied is %s\n", argv[1]);
		if(!isFile(argv[1])) //directory name supplied
		{
			while ((ent = readdir (dir)) != NULL) 
			{
				filePeriod = strstr(ent->d_name,".");
				if(!strcmp(filePeriod+1,"jack"))//check for .jack extention
				{
					printf ("Reading file: %s\n", ent->d_name);
					memset(vmFileName,0,50);
					memset(vmFileName1,0,50);
					memset(jackFileName,0,50);
					printf("--------------------------\n");
					snprintf(jackFileName, filePeriod - ent->d_name + 1, "%s", ent->d_name);
					sprintf(vmFileName1,"%s/%s.xml",argv[1],jackFileName); //temp to be removed eventually
					sprintf(vmFileName,"%s/%s.vm",argv[1],jackFileName);
					printf("xml file name %s\n", vmFileName1); //temp to be removed eventually
					printf("vm file name %s\n", vmFileName);
					memset(jackFileName,0,50);
					sprintf(jackFileName,"%s/%s",argv[1],ent->d_name);
					constructorTokenizer(jackFileName);
					constructorCompilationEngine(vmFileName1); //to be modified eventually
					constructorSymbolTable();
					constructorVMWriter(vmFileName);
					compileClass();
					/*errorLine = readJackFile();
					if(errorLine>0)
					{
						printf("Error at line no: %d\n", errorLine);
						return 1;
					}*/
					freeToken(); //free tokens previously built in current file
					fclose(vmFile);
					printf("--------------------------\n");
				}
			}
			closedir (dir);
			//close();//close asm file
			
		}
		else  //file name supplied
		{
			filePeriod = strstr(argv[1],".");
			//memset(asmFileName,0,30);
			if(strcmp(filePeriod+1,"jack")!=0)//check for .jack extention
			{
				printf("Please specify file with .jack extention!\n");
				return 1;
			} //check for .vm extension
			memset(vmFileName,0,50);
			memset(vmFileName1,0,50);
			memset(jackFileName,0,50);
			//printf("--------------------------\n");
			printf ("Reading file: %s\n", argv[1]);
			snprintf(jackFileName, filePeriod - argv[1] + 1, "%s", argv[1]);
			sprintf(vmFileName1,"%s.xml",jackFileName); //temp to be removed eventually
			sprintf(vmFileName,"%s.vm",jackFileName);
			//printf("xml file name %s\n", vmFileName);
			constructorTokenizer(argv[1]);
			constructorCompilationEngine(vmFileName1); //to be modified eventually
			constructorSymbolTable();
			constructorVMWriter(vmFileName);
			//testVMCommands(); // a very temp stuff
			//testSymbolTable(); // a very temp stuff
			compileClass();
			/*errorLine = readJackFile();
			//close();//close asm file
			if(errorLine>0)
			{
				printf("\nError at line no: %d\n", errorLine);
				return 1;
			}*/
			freeToken(); //free tokens previously built in current file
			fclose(vmFile);
			//printf("--------------------------\n");	
		}
		
		return 0;
	}
	else if( argc > 2 ) {
		printf("Too many arguments supplied.\n");
		return 1;
	}
	else {
		printf("One argument expected.\n");
		return 1;
	}
	
}
