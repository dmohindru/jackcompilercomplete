/* compilationengine.c
 * This file implements CompilationEngine Module of Jack Compiler
 * for Hack computer
 * Author: Dhruv Mohindru
 * Date: 23/03/2016
 */
#include "compilationengine.h"
void constructorCompilationEngine(char *fileName)
{
	//TODO: As check for filename extenstion .asm
	xmlFile = fopen ( fileName, "w" );
	//reset className, functionName buffer
	memset(className, 0, 100); 
	memset(functionName, 0, 100);
	numOfParameter = 0;
	numOfLocals = 0;
	isVoid = 0; //return type of function 0 = not void, 1 = void
	loopIfIndex = 0; //index for unique while or if command label
	functionType = 0; //if subroutine is constructor 0 = Not, 1 = Is Constuctor
	//fprintf(xmlFile, "XML Content for file %s\n", fileName);
	//incrementer = 0;
	//temp stuff
	//memset(currentFunction,0,50);
	//strcpy(currentFunction,"dummy");
	//temp stuff end
	//if ( asmFile == NULL )
	//{
    //  perror(fileName);
    //}
}
void compileClass()
{
	//memset(indentString, 0, 100);
	if(!hasMoreTokens()) //if block for class keyword
	{
		printf("Could not find class keyword at start of file\n");
		return; //or may be exit  
	}
	else
	{
		advance();//get a keyword class
		if(keyWord() != CLASS)
		{
			printf("Could not find class keyword at start of file\n");
			return; //or may be exit
		}
		//write here to vm file <class> tag here
		//fprintf(xmlFile, "<class>\n");
		//strcat(indentString, "  ");//increase indentation
		//fprintf(xmlFile, "%s<keyword> class </keyword>\n", indentString);
	}
	
	if(!hasMoreTokens()) //if block for class name identifier
	{
		printf("Could not file class name identifier\n");
		return;
	}
	else
	{
		advance();
		if(tokenType() != IDENTIFIER)
		{
			printf("Could not file class name identifier\n");
			return;
		}
		//write here to vm file name identifier tag <identifier> className </identifer>
		strcpy(className, identifier()); //get the class name
		//fprintf(xmlFile, "%s<identifier> %s </identifier>\n", indentString, identifier());
	}
	
	if(!hasMoreTokens()) //if block for '{' symbol
	{
		printf("Could not find symbol '{' after class decleration\n");
		return;
	}
	else
	{
		advance();
		if(tokenType() != SYMBOL || symbol() != '{')
		{
			printf("Could not find symbol '{' after class decleration\n");
			return;
		}
		//write here to vm file for symbol { tag <symbol> { </symbol>
		//fprintf(xmlFile, "%s<symbol> { </symbol>\n", indentString);
	}
	compileClassVarDec(); //compile class variable decleration
	//we should have now class symbol table ready so display it
	printf("-------------------------------------------------\n");
	printf("Displaying Class symbol table\n");
	displaySymbolTable(0);
	printf("-------------------------------------------------\n");
	/////////////////////////////
	compileSubroutine(); //compile class subroutines
	
	//token as already been read by previous function just check for 
	//syntatic correctness
	if(tokenType() != SYMBOL || symbol() != '}')
	{
		printf("Could not find symbol '}' for class decleration\n");
		return;
	}
	//write here to vm file for symbol } tag <symbol> } </symbol>
	//fprintf(xmlFile, "%s<symbol> } </symbol>\n", indentString);
	//finally write a ending class tag </class>
	//fprintf(xmlFile, "</class>");
}
void compileClassVarDec()
{
	//variables used for define function in symbol table module
	char nameOfVar[100];
	char typeOfVar[100];
	kind kindOfVar;
	//check for static or field keyword code block
	if(!hasMoreTokens()) //if no more tokens return to compileClass method
	{			
		return;
	}
	else
	{
		advance(); //read next token
		if(tokenType() == KEYWORD)
		{
			switch(keyWord())
			{
				case STATIC:
					fprintf(xmlFile, "%s<classVarDec>\n", indentString);
					strcat(indentString, "  "); //increase the indent
					fprintf(xmlFile, "%s<keyword> static </keyword>\n", indentString);
					kindOfVar = STATIC_SMBL;
					break;
				case FIELD:
					fprintf(xmlFile, "%s<classVarDec>\n", indentString);
					strcat(indentString, "  ");
					fprintf(xmlFile, "%s<keyword> field </keyword>\n", indentString);
					kindOfVar = FIELD_SMBL; 
					break;
				default:   //static or field keyword not found return to compileClass Method
					return;
			}
		}
		else //if the token type in not keyword this may be possible some other statement
		{	  //return to compileClass method	
			return;
		}
	}
	//we have come so far so it must be a classVarDecleration
	//check for syntatic correctness and exit program if some errors are found
	//check for 'type' token code block
	if(!hasMoreTokens()) 
	{
		printf("token 'type' not found at line %d\n", currentToken->line);
		freeToken();
		fclose(xmlFile);
		exit(1);
	}
	else
	{
		advance(); //get the next 'type' token
		if(tokenType() == KEYWORD)
		{
			switch(keyWord())
			{
				case INT:
					fprintf(xmlFile, "%s<keyword> int </keyword>\n", indentString);
					strcpy(typeOfVar, "int");
					break;
				case CHAR:
					fprintf(xmlFile, "%s<keyword> char </keyword>\n", indentString);
					strcpy(typeOfVar, "char");
					break;
				case BOOLEAN:
					fprintf(xmlFile, "%s<keyword> boolean </keyword>\n", indentString);
					strcpy(typeOfVar, "boolean");
					break;
				default: //not a valid keyword found in 'type' decleration
					printf("Variable declareation unknown 'type' at line %d\n", currentToken->line);
					freeToken();
					fclose(xmlFile);
					exit(1);
			}
		}
		else if(tokenType() == IDENTIFIER)
		{
			fprintf(xmlFile, "%s<identifier> %s </identifier>\n", indentString, identifier());
			strcpy(typeOfVar, identifier());
			
		}
		else  //not found legal keyword or identifier in 'type' decleration
		{
			printf("Variable declareation unknown type at line %d\n", currentToken->line);
			freeToken();
			fclose(xmlFile);
			exit(1);
		}
	}
	//variable name if block
	if(!hasMoreTokens()) 
	{
		printf("token variable name not found at line %d\n", currentToken->line);
		freeToken();
		fclose(xmlFile);
		exit(1);
	}
	else
	{
		advance(); //get next token for variable name
		if(tokenType() == IDENTIFIER)
		{
			fprintf(xmlFile, "%s<identifier> %s </identifier>\n", indentString, identifier());
			strcpy(nameOfVar, identifier());
		}
		else //not a valid variable name token
		{
			printf("token variable not found at line %d\n", currentToken->line);
			freeToken();
			fclose(xmlFile);
			exit(1);
		}
	}
	//we have got all the details to define a symbol call define
	define(nameOfVar, typeOfVar, kindOfVar);
	//below block of code for occurance of code like in next line
	//(',' varName)* ';' block
	if(!hasMoreTokens())
	{
		printf("terminating symbol ';' or ',' not found at line %d\n", currentToken->line);
		freeToken();
		fclose(xmlFile);
		exit(1);
	}
	else
	{
		advance();
		if(tokenType() == SYMBOL)
		{
			while(1)
			{
				//first clear nameOfVar for define a new symbol
				memset(nameOfVar, 0, 20);
				if(symbol() == ';')
				{
					fprintf(xmlFile, "%s<symbol> ; </symbol>\n", indentString);
					//strncpy(indentString, indentString, strlen(indentString)-2);
					indentString[strlen(indentString)-2] = '\0'; //decrease the indent
					fprintf(xmlFile, "%s</classVarDec>\n", indentString);
					break;
				}
				else if(symbol() == ',')
				{
					fprintf(xmlFile, "%s<symbol> , </symbol>\n", indentString);
					if(!hasMoreTokens()) //check for variable name token
					{
						printf("token variable name not found at line %d\n", currentToken->line);
						freeToken();
						fclose(xmlFile);
						exit(1);
					}
					advance(); //get the next variable name token 
					if(tokenType() == IDENTIFIER)
					{
						fprintf(xmlFile, "%s<identifier> %s </identifier>\n", indentString, identifier());
						strcpy(nameOfVar, identifier());
						//we got the name call define
						define(nameOfVar, typeOfVar, kindOfVar);
					}
					else //not a valid variable name
					{
						printf("token variable not found at line %d\n", currentToken->line);
						freeToken();
						fclose(xmlFile);
						exit(1);
					}	
				}
				if(!hasMoreTokens()) // check for more token of type ',' or ';'
				{
					printf("terminating symbol ';' or ',' not found at line %d\n", currentToken->line);
					freeToken();
					fclose(xmlFile);
					exit(1);
				}
				else
				{
					advance();
				}
			}
		}
		else
		{
			printf("terminating symbol ';' or ',' not found at line %d\n", currentToken->line);
			freeToken();
			fclose(xmlFile);
			exit(1);
		}
	}
	//at the end recussively call compileClassVarDec
	compileClassVarDec();
}
void compileSubroutine()
{
	//check for constructor or function or method keyword code block
	//since token has been ready by previous function like compileVarDec or 
	//compileSubroutine which is called recussively
	char vmFunctionName[200];
	isVoid = 0;
	functionType = 0; //by default functionType is for function subroutine
	memset(vmFunctionName, 0, 200); //reset vmFunctionName
	if(tokenType() == KEYWORD)
	{
		switch(keyWord())
		{
			case CONSTRUCTOR:
				functionType = 1;
				//fprintf(xmlFile, "%s<subroutineDec>\n", indentString);
				//strcat(indentString, "  ");//increase the indent
				//fprintf(xmlFile, "%s<keyword> constructor </keyword>\n", indentString);
				break;
			case METHOD:
				functionType = 2;
				//fprintf(xmlFile, "%s<subroutineDec>\n", indentString);
				//strcat(indentString, "  ");//increase the indent
				//fprintf(xmlFile, "%s<keyword> function </keyword>\n", indentString); 
				break;
			case FUNCTION:
				//fprintf(xmlFile, "%s<subroutineDec>\n", indentString);
				//strcat(indentString, "  ");//increase the indent
				//fprintf(xmlFile, "%s<keyword> method </keyword>\n", indentString);
				break;
			default:   //static or field keyword not found return to compileClass Method
				//printf("Not found any thing\n");
				return;
		}
		
	}
	else //if the token type in not keyword this may be possible some other statement
	{	  //return to compileClass method	
		return;
	}
	//we have come so far so it must be a subroutine Decleration
	//here we can safely start a new subroutine symbol table
	startSubroutine();
	//check for syntatic correctness and exit program if some errors are found
	//check for 'type' token code block
	if(!hasMoreTokens()) 
	{
		printf("token 'type' not found at line %d\n", currentToken->line);
		freeToken();
		fclose(xmlFile);
		exit(1);
	}
	else
	{
		advance(); //get the next 'type' token
		if(tokenType() == KEYWORD)
		{
			switch(keyWord())
			{
				case INT:
					//fprintf(xmlFile, "%s<keyword> int </keyword>\n", indentString);
					break;
				case CHAR:
					//fprintf(xmlFile, "%s<keyword> char </keyword>\n", indentString);
					break;
				case BOOLEAN:
					//fprintf(xmlFile, "%s<keyword> boolean </keyword>\n", indentString);
					break;
				case VOID:
					//fprintf(xmlFile, "%s<keyword> void </keyword>\n", indentString);
					isVoid = 1;
					break;
				default: //not a valid keyword found in 'type' decleration
					printf("Return 'type' unknown at line %d\n", currentToken->line);
					freeToken();
					fclose(xmlFile);
					exit(1);
			}
		}
		else if(tokenType() == IDENTIFIER)
		{
			//fprintf(xmlFile, "%s<identifier> %s </identifier>\n", indentString, identifier());
		}
		else  //not found legal keyword or identifier in 'type' decleration
		{
			printf("Return 'type' unknown at line %d\n", currentToken->line);
			freeToken();
			fclose(xmlFile);
			exit(1);
		}
	}
	//subroutine name check block
	if(!hasMoreTokens()) 
	{
		printf("token subroutine name not found at line %d\n", currentToken->line);
		freeToken();
		fclose(xmlFile);
		exit(1);
	}
	else
	{
		advance(); //get next token for subroutine name
		if(tokenType() == IDENTIFIER)
		{
			//fprintf(xmlFile, "%s<identifier> %s </identifier>\n", indentString, identifier());
			strcpy(functionName, identifier()); //get the function name
		}
		else //not a valid subroutine name token
		{
			printf("invalid subroutine name at line %d\n", currentToken->line);
			freeToken();
			fclose(xmlFile);
			exit(1);
		}
	}
	//symbol '(' check block
	//here set numOfLocals to zero
	numOfLocals = 0;
	if(!hasMoreTokens()) 
	{
		printf("expected symbol '(' at line %d\n", currentToken->line);
		freeToken();
		fclose(xmlFile);
		exit(1);
	}
	else
	{
		advance(); //get next token for symbol '('
		if(tokenType() == SYMBOL && symbol() == '(')
		{
			//fprintf(xmlFile, "%s<symbol> ( </symbol>\n", indentString);
		}
		else //not a valid subroutine name token
		{
			printf("expected symbol '(' at line %d\n", currentToken->line);
			freeToken();
			fclose(xmlFile);
			exit(1);
		}
	}
	//compile paramater list
	compileParameterList();
	//since next token has already been read by compileParameterList() so just
	//check for symbol ')'
	if(tokenType() != SYMBOL || symbol() != ')')
	{
		printf("expected symbol ')' at line %d\n", currentToken->line);
		freeToken();
		fclose(xmlFile);
		exit(1);
	}
	//fprintf(xmlFile, "%s<symbol> ) </symbol>\n", indentString);
	//compile subroutine body
	//fprintf(xmlFile, "%s<subroutineBody>\n", indentString);
	//strcat(indentString, "  ");//increase the indent
	//check for symbol '{' for start of subroutine body
	if(!hasMoreTokens()) 
	{
		printf("expected symbol '{' at line for subroutine body %d\n", currentToken->line);
		freeToken();
		fclose(xmlFile);
		exit(1);
	}
	else
	{
		advance(); //get next token for symbol '{'
		if(tokenType() == SYMBOL && symbol() == '{')
		{
			//fprintf(xmlFile, "%s<symbol> { </symbol>\n", indentString);
		}
		else //not a valid subroutine name token
		{
			printf("expected symbol '{' at line for subroutine body %d\n", currentToken->line);
			freeToken();
			fclose(xmlFile);
			exit(1);
		}
	}
	//compile var decleration
	compileVarDec();
	//we should have now method symbol table ready so display it
	printf("-------------------------------------------------\n");
	printf("Displaying Method symbol table\n");
	displaySymbolTable(1);
	printf("-------------------------------------------------\n");
	/////////////////////////////////////////////////
	//**here we should write functoin name nLocals to vm file as we 
	//**should have by now total number of local in a function
	sprintf(vmFunctionName, "%s.%s", className, functionName);
	writeFunction(vmFunctionName, numOfLocals);
	//if its a constructor then allocate memory for field var here
	if(functionType == 1)
	{
		writePush(CONST_SEG, varCount(FIELD_SMBL));
		writeCall("Memory.alloc", 1);
		writePop(POINTER_SEG, 0);
	}
	else if(functionType == 2)
	{
		writePush(ARG_SEG, 0);
		writePop(POINTER_SEG, 0);
	}
	//compile statements
	//fprintf(xmlFile, "%s<statements>\n", indentString);
	//strcat(indentString, "  ");//increase the indent
	compileStatements();
	//indentString[strlen(indentString)-2] = '\0'; //decrease the indent
	//fprintf(xmlFile, "%s</statements>\n", indentString);
	//since next token has already been read by compileVarDec() or compileStatements so just
	//check for symbol '}'
	if(tokenType() != SYMBOL || symbol() != '}')
	{
		printf("expected symbol '}' at line %d\n", currentToken->line);
		freeToken();
		fclose(xmlFile);
		exit(1);
	}
	//fprintf(xmlFile, "%s<symbol> } </symbol>\n", indentString);
	//indentString[strlen(indentString)-2] = '\0'; //decrease the indent
	//fprintf(xmlFile, "%s</subroutineBody>\n", indentString);
	//indentString[strlen(indentString)-2] = '\0'; //decrease the indent
	//fprintf(xmlFile, "%s</subroutineDec>\n", indentString);
	//recussively call compileSubroutine
	if(!hasMoreTokens())
	{
		printf("expected symbol '}' for class decleration\n");
		freeToken();
		fclose(xmlFile);
		exit(1);
	}
	else
	{
		advance(); //advance for next compileSubroutine() function
	}
	compileSubroutine();	
	
}
void compileParameterList()
{
	//variables used for define function in symbol table module
	char nameOfVar[100];
	char typeOfVar[100];
	//kind kindOfVar;
	//make sure to reduce the indent before returning
	if(!hasMoreTokens()) 
	{
		printf("expected return 'type' or symbol ')' for paramenter list at line %d\n", currentToken->line);
		freeToken();
		fclose(xmlFile);
		exit(1);
	}
	else
	{
		advance(); //get the next 'type' token or ')' symbol token
		if(tokenType() == KEYWORD) //type of int, char or boolean
		{
			switch(keyWord())
			{
				case INT:
					//fprintf(xmlFile, "%s<parameterList>\n", indentString);
					//strcat(indentString, "  ");//increase the indent
					//fprintf(xmlFile, "%s<keyword> int </keyword>\n", indentString);
					strcpy(typeOfVar, "int");
					break;
				case CHAR:
					//fprintf(xmlFile, "%s<parameterList>\n", indentString);
					//strcat(indentString, "  ");//increase the indent
					//fprintf(xmlFile, "%s<keyword> char </keyword>\n", indentString);
					strcpy(typeOfVar, "char");
					break;
				case BOOLEAN:
					//fprintf(xmlFile, "%s<parameterList>\n", indentString);
					//strcat(indentString, "  ");//increase the indent
					//fprintf(xmlFile, "%s<keyword> boolean </keyword>\n", indentString);
					strcpy(typeOfVar, "boolean");
					break;
				default: //not a valid keyword found in 'type' decleration
					printf("Parameter list declareation unknown 'type' at line %d\n", currentToken->line);
					freeToken();
					fclose(xmlFile);
					exit(1);
			}
		}
		else if(tokenType() == IDENTIFIER) //type of some class name 'identifer'
		{
			//fprintf(xmlFile, "%s<parameterList>\n", indentString);
			//strcat(indentString, "  ");//increase the indent
			//fprintf(xmlFile, "%s<identifier> %s </identifier>\n", indentString, identifier());
			strcpy(typeOfVar, identifier());
		}
		else if(tokenType() == SYMBOL && symbol() == ')') //we just found an empty parameter list
		{
			//indentString[strlen(indentString)-2] = '\0'; //decrease the indent
			//fprintf(xmlFile, "%s<parameterList>\n", indentString);
			//fprintf(xmlFile, "%s</parameterList>\n", indentString);
			return;
		}
		else  //not found legal keyword or identifier in 'type' decleration
		{
			printf("Variable declareation unknown type at line %d\n", currentToken->line);
			freeToken();
			fclose(xmlFile);
			exit(1);
		}
	}
	//variable name if block
	if(!hasMoreTokens()) 
	{
		printf("token variable name not found at line %d\n", currentToken->line);
		freeToken();
		fclose(xmlFile);
		exit(1);
	}
	else
	{
		advance(); //get next token for variable name
		if(tokenType() == IDENTIFIER)
		{
			//fprintf(xmlFile, "%s<identifier> %s </identifier>\n", indentString, identifier());
			strcpy(nameOfVar, identifier());
			define(nameOfVar, typeOfVar, ARG_SMBL);
			//numOfLocals++; //increment number of locals for a function
		}
		else //not a valid variable name token
		{
			printf("token variable not found at line %d\n", currentToken->line);
			freeToken();
			fclose(xmlFile);
			exit(1);
		}
	}
	//loop through multiple parameter list
	while(1)
	{
		if(!hasMoreTokens()) // check for more token of type ','
		{
			printf("Parameter decleration error at line %d\n", currentToken->line);
			freeToken();
			fclose(xmlFile);
			exit(1);
		}
		else
		{
			advance();
		}
		//clear previously set variables
		memset(nameOfVar, 0, 100);
		memset(typeOfVar, 0, 100);
		//check for symbol ','
		if(tokenType() == SYMBOL)
		{
			switch(symbol())
			{
				case ')':
					indentString[strlen(indentString)-2] = '\0'; //decrease the indent
					fprintf(xmlFile, "%s</parameterList>\n", indentString);
					return;
					//break;
				case ',':
					//fprintf(xmlFile, "%s<symbol> , </symbol>\n", indentString);
					break;
				default:
					printf("Parameter decleration error at line %d\n", currentToken->line);
					freeToken();
					fclose(xmlFile);
					exit(1);
			}
		}
		if(!hasMoreTokens()) // check for more token of type keyword int, char, boolean
		{
			printf("Parameter decleration error at line %d\n", currentToken->line);
			freeToken();
			//fclose(xmlFile);
			exit(1);
		}
		else
		{
			advance();
		}
		if(tokenType() == KEYWORD)
		{
			switch(keyWord())
			{
				case INT:
					//fprintf(xmlFile, "%s<keyword> int </keyword>\n", indentString);
					strcpy(typeOfVar, "int");
					break;
				case CHAR:
					//fprintf(xmlFile, "%s<keyword> char </keyword>\n", indentString);
					strcpy(typeOfVar, "char");
					break;
				case BOOLEAN:
					//fprintf(xmlFile, "%s<keyword> boolean </keyword>\n", indentString);
					strcpy(typeOfVar, "boolean");
					break;
				default: //not a valid keyword found in 'type' decleration
					printf("Parameter list declareation unknown 'type' at line %d\n", currentToken->line);
					freeToken();
					//fclose(xmlFile);
					exit(1);
			}
		}
		else if(tokenType() == IDENTIFIER)
		{
			//fprintf(xmlFile, "%s<identifier> %s </identifer>\n", indentString, identifier());
			strcpy(typeOfVar, identifier());
		}
		else
		{
			printf("Parameter list declareation unknown 'type' at line %d\n", currentToken->line);
			freeToken();
			//fclose(xmlFile);
			exit(1);
		}
		if(!hasMoreTokens()) // check for more token for identifier
		{
			printf("Parameter decleration error at line %d\n", currentToken->line);
			freeToken();
			fclose(xmlFile);
			exit(1);
		}
		else
		{
			advance();
		}
		
		if(tokenType() == IDENTIFIER)
		{
			//fprintf(xmlFile, "%s<identifier> %s </identifier>\n", indentString, identifier());
			strcpy(nameOfVar, identifier());
			define(nameOfVar, typeOfVar, ARG_SMBL);
			//numOfLocals++;
		}
		else //not a valid variable name token
		{
			printf("token variable not found at line %d\n", currentToken->line);
			freeToken();
			//fclose(xmlFile);
			exit(1);
		}
	}
	
}
void compileVarDec()
{
	//variables used for define function in symbol table module
	char nameOfVar[100];
	char typeOfVar[100];
	//check for var keyword code block
	if(!hasMoreTokens()) //if no more tokens return to compileClass method
	{			
		return;
	}
	else
	{
		advance(); //read next token
		if(tokenType() == KEYWORD)
		{
			switch(keyWord())
			{
				case VAR:
					//fprintf(xmlFile, "%s<varDec>\n", indentString);
					//strcat(indentString, "  "); //increase the indent
					//fprintf(xmlFile, "%s<keyword> var </keyword>\n", indentString);
					break;
				default:   //var keyword not found return to compileSubroutine Method
					return;
			}
		}
		else //if the token type in not keyword this may be possible some other statement
		{	  //return to compileSubroutine method	
			return;
		}
	}
	//we have come so far so it must be a VarDecleration
	//check for syntatic correctness and exit program if some errors are found
	//check for 'type' token code block
	if(!hasMoreTokens()) 
	{
		printf("token 'type' not found at line %d\n", currentToken->line);
		freeToken();
		fclose(xmlFile);
		exit(1);
	}
	else
	{
		advance(); //get the next 'type' token
		if(tokenType() == KEYWORD)
		{
			switch(keyWord())
			{
				case INT:
					//fprintf(xmlFile, "%s<keyword> int </keyword>\n", indentString);
					strcpy(typeOfVar, "int");
					break;
				case CHAR:
					//fprintf(xmlFile, "%s<keyword> char </keyword>\n", indentString);
					strcpy(typeOfVar, "char");
					break;
				case BOOLEAN:
					//fprintf(xmlFile, "%s<keyword> boolean </keyword>\n", indentString);
					strcpy(typeOfVar, "boolean");
					break;
				default: //not a valid keyword found in 'type' decleration
					printf("Variable declareation unknown 'type' at line %d\n", currentToken->line);
					freeToken();
					fclose(xmlFile);
					exit(1);
			}
		}
		else if(tokenType() == IDENTIFIER)
		{
			//fprintf(xmlFile, "%s<identifier> %s </identifier>\n", indentString, identifier());
			strcpy(typeOfVar, identifier());
		}
		else  //not found legal keyword or identifier in 'type' decleration
		{
			printf("Variable declareation unknown type at line %d\n", currentToken->line);
			freeToken();
			fclose(xmlFile);
			exit(1);
		}
	}
	//variable name if block
	if(!hasMoreTokens()) 
	{
		printf("token variable name not found at line %d\n", currentToken->line);
		freeToken();
		fclose(xmlFile);
		exit(1);
	}
	else
	{
		advance(); //get next token for variable name
		if(tokenType() == IDENTIFIER)
		{
			//fprintf(xmlFile, "%s<identifier> %s </identifier>\n", indentString, identifier());
			strcpy(nameOfVar, identifier());
			define(nameOfVar, typeOfVar, VAR_SMBL);
			numOfLocals++;
		}
		else //not a valid variable name token
		{
			printf("token variable not found at line %d\n", currentToken->line);
			freeToken();
			fclose(xmlFile);
			exit(1);
		}
	}
	//below block of code for occurance of code like in next line
	//(',' varName)* ';' block
	if(!hasMoreTokens())
	{
		printf("terminating symbol ';' or ',' not found at line %d\n", currentToken->line);
		freeToken();
		fclose(xmlFile);
		exit(1);
	}
	else
	{
		advance();
		if(tokenType() == SYMBOL)
		{
			while(1)
			{
				//set the previously set nameOfVar variable
				memset(nameOfVar, 0, 100);
				if(symbol() == ';')
				{
					//fprintf(xmlFile, "%s<symbol> ; </symbol>\n", indentString);
					//strncpy(indentString, indentString, strlen(indentString)-2);
					//indentString[strlen(indentString)-2] = '\0'; //decrease the indent
					//fprintf(xmlFile, "%s</varDec>\n", indentString);
					break;
				}
				else if(symbol() == ',')
				{
					//fprintf(xmlFile, "%s<symbol> , </symbol>\n", indentString);
					if(!hasMoreTokens()) //check for variable name token
					{
						printf("token variable name not found at line %d\n", currentToken->line);
						freeToken();
						fclose(xmlFile);
						exit(1);
					}
					advance(); //get the next variable name token 
					if(tokenType() == IDENTIFIER)
					{
						//fprintf(xmlFile, "%s<identifier> %s </identifier>\n", indentString, identifier());
						strcpy(nameOfVar, identifier());
						define(nameOfVar, typeOfVar, VAR_SMBL);
						numOfLocals++;
					}
					else //not a valid variable name
					{
						printf("token variable not found at line %d\n", currentToken->line);
						freeToken();
						fclose(xmlFile);
						exit(1);
					}	
				}
				if(!hasMoreTokens()) // check for more token of type ',' or ';'
				{
					printf("terminating symbol ';' or ',' not found at line %d\n", currentToken->line);
					freeToken();
					fclose(xmlFile);
					exit(1);
				}
				else
				{
					advance();
				}
			}
		}
		else
		{
			printf("terminating symbol ';' or ',' not found at line %d\n", currentToken->line);
			freeToken();
			fclose(xmlFile);
			exit(1);
		}
	}
	//at the end recussively call compileClassVarDec
	compileVarDec();
}
void compileStatements()
{	
	if(tokenType() == KEYWORD) //since next token has already been ready by compileVarDec
	{							//just need to check here for statement keywords
		switch(keyWord())
		{
			case LET:
				compileLet();
				break;
			case IF:
				compileIf();
				break;
			case WHILE:
				compileWhile();
				break;
			case DO:
				compileDo();
				break;
			case RETURN:
				compileReturn();
				break;
			default:
				return; //did not found any valid statement return as it be be some other statement
		}
	}
	else
	{
		return; //did not found any valid statement return as it be be some other statement
	}
	//read the next token
	if(!hasMoreTokens())
	{
		printf("expected symbol '}' for subroutine decleration\n");
		freeToken();
		fclose(xmlFile);
		exit(1);
	}
	else
	{
		advance(); //advance for next compileSubroutine() function
	}
	//call compileStatement recussively
	compileStatements();
}
void compileDo()
{
	//straight away write <doStatement> tag as it has been
	//ready by compileStatments function
	//fprintf(xmlFile, "%s<doStatement>\n", indentString);
	//strcat(indentString, "  "); //increase the indent
	//fprintf(xmlFile, "%s<keyword> do </keyword>\n", indentString);
	//read next token which should be identifier
	char functionCallName[200], objectName[100];
	char *objectClass;
	memset(functionCallName, 0, 200);
	memset(objectName, 0, 100);
	numOfParameter = 0;
	if(!hasMoreTokens())
	{
		printf("expected an identifier at line %d\n", currentToken->line);
		freeToken();
		fclose(xmlFile);
		exit(1);
	}
	else
	{
		advance(); 
		if(tokenType() != IDENTIFIER)
		{
			printf("expected an identifier at line %d\n", currentToken->line);
			freeToken();
			fclose(xmlFile);
			exit(1);
		}
		//fprintf(xmlFile, "%s<identifier> %s </identifier>\n", indentString, identifier());
		//get the type of for current identifier to check if its object of some class
		objectClass = typeOf(identifier());
		//printf("objectClass %s\n", objectClass);
		if(objectClass)//identifier is a name of object so its a method call
		{
			strcpy(functionCallName, objectClass);
			strcpy(objectName, identifier());
		}
		else //its a call to some class function or constructor
		{
			strcpy(functionCallName, identifier());
		}
		
	}
	//read next token as symbol '(' or '.'
	if(!hasMoreTokens())
	{
		printf("expected a symbol '(' or '.' at line %d\n", currentToken->line);
		freeToken();
		fclose(xmlFile);
		exit(1);
	}
	else
	{
		advance();
		if(tokenType() != SYMBOL)
		{
			printf("expected a symbol '(' or '.' at line %d\n", currentToken->line);
			freeToken();
			fclose(xmlFile);
			exit(1);
		}
		if(symbol() == '.') //found '.' symbol
		{
			//fprintf(xmlFile, "%s<symbol> . </symbol>\n", indentString);
			strcat(functionCallName, ".");
			//read next token should be a identifier
			if(!hasMoreTokens())
			{
				printf("expected a identifier at line %d\n", currentToken->line);
				freeToken();
				fclose(xmlFile);
				exit(1);
			}
			else
			{
				advance();
				if(tokenType() != IDENTIFIER)
				{
					printf("expected an identifier at line %d\n", currentToken->line);
					freeToken();
					fclose(xmlFile);
					exit(1);
				}
				//fprintf(xmlFile, "%s<identifier> %s </identifier>\n", indentString, identifier());
				strcat(functionCallName, identifier());
			}
			//read next token and should be a '(' symbol
			//but it will be processed in next if block
			if(!hasMoreTokens())
			{
				printf("expected a symbol '(' at line %d\n", currentToken->line);
				freeToken();
				fclose(xmlFile);
				exit(1);
			}
			else
			{
				advance();
				if(tokenType() != SYMBOL)
				{
					printf("expected a symbol '(' at line %d\n", currentToken->line);
					freeToken();
					fclose(xmlFile);
					exit(1);
				}
			}				
		}
		if(symbol() == '(') //need to check this block of code <--------------
		{
			//fprintf(xmlFile, "%s<symbol> ( </symbol>\n", indentString);
		}
		else //use if(symbol != '(') to finish else block
		{
			printf("expected a symbol '(' or '.' at line %d\n", currentToken->line);
			freeToken();
			fclose(xmlFile);
			exit(1);
		}
	}
	//if do call involves call to object's method push that object before
	//compiling expression list
	if(objectClass) //its a call to objects method
	{
		numOfParameter++; //increment number of paramenter for pushing object
		//get the memory segment and index for object
		switch(kindOf(objectName))
		{
			case STATIC_SMBL: 
				writePush(STATIC_SEG, indexOf(objectName));
				break;
			case FIELD_SMBL:
				writePush(THIS_SEG, indexOf(objectName));
				break;
			case ARG_SMBL:
				writePush(ARG_SEG, indexOf(objectName));
				break;
			case VAR_SMBL:
				writePush(LOCAL_SEG, indexOf(objectName));
				break;
			case NONE: //we should not land into this code by just in case
				break;
		}
	}
	compileExpressionList();
	//since next token has been read by compileExpressionList
	//next token should be ')'
	if(tokenType() != SYMBOL || symbol() != ')')
	{
		//printf("Token: %s\n", currentToken->stringToken);
		printf("expected a symbol ')' at line %d\n", currentToken->line);
		freeToken();
		fclose(xmlFile);
		exit(1);
	}
	//fprintf(xmlFile, "%s<symbol> ) </symbol>\n", indentString);
	//subroutineCall Ends here
	//read next token should be a symbol ';'
	if(!hasMoreTokens())
	{
		printf("expected a symbol ';' at line %d\n", currentToken->line);
		freeToken();
		fclose(xmlFile);
		exit(1);
	}
	else
	{
		advance(); 
		if(tokenType() != SYMBOL || symbol() != ';')
		{
			printf("expected a symbol ';' at line %d\n", currentToken->line);
			freeToken();
			fclose(xmlFile);
			exit(1);
		}
		//fprintf(xmlFile, "%s<symbol> ; </symbol>\n", indentString);
	}
	//indentString[strlen(indentString)-2] = '\0'; //decrease the indent
	//fprintf(xmlFile, "%s</doStatement>\n", indentString);
	writeCall(functionCallName, numOfParameter);
	//discard the return value on stack by previous function call
	writePop(TEMP_SEG, 0);	
	//free objectClass buffer here
	free(objectClass);	
}
void compileLet()
{
	//straight away write <letStatement> tag as it has been
	//ready by compileStatments function
	char vName[100]; //name of variable
	memset(vName, 0, 100); //reset varName buffer
	//fprintf(xmlFile, "%s<letStatement>\n", indentString);
	//strcat(indentString, "  "); //increase the indent
	//fprintf(xmlFile, "%s<keyword> let </keyword>\n", indentString);
	//read next token which should be identifier
	if(!hasMoreTokens())
	{
		printf("expected an identifier at line %d\n", currentToken->line);
		freeToken();
		fclose(xmlFile);
		exit(1);
	}
	else
	{
		advance(); 
		if(tokenType() != IDENTIFIER)
		{
			printf("expected an identifier at line %d\n", currentToken->line);
			freeToken();
			fclose(xmlFile);
			exit(1);
		}
		//fprintf(xmlFile, "%s<identifier> %s </identifier>\n", indentString, identifier());
		strcpy(vName, identifier()); //copy identifer for later use
	}
	//read next token as symbol '[' or '='
	if(!hasMoreTokens())
	{
		printf("expected a symbol '[' or '=' at line %d\n", currentToken->line);
		freeToken();
		fclose(xmlFile);
		exit(1);
	}
	else
	{
		advance();
		if(tokenType() != SYMBOL)
		{
			printf("expected a symbol '[' or '=' at line %d\n", currentToken->line);
			freeToken();
			fclose(xmlFile);
			exit(1);
		}
		//code block for '[expression]'
		if(symbol() == '[')
		{
			//fprintf(xmlFile, "%s<symbol> [ </symbol>\n", indentString);
			compileExpression();
			//since next token has been ready by compileExpression()
			if(tokenType() != SYMBOL || symbol() != ']')
			{
				printf("expected a symbol ']' at line %d\n", currentToken->line);
				freeToken();
				fclose(xmlFile);
				exit(1);
			}
			//fprintf(xmlFile, "%s<symbol> ] </symbol>\n", indentString);
			//read the next token '=' for next if block
			if(!hasMoreTokens())
			{
				printf("expected a symbol '=' at line %d\n", currentToken->line);
				freeToken();
				fclose(xmlFile);
				exit(1);
			}
			else
			{
				advance();
			}				
		}
		//code block for symbol '='
		if(symbol() == '=')
		{
			//fprintf(xmlFile, "%s<symbol> = </symbol>\n", indentString);
		}
		else
		{
			printf("expected a symbol '=' at line %d\n", currentToken->line);
			freeToken();
			fclose(xmlFile);
			exit(1);
		}
	}
	//compile expressions
	compileExpression();
	//since next token has been read by compileExpression
	if(tokenType() != SYMBOL || symbol() != ';')
	{
		printf("Token: %s\n", currentToken->stringToken);
		printf("expected a symbol ';' at line %d\n", currentToken->line);
		freeToken();
		fclose(xmlFile);
		exit(1);
	}
	//this logic might have to be moved to other place in this function
	switch(kindOf(vName))
	{
		case STATIC_SMBL:
			writePop(STATIC_SEG, indexOf(vName));
			break;
		case FIELD_SMBL:
			writePop(THIS_SEG, indexOf(vName)); //need modification here
			break;
		case ARG_SMBL:
			writePop(ARG_SEG, indexOf(vName));
			break;
		case VAR_SMBL:
			writePop(LOCAL_SEG, indexOf(vName));
			break;
	}
	//fprintf(xmlFile, "%s<symbol> ; </symbol>\n", indentString);
	//indentString[strlen(indentString)-2] = '\0'; //decrease the indent
	//fprintf(xmlFile, "%s</letStatement>\n", indentString);		
}

