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

int main( int argc, char *argv[] )  {

	if( argc == 2 ) {
		
		struct dirent *ent;
		char *filePeriod, jackFileName[50],vmFileName[50];
		int errorLine;
		if(!isFile(argv[1])) //directory name supplied
		{
			while ((ent = readdir (dir)) != NULL) 
			{
				filePeriod = strstr(ent->d_name,".");
				if(!strcmp(filePeriod+1,"jack"))//check for .jack extention
				{
					memset(vmFileName,0,50);
					memset(jackFileName,0,50);
					snprintf(jackFileName, filePeriod - ent->d_name + 1, "%s", ent->d_name);
					sprintf(vmFileName,"%s/%s.vm",argv[1],jackFileName);
					memset(jackFileName,0,50);
					sprintf(jackFileName,"%s/%s",argv[1],ent->d_name);
					constructorTokenizer(jackFileName);
					constructorCompilationEngine(); 
					constructorSymbolTable();
					constructorVMWriter(vmFileName);
					compileClass();
					freeToken(); //free tokens previously built in current file
					fclose(vmFile);
				}
			}
			closedir (dir);
					
		}
		else  //file name supplied
		{
			filePeriod = strstr(argv[1],".");
			if(strcmp(filePeriod+1,"jack")!=0)//check for .jack extention
			{
				printf("Please specify file with .jack extention!\n");
				return 1;
			} //check for .vm extension
			memset(vmFileName,0,50);
			memset(jackFileName,0,50);
			snprintf(jackFileName, filePeriod - argv[1] + 1, "%s", argv[1]);
			sprintf(vmFileName,"%s.vm",jackFileName);
			constructorTokenizer(argv[1]);
			constructorCompilationEngine(); //to be modified eventually
			constructorSymbolTable();
			constructorVMWriter(vmFileName);
			compileClass();
			printf("After compileClass\n");
			freeToken(); //free tokens previously built in current file
			fclose(vmFile);
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
