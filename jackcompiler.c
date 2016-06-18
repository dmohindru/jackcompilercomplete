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
	char symbolTemp;
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
	return 0;
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
		char *filePeriod, vmFileName[50], jackFileName[50];
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
					memset(jackFileName,0,50);
					printf("--------------------------\n");
					snprintf(jackFileName, filePeriod - ent->d_name + 1, "%s", ent->d_name);
					sprintf(vmFileName,"%s/%s.xml",argv[1],jackFileName);
					printf("xml file name %s\n", vmFileName);
					memset(jackFileName,0,50);
					sprintf(jackFileName,"%s/%s",argv[1],ent->d_name);
					constructorTokenizer(jackFileName);
					constructorCompilationEngine(vmFileName);
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
			memset(jackFileName,0,50);
			printf("--------------------------\n");
			printf ("Reading file: %s\n", argv[1]);
			snprintf(jackFileName, filePeriod - argv[1] + 1, "%s", argv[1]);
			sprintf(vmFileName,"%s.xml",jackFileName);
			printf("xml file name %s\n", vmFileName);
			constructorTokenizer(argv[1]);
			constructorCompilationEngine(vmFileName);
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
			printf("--------------------------\n");	
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