void compileWhile()
{
	//straight away write <whileStatement> tag as it has been
	//ready by compileStatments function
	//fprintf(xmlFile, "%s<whileStatement>\n", indentString);
	//strcat(indentString, "  "); //increase the indent
	//fprintf(xmlFile, "%s<keyword> while </keyword>\n", indentString);
	//read next token should be a symbol '('
	int localIndex = loopIfIndex++;
	char loopLabel[100];
	memset(loopLabel, 0, 100);
	sprintf(loopLabel, "WHILE_START_%d", localIndex);
	writeLabel(loopLabel);
	if(!hasMoreTokens())
	{
		printf("expected an symbol '(' at line %d\n", currentToken->line);
		freeToken();
		fclose(xmlFile);
		exit(1);
	}
	else
	{
		advance(); 
		if(tokenType() != SYMBOL || symbol() != '(')
		{
			printf("expected an symbol '(' at line %d\n", currentToken->line);
			freeToken();
			fclose(xmlFile);
			exit(1);
		}
		//fprintf(xmlFile, "%s<symbol> ( </symbol>\n", indentString);
	}
	//compile expression then
	compileExpression();
	//write a not statement and jump command to end of loop
	writeArithmetic(NOT);
	memset(loopLabel, 0, 100);
	sprintf(loopLabel, "WHILE_END_%d", localIndex);
	writeIf(loopLabel);
	//since next token has been read by compileExpression
	//and should be a symbol ')'
	if(tokenType() != SYMBOL || symbol() != ')')
	{
		printf("expected a symbol ')' at line %d\n", currentToken->line);
		freeToken();
		fclose(xmlFile);
		exit(1);
	}
	//fprintf(xmlFile, "%s<symbol> ) </symbol>\n", indentString);
	//read next token should be a symbol '{'
	if(!hasMoreTokens())
	{
		printf("expected an symbol '{' at line %d\n", currentToken->line);
		freeToken();
		fclose(xmlFile);
		exit(1);
	}
	else
	{
		advance(); 
		if(tokenType() != SYMBOL || symbol() != '{')
		{
			printf("expected an symbol '{' at line %d\n", currentToken->line);
			freeToken();
			fclose(xmlFile);
			exit(1);
		}
		//fprintf(xmlFile, "%s<symbol> { </symbol>\n", indentString);
	}
	//read next token and
	//then compile statements in a while loop
	if(!hasMoreTokens())
	{
		printf("expected a statement or symbol '}' at line %d\n", currentToken->line);
		freeToken();
		fclose(xmlFile);
		exit(1);
	}
	else
	{
		advance(); 
	}
	//fprintf(xmlFile, "%s<statements>\n", indentString);
	//strcat(indentString, "  ");//increase the indent
	compileStatements();
	//indentString[strlen(indentString)-2] = '\0'; //decrease the indent
	//fprintf(xmlFile, "%s</statements>\n", indentString);
	//since the next tokens as already ready by compileStatement()
	//just check for symbol ;}'
	if(tokenType() != SYMBOL || symbol() != '}')
	{
		printf("expected an symbol '}' at line %d\n", currentToken->line);
		freeToken();
		fclose(xmlFile);
		exit(1);
	}
	memset(loopLabel, 0, 100);
	sprintf(loopLabel, "WHILE_START_%d", localIndex);
	writeGoto(loopLabel);
	memset(loopLabel, 0, 100);
	sprintf(loopLabel, "WHILE_END_%d", localIndex);
	writeLabel(loopLabel);
	//fprintf(xmlFile, "%s<symbol> } </symbol>\n", indentString);
	//indentString[strlen(indentString)-2] = '\0'; //decrease the indent
	//fprintf(xmlFile, "%s</whileStatement>\n", indentString);
}
void compileReturn()
{
	//straight away write <ReturnStatement> tag as it has been
	//ready by compileStatments function
	//fprintf(xmlFile, "%s<returnStatement>\n", indentString);
	//strcat(indentString, "  "); //increase the indent
	//fprintf(xmlFile, "%s<keyword> return </keyword>\n", indentString);
	//straight way call compileExpression
	compileExpression();
	//since next token has been read by compileExpression
	if(tokenType() != SYMBOL || symbol() != ';')
	{
		printf("expected a symbol ';' at line %d\n", currentToken->line);
		freeToken();
		fclose(xmlFile);
		exit(1);
	}
	//push constant 0 if return type is of type void
	if(isVoid)
	{
		writePush(CONST_SEG, 0);
	}
	writeReturn();
	//fprintf(xmlFile, "%s<symbol> ; </symbol>\n", indentString);
	//indentString[strlen(indentString)-2] = '\0'; //decrease the indent
	//fprintf(xmlFile, "%s</returnStatement>\n", indentString);
}
void compileIf()
{
	//straight away write <ifStatement> tag as it has been
	//ready by compileStatments function
	//struct fileToken *previousToken = 0;
	int localIndex = loopIfIndex++;
	char ifLabel[100];
	memset(ifLabel, 0, 100);
	//fprintf(xmlFile, "%s<ifStatement>\n", indentString);
	//strcat(indentString, "  "); //increase the indent
	//fprintf(xmlFile, "%s<keyword> if </keyword>\n", indentString);
	//read next token should be a symbol '('
	if(!hasMoreTokens())
	{
		printf("expected an symbol '(' at line %d\n", currentToken->line);
		freeToken();
		fclose(xmlFile);
		exit(1);
	}
	else
	{
		advance(); 
		if(tokenType() != SYMBOL || symbol() != '(')
		{
			printf("expected an symbol '(' at line %d\n", currentToken->line);
			freeToken();
			fclose(xmlFile);
			exit(1);
		}
		//fprintf(xmlFile, "%s<symbol> ( </symbol>\n", indentString);
	}
	//compile expression then
	compileExpression();
	sprintf(ifLabel, "IF_TRUE_%d", localIndex);
	writeIf(ifLabel);
	memset(ifLabel, 0, 100);
	sprintf(ifLabel, "IF_FALSE_%d", localIndex);
	writeGoto(ifLabel);
	//since next token has been read by compileExpression
	//and should be a symbol ')'
	if(tokenType() != SYMBOL || symbol() != ')')
	{
		printf("expected a symbol ')' at line %d\n", currentToken->line);
		freeToken();
		fclose(xmlFile);
		exit(1);
	}
	//fprintf(xmlFile, "%s<symbol> ) </symbol>\n", indentString);
	//read next token should be a symbol '{'
	if(!hasMoreTokens())
	{
		printf("expected an symbol '{' at line %d\n", currentToken->line);
		freeToken();
		fclose(xmlFile);
		exit(1);
	}
	else
	{
		advance(); 
		if(tokenType() != SYMBOL || symbol() != '{')
		{
			printf("expected an symbol '{' at line %d\n", currentToken->line);
			freeToken();
			fclose(xmlFile);
			exit(1);
		}
		//fprintf(xmlFile, "%s<symbol> { </symbol>\n", indentString);
	}
	//read next token and
	//then compile statements
	if(!hasMoreTokens())
	{
		printf("expected a statement or symbol '}' at line %d\n", currentToken->line);
		freeToken();
		fclose(xmlFile);
		exit(1);
	}
	else
	{
		advance(); 
	}
	memset(ifLabel, 0, 100);
	sprintf(ifLabel, "IF_TRUE_%d", localIndex);
	writeLabel(ifLabel);
	//fprintf(xmlFile, "%s<statements>\n", indentString);
	//strcat(indentString, "  ");//increase the indent
	compileStatements();
	//indentString[strlen(indentString)-2] = '\0'; //decrease the indent
	//fprintf(xmlFile, "%s</statements>\n", indentString);
	//since the next tokens as already ready by compileStatement()
	//just check for symbol }'
	if(tokenType() != SYMBOL || symbol() != '}')
	{
		printf("expected an symbol '}' at line %d\n", currentToken->line);
		freeToken();
		fclose(xmlFile);
		exit(1);
	}
	//fprintf(xmlFile, "%s<symbol> } </symbol>\n", indentString);
	
	//look for else block
	//previousToken = currentToken;
	if(!hasMoreTokens())
	{
		printf("expected an symbol '}' or a statement at line %d\n", currentToken->line);
		freeToken();
		fclose(xmlFile);
		exit(1);
	}
	else
	{
		advance(); 
		//else keyword not found return
		//but we have also drive past on token also here try to fix it here
		if(tokenType() != KEYWORD || keyWord() != ELSE) 
		{
			//printf("Else not found\n");
			//printf("currentToken->str: %s, previousToken->str: %s\n", currentToken->stringToken, previousToken->stringToken);
			//currentToken = previousToken;
			//current->next = current;
			current = currentToken;
		//	indentString[strlen(indentString)-2] = '\0'; //decrease the indent
		//	fprintf(xmlFile, "%s</ifStatement>\n", indentString);
			memset(ifLabel, 0, 100);
			sprintf(ifLabel, "IF_FALSE_%d", localIndex);
			writeLabel(ifLabel);
			return;												
		}
	}
	//fprintf(xmlFile, "%s<keyword> else </keyword>\n", indentString);
	//read next token should be a symbol '{'
	memset(ifLabel, 0, 100);
	sprintf(ifLabel, "IF_END_%d", localIndex);
	writeGoto(ifLabel);
	memset(ifLabel, 0, 100);
	sprintf(ifLabel, "IF_FALSE_%d", localIndex);
	writeLabel(ifLabel);
	if(!hasMoreTokens())
	{
		printf("expected an symbol '{' at line %d\n", currentToken->line);
		freeToken();
		fclose(xmlFile);
		exit(1);
	}
	else
	{
		advance(); 
		if(tokenType() != SYMBOL || symbol() != '{')
		{
			printf("expected an symbol '{' at line %d\n", currentToken->line);
			freeToken();
			fclose(xmlFile);
			exit(1);
		}
		//fprintf(xmlFile, "%s<symbol> { </symbol>\n", indentString);
	}
	//read next token and
	//then compile statements
	if(!hasMoreTokens())
	{
		printf("expected a statement or symbol '}' at line %d\n", currentToken->line);
		freeToken();
		fclose(xmlFile);
		exit(1);
	}
	else
	{
		advance(); 
	}
	//fprintf(xmlFile, "%s<statements>\n", indentString);
	//strcat(indentString, "  ");//increase the indent
	compileStatements();
	//indentString[strlen(indentString)-2] = '\0'; //decrease the indent
	//fprintf(xmlFile, "%s</statements>\n", indentString);
	//since the next tokens as already ready by compileStatement()
	//just check for symbol }'
	if(tokenType() != SYMBOL || symbol() != '}')
	{
		printf("expected an symbol '}' at line %d\n", currentToken->line);
		freeToken();
		fclose(xmlFile);
		exit(1);
	}
	memset(ifLabel, 0, 100);
	sprintf(ifLabel, "IF_END_%d", localIndex);
	writeLabel(ifLabel);
	//fprintf(xmlFile, "%s<symbol> } </symbol>\n", indentString);
	//indentString[strlen(indentString)-2] = '\0'; //decrease the indent
	//fprintf(xmlFile, "%s</ifStatement>\n", indentString);
}
void compileExpression()
{
	command commType;
	//variable to check if multiply = 1 or divide = 2 command 
	int mulOrDivide = 0; 
	if(!hasMoreTokens())
	{
		printf("expression error at line %d\n", currentToken->line);
		freeToken();
		fclose(xmlFile);
		exit(1);
	}
	else
	{
		advance();
		//this logic to accomadate expressionList & return statement
		//and term '(' expression ')'
		/*if(tokenType() == SYMBOL && symbol() == ')')
		{
			return;
		}*/
		if(tokenType() == SYMBOL)
		{
			switch(symbol())
			{
				case ')':
					/* This block of code is specific to compileExpressionList
					 * and will be only executed if expressionList is empty
					 */
					numOfParameter--;
					return;
				case ';':
					return;
				default:
					break;
			}
		}
		//fprintf(xmlFile, "%s<expression>\n", indentString);
		//strcat(indentString, "  "); //increase the indent
		//fprintf(xmlFile, "%s<term>\n", indentString);
	}
	//printf("In compileExpression function, before 1st compileTerm\n");
	compileTerm();
	//printf("In compileExpression function, after 1st compileTerm\n");
	//fprintf(xmlFile, "%s</term>\n", indentString);
	//indentString[strlen(indentString)-2] = '\0'; //decrease the indent
	//assuming that advance has already been called by compileTerm();
	//look for the occurance of (op term)*
	while(1)
	{
		if(tokenType()!= SYMBOL)
		{
			//next token is not of type 'op' so expression has ended and return;
			//this is the only place from where we will exit the function
			//so put ending tags here
			//indentString[strlen(indentString)-2] = '\0'; //decrease the indent
			//fprintf(xmlFile, "%s</expression>\n", indentString);
			//numOfParameter++;
			return;
		}
		switch(symbol())
		{
			case '+':
				//fprintf(xmlFile, "%s<symbol> + </symbol>\n", indentString);
				commType = ADD;
				break;
			case '-':
				//fprintf(xmlFile, "%s<symbol> - </symbol>\n", indentString);
				commType = SUB;
				break;
			case '*':
				//fprintf(xmlFile, "%s<symbol> * </symbol>\n", indentString);
				mulOrDivide = 1;
				break;
			case '/':
				//fprintf(xmlFile, "%s<symbol> / </symbol>\n", indentString);
				mulOrDivide = 2;
				break;
			case '&'://use &amp;
				//fprintf(xmlFile, "%s<symbol> &amp; </symbol>\n", indentString);
				commType = AND;
				break;
			case '|':
				//fprintf(xmlFile, "%s<symbol> | </symbol>\n", indentString);
				commType = OR;
				break;
			case '<': //use &lt;
				//fprintf(xmlFile, "%s<symbol> &lt; </symbol>\n", indentString);
				commType = LT;
				break;
			case '>':
				//fprintf(xmlFile, "%s<symbol> &gt; </symbol>\n", indentString);
				commType = GT;
				break;//use &gt;
			case '=':
				//fprintf(xmlFile, "%s<symbol> = </symbol>\n", indentString);
				commType = EQ;
				break;	
			
			default:
				//indentString[strlen(indentString)-2] = '\0'; //decrease the indent
				//fprintf(xmlFile, "%s</expression>\n", indentString);
				//numOfParameter++;
				return;
				//printf("unknown 'op' type at line %d\n", currentToken->line);
				//freeToken();
				//fclose(xmlFile);
				//exit(1);
		}
		if(!hasMoreTokens())
		{
			printf("expression error at line %d\n", currentToken->line);
			freeToken();
			fclose(xmlFile);
			exit(1);
		}
		else
		{
			advance();
			//strcat(indentString, "  "); //increase the indent
			//fprintf(xmlFile, "%s<term>\n", indentString);
			//printf("In compileExpression function, before 2nd compileTerm\n");
			compileTerm();
			//printf("In compileExpression function, after 2nd compileTerm\n");
			//indentString[strlen(indentString)-2] = '\0'; //decrease the indent
			//fprintf(xmlFile, "%s</term>\n", indentString);	
		}
		//write 'op' command to vm file
		if(mulOrDivide == 1) // write math.multiply function
		{
			writeCall("Math.multiply", 2); 
		}
		else if(mulOrDivide ==2) //write math.divide function
		{
			writeCall("Math.divide", 2);
		}
		else
		{
			switch(commType)
			{
				case ADD:
					writeArithmetic(ADD);
					break;
				case SUB:
					writeArithmetic(SUB);
					break;
				case AND:
					writeArithmetic(AND);
					break;
				case OR:
					writeArithmetic(OR);
					break;
				case LT:
					writeArithmetic(LT);
					break;
				case GT:
					writeArithmetic(GT);
					break;
				case EQ:
					writeArithmetic(EQ);
					break;
			}
		}
		
	}
	//advance(); //dangerous but just the temp stuff
}
void compileTerm()
{
	char functionCallName[200], objectName[100];
	char *objectClass;
	memset(functionCallName, 0, 200);
	memset(objectName, 0, 100);
	
	//strcat(indentString, "  "); //increase the indent
	if(tokenType() == INT_CONST) //int constant
	{
		writePush(CONST_SEG, intVal());
		//fprintf(xmlFile, "%s<integerConstant> %d </integerConstant>\n", indentString, intVal());
	}
	else if(tokenType() == STRING_CONST) //string constant
	{
		//fprintf(xmlFile, "%s<stringConstant> %s </stringConstant>\n", indentString, stringVal());
	}
	else if(tokenType() == KEYWORD) //keyword constant
	{
		switch(keyWord())
		{
			case TRUE:
				//fprintf(xmlFile, "%s<keyword> true </keyword>\n", indentString);
				writePush(CONST_SEG, 0);
				writeArithmetic(NOT);
				break;
			case FALSE:
				//fprintf(xmlFile, "%s<keyword> false </keyword>\n", indentString);
				writePush(CONST_SEG, 0);
				break;
			case NULL_KEYWORD:
				//fprintf(xmlFile, "%s<keyword> null </keyword>\n", indentString);
				break;
			case THIS:
				//fprintf(xmlFile, "%s<keyword> this </keyword>\n", indentString);
				writePush(POINTER_SEG, 0);
				break;
			default:
				printf("unknown keyword constant for term decleration at line %d\n", currentToken->line);
				freeToken();
				fclose(xmlFile);
				exit(1);
		}
	}
	else if(tokenType() == IDENTIFIER) //it may be a varName | varName'['expression']' | subroutineCall
	{
		//numOfParameter = 0;
		//fprintf(xmlFile, "%s<identifier> %s </identifier>\n", indentString, identifier());
		strcpy(functionCallName, identifier());
		if(!hasMoreTokens())
		{
			printf("expected ';' after term identifier at line %d\n", currentToken->line);
			freeToken();
			fclose(xmlFile);
			exit(1);
		}
		else
		{
			advance();
		}
		if(tokenType() == SYMBOL)
		{
			if(symbol() == '[') //process varName '['expression']'
			{
				//fprintf(xmlFile, "%s<symbol> [ </symbol>\n", indentString);
				compileExpression();
				//since the next token must have been read by compileExpression() just check for ']' character
				if(tokenType() != SYMBOL || symbol() != ']')
				{
					printf("Current token: %s\n", currentToken->stringToken);
					printf("expected a symbol ']' at line %d\n", currentToken->line);
					freeToken();
					fclose(xmlFile);
					exit(1);
				}
				//fprintf(xmlFile, "%s<symbol> ] </symbol>\n", indentString);
			}
			else if(symbol() == '(') //process subroutineName'(' expressionList ')'
			{
				//fprintf(xmlFile, "%s<symbol> ( </symbol>\n", indentString);
				numOfParameter = 0;
				compileExpressionList();
				//since the next token must have been read by compileExpressionList() just check for ')' character
				if(tokenType() != SYMBOL || symbol() != ')')
				{
					printf("Current token: %s\n", currentToken->stringToken);
					printf("expected a symbol ')' at line %d\n", currentToken->line);
					freeToken();
					fclose(xmlFile);
					exit(1);
				}
				//fprintf(xmlFile, "%s<symbol> ) </symbol>\n", indentString);
				writeCall(functionCallName, numOfParameter);
			}
			else if(symbol() == '.') //process className|varName.subroutineName '(' expressionList ')'
			{
				//fprintf(xmlFile, "%s<symbol> . </symbol>\n", indentString);
				//get the type of from buffer functionCallName to check if its object of some class
				numOfParameter = 0;
				objectClass = typeOf(functionCallName);
				//printf("objectClass %s\n", objectClass);
				if(objectClass)//identifier is a name of object so its a method call
				{
					strcpy(objectName, functionCallName);
					memset(functionCallName, 0, 200);
					strcpy(functionCallName, objectClass);
					//strcat(functionCallName, objectClass);
					
				}
				/*else //its a call to some class function or constructor
				{
					strcat(functionCallName, identifier());
				}*/
				strcat(functionCallName, ".");
				if(!hasMoreTokens()) //read subRoutineName
				{
					printf("expected identifier for subroutine name at line %d\n", currentToken->line);
					freeToken();
					fclose(xmlFile);
					exit(1);
				}
				else
				{
					advance();
				}
				if(tokenType() != IDENTIFIER)
				{
					printf("expected identifier for subroutine name at line %d\n", currentToken->line);
					freeToken();
					fclose(xmlFile);
					exit(1);
				}
				//fprintf(xmlFile, "%s<identifier> %s </identifier>\n", indentString, identifier());
				strcat(functionCallName, identifier());
				//read symbol '('
				if(!hasMoreTokens())
				{
					printf("expected a symbol '(' at line %d\n", currentToken->line);
					freeToken();
					fclose(xmlFile);
					exit(1);
				}
				else
				{
					advance();
				}
				if(tokenType() != SYMBOL || symbol() != '(')
				{
					printf("expected a symbol '(' at line %d\n", currentToken->line);
					freeToken();
					fclose(xmlFile);
					exit(1);
				}
				//fprintf(xmlFile, "%s<symbol> ( </symbol>\n", indentString);
				if(objectClass) //its a call to objects method
				{
					numOfParameter++; //increment number of paramenter for pushing object
					//get the memory segment and index for object
					switch(kindOf(objectName))
					{
						case STATIC_SMBL: //dont have logic what to put here
							break;
						case FIELD_SMBL:
							writePush(THIS_SEG, indexOf(objectName));
							break;
						case ARG_SMBL:
							break;
						case VAR_SMBL:
							writePush(LOCAL_SEG, indexOf(objectName));
							break;
						case NONE: //we should not land into this code by just in case
						break;
					}
				}
				compileExpressionList();
				//since the next token must have been read by compileExpressionList() just check for ')' character
				if(tokenType() != SYMBOL || symbol() != ')')
				{
					printf("Current token: %s\n", currentToken->stringToken);
					printf("expected a symbol ')' at line %d\n", currentToken->line);
					freeToken();
					fclose(xmlFile);
					exit(1);
				}
				//fprintf(xmlFile, "%s<symbol> ) </symbol>\n", indentString);
				writeCall(functionCallName, numOfParameter);
				free(objectClass);
				
			}
			else //it may be a operator or ';' symbol just return
			{
				
				switch(kindOf(functionCallName))
				{
					case STATIC_SMBL:
						writePush(STATIC_SEG, indexOf(functionCallName));
						break;
					case FIELD_SMBL:
						writePush(THIS_SEG, indexOf(functionCallName));
						break;
					case ARG_SMBL:
						writePush(ARG_SEG, indexOf(functionCallName));
						break;
					case VAR_SMBL:
						writePush(LOCAL_SEG, indexOf(functionCallName));
						break;
				}
				//indentString[strlen(indentString)-2] = '\0'; //decrease the indent
				return;
			}
		}
	}
	else if(tokenType() == SYMBOL)// && symbol() == '(') //'(' expression ')'
	{
		switch(symbol())
		{
			case '(':
				//fprintf(xmlFile, "%s<symbol> ( </symbol>\n", indentString);
				compileExpression();
				//since the next token must have been read by compileExpression() just check for ')' character
				if(tokenType() != SYMBOL || symbol() != ')')
				{
					printf("Current token: %s\n", currentToken->stringToken);
					printf("expected a symbol ')' at line %d\n", currentToken->line);
					freeToken();
					fclose(xmlFile);
					exit(1);
				}
				//fprintf(xmlFile, "%s<symbol> ) </symbol>\n", indentString);
				break;
			case '~':
				//fprintf(xmlFile, "%s<symbol> ~ </symbol>\n", indentString);
				if(!hasMoreTokens()) //advance for compileTerm
				{
					printf("expected a term at line %d\n", currentToken->line);
					freeToken();
					fclose(xmlFile);
					exit(1);
				}
				else
				{
					advance();
				}
				//strcat(indentString, "  "); //increase the indent
				//fprintf(xmlFile, "%s<term>\n", indentString);
				compileTerm();
				writeArithmetic(NOT);
				//fprintf(xmlFile, "%s</term>\n", indentString);
				//indentString[strlen(indentString)-2] = '\0'; //decrease the indent
				return;
			case '-':
				//fprintf(xmlFile, "%s<symbol> - </symbol>\n", indentString);
				if(!hasMoreTokens()) //advance for compileTerm
				{
					printf("expected a term at line %d\n", currentToken->line);
					freeToken();
					fclose(xmlFile);
					exit(1);
				}
				else
				{
					advance();
				}
				//strcat(indentString, "  "); //increase the indent
				//fprintf(xmlFile, "%s<term>\n", indentString);
				compileTerm();
				writeArithmetic(NEG);
				//fprintf(xmlFile, "%s</term>\n", indentString);
				//indentString[strlen(indentString)-2] = '\0'; //decrease the indent
				return;
			default:
				printf("unknown term format at line %d\n", currentToken->line);
				printf("Symbol: %c\n", symbol());
				freeToken();
				fclose(xmlFile);
				exit(1);
		}
	}
	if(!hasMoreTokens()) //advance for compileTerm
	{
		printf("term advance error at line %d\n", currentToken->line);
		freeToken();
		fclose(xmlFile);
		exit(1);
	}
	else
	{
		advance();
	}
	//advance();//temp stuff
	//indentString[strlen(indentString)-2] = '\0'; //decrease the indent
}
void compileExpressionList()
{
	//numOfParameter = 0;
	//fprintf(xmlFile, "%s<expressionList>\n", indentString);
	//strcat(indentString, "  "); //increase the indent
	/*if(tokenType() == SYMBOL && symbol() == ')')
	{
		indentString[strlen(indentString)-2] = '\0'; //decrease the indent
		fprintf(xmlFile, "%s</expressionList>\n", indentString);
		return;
	}*/
	/* Below is a desi jugad for incrementing number of numOfParameters
	* as some of the logic of empty expressionList lies in functoin
	* compileExpression
	*/
	numOfParameter++;
	compileExpression();
	//numOfParameter++;
	while(1)
	{
		if(tokenType() != SYMBOL || symbol() != ',')
		{
			//indentString[strlen(indentString)-2] = '\0'; //decrease the indent
			//fprintf(xmlFile, "%s</expressionList>\n", indentString);
			return;
		}
		//fprintf(xmlFile, "%s<symbol> , </symbol>\n", indentString);
		/* Below is a desi jugad for incrementing number of numOfParameters
		 * as some of the logic of empty expressionList lies in functoin
		 * compileExpression
		 */
		numOfParameter++;
		compileExpression();
		//numOfParameter++;
	}
}
