/* compilationengine.c
 * This file implements CompilationEngine Module of Jack Compiler
 * for Hack computer
 * Author: Dhruv Mohindru
 * Date: 23/03/2016
 */
#include "compilationengine.h"
void constructorCompilationEngine()
{
	//reset className, functionName buffer
	memset(className, 0, 100); 
	memset(functionName, 0, 100);
	numOfParameter = 0;
	numOfLocals = 0;
	isVoid = 0; //return type of function 0 = not void, 1 = void
	loopIfIndex = 0; //index for unique while or if command label
	functionType = 0; //if subroutine is constructor 0 = Not, 1 = Is Constuctor
}
void compileClass()
{
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
	}
	compileClassVarDec(); //compile class variable decleration
	//we should have now class symbol table ready so display it
	//printf("-------------------------------------------------\n");
	//printf("Displaying Class symbol table\n");
	//displaySymbolTable(0);
	//printf("-------------------------------------------------\n");
	/////////////////////////////
	compileSubroutine(); //compile class subroutines
	
	//token as already been read by previous function just check for 
	//syntatic correctness
	if(tokenType() != SYMBOL || symbol() != '}')
	{
		printf("Could not find symbol '}' for class decleration\n");
		return;
	}
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
					kindOfVar = STATIC_SMBL;
					break;
				case FIELD:
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
					strcpy(typeOfVar, "int");
					break;
				case CHAR:
					strcpy(typeOfVar, "char");
					break;
				case BOOLEAN:
					strcpy(typeOfVar, "boolean");
					break;
				default: //not a valid keyword found in 'type' decleration
					printf("Variable declareation unknown 'type' at line %d\n", currentToken->line);
					freeToken();
					
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
			
			exit(1);
		}
	}
	//variable name if block
	if(!hasMoreTokens()) 
	{
		printf("token variable name not found at line %d\n", currentToken->line);
		freeToken();
		
		exit(1);
	}
	else
	{
		advance(); //get next token for variable name
		if(tokenType() == IDENTIFIER)
		{
			//fprintf(xmlFile, "%s<identifier> %s </identifier>\n", indentString, identifier());
			strcpy(nameOfVar, identifier());
		}
		else //not a valid variable name token
		{
			printf("token variable not found at line %d\n", currentToken->line);
			freeToken();
			
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
				memset(nameOfVar, 0, 100);
				if(symbol() == ';')
				{
					break;
				}
				else if(symbol() == ',')
				{
					if(!hasMoreTokens()) //check for variable name token
					{
						printf("token variable name not found at line %d\n", currentToken->line);
						freeToken();
						
						exit(1);
					}
					advance(); //get the next variable name token 
					if(tokenType() == IDENTIFIER)
					{
						strcpy(nameOfVar, identifier());
						//we got the name call define
						define(nameOfVar, typeOfVar, kindOfVar);
					}
					else //not a valid variable name
					{
						printf("token variable not found at line %d\n", currentToken->line);
						freeToken();
						
						exit(1);
					}	
				}
				if(!hasMoreTokens()) // check for more token of type ',' or ';'
				{
					printf("terminating symbol ';' or ',' not found at line %d\n", currentToken->line);
					freeToken();
					
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
	startSubroutine();
	if(tokenType() == KEYWORD)
	{
		switch(keyWord())
		{
			case CONSTRUCTOR:
				functionType = 1;
				break;
			case METHOD:
				functionType = 2;
				argIndex++; //increment argIndex to account for pushing object as first argument
				break;
			case FUNCTION:
				break;
			default:   //static or field keyword not found return to compileClass Method
				return;
		}
		
	}
	else //if the token type in not keyword this may be possible some other statement
	{	  //return to compileClass method	
		return;
	}
	//we have come so far so it must be a subroutine Decleration
	//here we can safely start a new subroutine symbol table
	
	//check for syntatic correctness and exit program if some errors are found
	//check for 'type' token code block
	if(!hasMoreTokens()) 
	{
		printf("token 'type' not found at line %d\n", currentToken->line);
		freeToken();
		
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
					break;
				case CHAR:
					break;
				case BOOLEAN:
					break;
				case VOID:
					isVoid = 1;
					break;
				default: //not a valid keyword found in 'type' decleration
					printf("Return 'type' unknown at line %d\n", currentToken->line);
					freeToken();
					
					exit(1);
			}
		}
		else if(tokenType() == IDENTIFIER)
		{

		}
		else  //not found legal keyword or identifier in 'type' decleration
		{
			printf("Return 'type' unknown at line %d\n", currentToken->line);
			freeToken();
			
			exit(1);
		}
	}
	//subroutine name check block
	if(!hasMoreTokens()) 
	{
		printf("token subroutine name not found at line %d\n", currentToken->line);
		freeToken();
		
		exit(1);
	}
	else
	{
		advance(); //get next token for subroutine name
		if(tokenType() == IDENTIFIER)
		{
			strcpy(functionName, identifier()); //get the function name
		}
		else //not a valid subroutine name token
		{
			printf("invalid subroutine name at line %d\n", currentToken->line);
			freeToken();
			
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
		
		exit(1);
	}
	else
	{
		advance(); //get next token for symbol '('
		if(tokenType() == SYMBOL && symbol() == '(')
		{

		}
		else //not a valid subroutine name token
		{
			printf("expected symbol '(' at line %d\n", currentToken->line);
			freeToken();
			
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
		
		exit(1);
	}
	//compile subroutine body
	//check for symbol '{' for start of subroutine body
	if(!hasMoreTokens()) 
	{
		printf("expected symbol '{' at line for subroutine body %d\n", currentToken->line);
		freeToken();
		
		exit(1);
	}
	else
	{
		advance(); //get next token for symbol '{'
		if(tokenType() == SYMBOL && symbol() == '{')
		{
		}
		else //not a valid subroutine name token
		{
			printf("expected symbol '{' at line for subroutine body %d\n", currentToken->line);
			freeToken();
			
			exit(1);
		}
	}
	//compile var decleration
	compileVarDec();
	//we should have now method symbol table ready so display it
	//printf("-------------------------------------------------\n");
	//printf("Displaying Method symbol table\n");
	//displaySymbolTable(1);
	//printf("-------------------------------------------------\n");
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
	compileStatements();
	//since next token has already been read by compileVarDec() or compileStatements so just
	//check for symbol '}'
	if(tokenType() != SYMBOL || symbol() != '}')
	{
		printf("expected symbol '}' at line %d\n", currentToken->line);
		freeToken();
		
		exit(1);
	}
	//recussively call compileSubroutine
	if(!hasMoreTokens())
	{
		printf("expected symbol '}' for class decleration\n");
		freeToken();
		
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
	//make sure to reduce the indent before returning
	if(!hasMoreTokens()) 
	{
		printf("expected return 'type' or symbol ')' for paramenter list at line %d\n", currentToken->line);
		freeToken();
		
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
					strcpy(typeOfVar, "int");
					break;
				case CHAR:
					strcpy(typeOfVar, "char");
					break;
				case BOOLEAN:
					strcpy(typeOfVar, "boolean");
					break;
				default: //not a valid keyword found in 'type' decleration
					printf("Parameter list declareation unknown 'type' at line %d\n", currentToken->line);
					freeToken();
					
					exit(1);
			}
		}
		else if(tokenType() == IDENTIFIER) //type of some class name 'identifer'
		{
			strcpy(typeOfVar, identifier());
		}
		else if(tokenType() == SYMBOL && symbol() == ')') //we just found an empty parameter list
		{
			return;
		}
		else  //not found legal keyword or identifier in 'type' decleration
		{
			printf("Variable declareation unknown type at line %d\n", currentToken->line);
			freeToken();
			
			exit(1);
		}
	}
	//variable name if block
	if(!hasMoreTokens()) 
	{
		printf("token variable name not found at line %d\n", currentToken->line);
		freeToken();
		
		exit(1);
	}
	else
	{
		advance(); //get next token for variable name
		if(tokenType() == IDENTIFIER)
		{
			strcpy(nameOfVar, identifier());
			define(nameOfVar, typeOfVar, ARG_SMBL);
		}
		else //not a valid variable name token
		{
			printf("token variable not found at line %d\n", currentToken->line);
			freeToken();
			
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
					return;
				case ',':
					break;
				default:
					printf("Parameter decleration error at line %d\n", currentToken->line);
					freeToken();
					
					exit(1);
			}
		}
		if(!hasMoreTokens()) // check for more token of type keyword int, char, boolean
		{
			printf("Parameter decleration error at line %d\n", currentToken->line);
			freeToken();
			
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
					strcpy(typeOfVar, "int");
					break;
				case CHAR:
					strcpy(typeOfVar, "char");
					break;
				case BOOLEAN:
					strcpy(typeOfVar, "boolean");
					break;
				default: //not a valid keyword found in 'type' decleration
					printf("Parameter list declareation unknown 'type' at line %d\n", currentToken->line);
					freeToken();
					
					exit(1);
			}
		}
		else if(tokenType() == IDENTIFIER)
		{
			strcpy(typeOfVar, identifier());
		}
		else
		{
			printf("Parameter list declareation unknown 'type' at line %d\n", currentToken->line);
			freeToken();
			
			exit(1);
		}
		if(!hasMoreTokens()) // check for more token for identifier
		{
			printf("Parameter decleration error at line %d\n", currentToken->line);
			freeToken();
			
			exit(1);
		}
		else
		{
			advance();
		}
		
		if(tokenType() == IDENTIFIER)
		{
			strcpy(nameOfVar, identifier());
			define(nameOfVar, typeOfVar, ARG_SMBL);
		}
		else //not a valid variable name token
		{
			printf("token variable not found at line %d\n", currentToken->line);
			freeToken();
			
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
					strcpy(typeOfVar, "int");
					break;
				case CHAR:
					strcpy(typeOfVar, "char");
					break;
				case BOOLEAN:
					strcpy(typeOfVar, "boolean");
					break;
				default: //not a valid keyword found in 'type' decleration
					printf("Variable declareation unknown 'type' at line %d\n", currentToken->line);
					freeToken();
					
					exit(1);
			}
		}
		else if(tokenType() == IDENTIFIER)
		{
			strcpy(typeOfVar, identifier());
		}
		else  //not found legal keyword or identifier in 'type' decleration
		{
			printf("Variable declareation unknown type at line %d\n", currentToken->line);
			freeToken();
			
			exit(1);
		}
	}
	//variable name if block
	if(!hasMoreTokens()) 
	{
		printf("token variable name not found at line %d\n", currentToken->line);
		freeToken();
		
		exit(1);
	}
	else
	{
		advance(); //get next token for variable name
		if(tokenType() == IDENTIFIER)
		{
			strcpy(nameOfVar, identifier());
			define(nameOfVar, typeOfVar, VAR_SMBL);
			numOfLocals++;
		}
		else //not a valid variable name token
		{
			printf("token variable not found at line %d\n", currentToken->line);
			freeToken();
			
			exit(1);
		}
	}
	//below block of code for occurance of code like in next line
	//(',' varName)* ';' block
	if(!hasMoreTokens())
	{
		printf("terminating symbol ';' or ',' not found at line %d\n", currentToken->line);
		freeToken();
		
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
					break;
				}
				else if(symbol() == ',')
				{
					if(!hasMoreTokens()) //check for variable name token
					{
						printf("token variable name not found at line %d\n", currentToken->line);
						freeToken();
						
						exit(1);
					}
					advance(); //get the next variable name token 
					if(tokenType() == IDENTIFIER)
					{
						strcpy(nameOfVar, identifier());
						define(nameOfVar, typeOfVar, VAR_SMBL);
						numOfLocals++;
					}
					else //not a valid variable name
					{
						printf("token variable not found at line %d\n", currentToken->line);
						freeToken();
						
						exit(1);
					}	
				}
				if(!hasMoreTokens()) // check for more token of type ',' or ';'
				{
					printf("terminating symbol ';' or ',' not found at line %d\n", currentToken->line);
					freeToken();
					
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
	//read next token which should be identifier
	char functionCallName[200], objectName[100], tempBuff[100];
	char *objectClass;
	int methodCallOnly = 1; //to track if its only call to method 1 = true, 0 = false
	memset(functionCallName, 0, 200);
	memset(objectName, 0, 100);
	memset(tempBuff, 0, 100);
	numOfParameter = 0;
	if(!hasMoreTokens())
	{
		printf("expected an identifier at line %d\n", currentToken->line);
		freeToken();
		
		exit(1);
	}
	else
	{
		advance(); 
		if(tokenType() != IDENTIFIER)
		{
			printf("expected an identifier at line %d\n", currentToken->line);
			freeToken();
			
			exit(1);
		}
		//get the type of for current identifier to check if its object of some class
		objectClass = typeOf(identifier());
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
		
		exit(1);
	}
	else
	{
		advance();
		if(tokenType() != SYMBOL)
		{
			printf("expected a symbol '(' or '.' at line %d\n", currentToken->line);
			freeToken();
			
			exit(1);
		}
		if(symbol() == '.') //found '.' symbol its a call to Class.function() or object.method()
		{
			methodCallOnly = 0;
			strcat(functionCallName, ".");
			//read next token should be a identifier
			if(!hasMoreTokens())
			{
				printf("expected a identifier at line %d\n", currentToken->line);
				freeToken();
				
				exit(1);
			}
			else
			{
				advance();
				if(tokenType() != IDENTIFIER)
				{
					printf("expected an identifier at line %d\n", currentToken->line);
					freeToken();
					
					exit(1);
				}
				strcat(functionCallName, identifier());
			}
			//read next token and should be a '(' symbol
			//but it will be processed in next if block
			if(!hasMoreTokens())
			{
				printf("expected a symbol '(' at line %d\n", currentToken->line);
				freeToken();
				
				exit(1);
			}
			else
			{
				advance();
				if(tokenType() != SYMBOL)
				{
					printf("expected a symbol '(' at line %d\n", currentToken->line);
					freeToken();
					
					exit(1);
				}
			}				
		}
		if(symbol() == '(') //its a call to method() 
		{
			if(methodCallOnly)
			{
				numOfParameter++;
				writePush(POINTER_SEG, 0);
				strcpy(tempBuff, functionCallName); //copy method name to temp buff
				memset(functionCallName, 0, 200);
				sprintf(functionCallName, "%s.%s", className, tempBuff); //make a method name as Class.method
			}
		}
		else //use if(symbol != '(') to finish else block
		{
			printf("expected a symbol '(' or '.' at line %d\n", currentToken->line);
			freeToken();
			
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
		printf("expected a symbol ')' at line %d\n", currentToken->line);
		freeToken();
		
		exit(1);
	}
	//subroutineCall Ends here
	//read next token should be a symbol ';'
	if(!hasMoreTokens())
	{
		printf("expected a symbol ';' at line %d\n", currentToken->line);
		freeToken();
		
		exit(1);
	}
	else
	{
		advance(); 
		if(tokenType() != SYMBOL || symbol() != ';')
		{
			printf("expected a symbol ';' at line %d\n", currentToken->line);
			freeToken();
			
			exit(1);
		}
	}
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
	int isArray = 0; //variable to keep track if array assignment is present in let statement 0=No, 1=Yes 
	char vName[100]; //name of variable
	memset(vName, 0, 100); //reset varName buffer
	//read next token which should be identifier
	if(!hasMoreTokens())
	{
		printf("expected an identifier at line %d\n", currentToken->line);
		freeToken();
		
		exit(1);
	}
	else
	{
		advance(); 
		if(tokenType() != IDENTIFIER)
		{
			printf("expected an identifier at line %d\n", currentToken->line);
			freeToken();
			
			exit(1);
		}
		strcpy(vName, identifier()); //copy identifer for later use
	}
	//read next token as symbol '[' or '='
	if(!hasMoreTokens())
	{
		printf("expected a symbol '[' or '=' at line %d\n", currentToken->line);
		freeToken();
		
		exit(1);
	}
	else
	{
		advance();
		if(tokenType() != SYMBOL)
		{
			printf("expected a symbol '[' or '=' at line %d\n", currentToken->line);
			freeToken();
			
			exit(1);
		}
		//code block for '[expression]'
		if(symbol() == '[')
		{
			isArray = 1;
			compileExpression();
			//add array start address to get desired memory location
			switch(kindOf(vName))
			{
				case STATIC_SMBL:
					writePush(STATIC_SEG, indexOf(vName));
					break;
				case FIELD_SMBL:
					writePush(THIS_SEG, indexOf(vName));
					break;
				case ARG_SMBL:
					writePush(ARG_SEG, indexOf(vName));
					break;
				case VAR_SMBL:
					writePush(LOCAL_SEG, indexOf(vName));
					break;
				default:	//we should never land into this code but just in case
					return;
			}
			writeArithmetic(ADD);
			//since next token has been ready by compileExpression()
			if(tokenType() != SYMBOL || symbol() != ']')
			{
				printf("expected a symbol ']' at line %d\n", currentToken->line);
				freeToken();
				
				exit(1);
			}
			//read the next token '=' for next if block
			if(!hasMoreTokens())
			{
				printf("expected a symbol '=' at line %d\n", currentToken->line);
				freeToken();
				
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

		}
		else
		{
			printf("expected a symbol '=' at line %d\n", currentToken->line);
			freeToken();
			
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
		
		exit(1);
	}
	
	if(isArray)
	{
		writePop(TEMP_SEG, 0);
		writePop(POINTER_SEG, 1);
		writePush(TEMP_SEG, 0);
		writePop(THAT_SEG, 0);
	}
	else
	{
		switch(kindOf(vName))
		{
			case STATIC_SMBL:
				writePop(STATIC_SEG, indexOf(vName));
				break;
			case FIELD_SMBL:
				writePop(THIS_SEG, indexOf(vName));
				break;
			case ARG_SMBL:
				writePop(ARG_SEG, indexOf(vName));
				break;
			case VAR_SMBL:
				writePop(LOCAL_SEG, indexOf(vName));
				break;
		}
	}			
}

void compileWhile()
{
	//straight away write <whileStatement> tag as it has been
	//ready by compileStatments function
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
		
		exit(1);
	}
	else
	{
		advance(); 
		if(tokenType() != SYMBOL || symbol() != '(')
		{
			printf("expected an symbol '(' at line %d\n", currentToken->line);
			freeToken();
			
			exit(1);
		}
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
		
		exit(1);
	}
	//read next token should be a symbol '{'
	if(!hasMoreTokens())
	{
		printf("expected an symbol '{' at line %d\n", currentToken->line);
		freeToken();
		
		exit(1);
	}
	else
	{
		advance(); 
		if(tokenType() != SYMBOL || symbol() != '{')
		{
			printf("expected an symbol '{' at line %d\n", currentToken->line);
			freeToken();
			
			exit(1);
		}
	}
	//read next token and
	//then compile statements in a while loop
	if(!hasMoreTokens())
	{
		printf("expected a statement or symbol '}' at line %d\n", currentToken->line);
		freeToken();
		
		exit(1);
	}
	else
	{
		advance(); 
	}
	compileStatements();
	//since the next tokens as already ready by compileStatement()
	//just check for symbol ;}'
	if(tokenType() != SYMBOL || symbol() != '}')
	{
		printf("expected an symbol '}' at line %d\n", currentToken->line);
		freeToken();
		
		exit(1);
	}
	memset(loopLabel, 0, 100);
	sprintf(loopLabel, "WHILE_START_%d", localIndex);
	writeGoto(loopLabel);
	memset(loopLabel, 0, 100);
	sprintf(loopLabel, "WHILE_END_%d", localIndex);
	writeLabel(loopLabel);
}
void compileReturn()
{
	//straight away write <ReturnStatement> tag as it has been
	//ready by compileStatments function
	//straight way call compileExpression
	compileExpression();
	//since next token has been read by compileExpression
	if(tokenType() != SYMBOL || symbol() != ';')
	{
		printf("expected a symbol ';' at line %d\n", currentToken->line);
		freeToken();
		
		exit(1);
	}
	//push constant 0 if return type is of type void
	if(isVoid)
	{
		writePush(CONST_SEG, 0);
	}
	writeReturn();
}
void compileIf()
{
	//straight away write <ifStatement> tag as it has been
	//ready by compileStatments function
	//struct fileToken *previousToken = 0;
	int localIndex = loopIfIndex++;
	char ifLabel[100];
	memset(ifLabel, 0, 100);
	//read next token should be a symbol '('
	if(!hasMoreTokens())
	{
		printf("expected an symbol '(' at line %d\n", currentToken->line);
		freeToken();
		
		exit(1);
	}
	else
	{
		advance(); 
		if(tokenType() != SYMBOL || symbol() != '(')
		{
			printf("expected an symbol '(' at line %d\n", currentToken->line);
			freeToken();
			
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
		
		exit(1);
	}
	//read next token should be a symbol '{'
	if(!hasMoreTokens())
	{
		printf("expected an symbol '{' at line %d\n", currentToken->line);
		freeToken();
		
		exit(1);
	}
	else
	{
		advance(); 
		if(tokenType() != SYMBOL || symbol() != '{')
		{
			printf("expected an symbol '{' at line %d\n", currentToken->line);
			freeToken();
			
			exit(1);
		}
	}
	//read next token and
	//then compile statements
	if(!hasMoreTokens())
	{
		printf("expected a statement or symbol '}' at line %d\n", currentToken->line);
		freeToken();
		
		exit(1);
	}
	else
	{
		advance(); 
	}
	memset(ifLabel, 0, 100);
	sprintf(ifLabel, "IF_TRUE_%d", localIndex);
	writeLabel(ifLabel);
	compileStatements();
	//since the next tokens as already ready by compileStatement()
	//just check for symbol }'
	if(tokenType() != SYMBOL || symbol() != '}')
	{
		printf("expected an symbol '}' at line %d\n", currentToken->line);
		freeToken();
		
		exit(1);
	}
	//look for else block
	//previousToken = currentToken;
	if(!hasMoreTokens())
	{
		printf("expected an symbol '}' or a statement at line %d\n", currentToken->line);
		freeToken();
		
		exit(1);
	}
	else
	{
		advance(); 
		//else keyword not found return
		//but we have also drive past on token also here try to fix it here
		if(tokenType() != KEYWORD || keyWord() != ELSE) 
		{
			current = currentToken;
			memset(ifLabel, 0, 100);
			sprintf(ifLabel, "IF_FALSE_%d", localIndex);
			writeLabel(ifLabel);
			return;												
		}
	}
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
		
		exit(1);
	}
	else
	{
		advance(); 
		if(tokenType() != SYMBOL || symbol() != '{')
		{
			printf("expected an symbol '{' at line %d\n", currentToken->line);
			freeToken();
			
			exit(1);
		}
	}
	//read next token and
	//then compile statements
	if(!hasMoreTokens())
	{
		printf("expected a statement or symbol '}' at line %d\n", currentToken->line);
		freeToken();
		
		exit(1);
	}
	else
	{
		advance(); 
	}
	compileStatements();
	//since the next tokens as already ready by compileStatement()
	//just check for symbol }'
	if(tokenType() != SYMBOL || symbol() != '}')
	{
		printf("expected an symbol '}' at line %d\n", currentToken->line);
		freeToken();
		
		exit(1);
	}
	memset(ifLabel, 0, 100);
	sprintf(ifLabel, "IF_END_%d", localIndex);
	writeLabel(ifLabel);
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
		
		exit(1);
	}
	else
	{
		advance();
		//this logic to accomadate expressionList & return statement
		//and term '(' expression ')'
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
	}
	compileTerm();
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
			return;
		}
		switch(symbol())
		{
			case '+':
				commType = ADD;
				break;
			case '-':
				commType = SUB;
				break;
			case '*':
				mulOrDivide = 1;
				break;
			case '/':
				mulOrDivide = 2;
				break;
			case '&'://use &amp;
				commType = AND;
				break;
			case '|':
				commType = OR;
				break;
			case '<': //use &lt;
				commType = LT;
				break;
			case '>':
				commType = GT;
				break;//use &gt;
			case '=':
				commType = EQ;
				break;	
			
			default:
				return;
		}
		if(!hasMoreTokens())
		{
			printf("expression error at line %d\n", currentToken->line);
			freeToken();
			
			exit(1);
		}
		else
		{
			advance();
			compileTerm();	
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
}
void compileTerm()
{
	char functionCallName[200], objectName[100], tempBuff[100];
	char *objectClass, *strVal;
	int i;
	memset(functionCallName, 0, 200);
	memset(objectName, 0, 100);
	memset(tempBuff, 0, 100);
	
	if(tokenType() == INT_CONST) //int constant
	{
		writePush(CONST_SEG, intVal());
	}
	else if(tokenType() == STRING_CONST) //string constant
	{
		strVal = stringVal();
		writePush(CONST_SEG, strlen(strVal));
		writeCall("String.new", 1);
		for(i=0;i<strlen(strVal);i++)
		{
			writePush(CONST_SEG, strVal[i]);
			writeCall("String.appendChar", 2);
		}
	}
	else if(tokenType() == KEYWORD) //keyword constant
	{
		switch(keyWord())
		{
			case TRUE:
				writePush(CONST_SEG, 0);
				writeArithmetic(NOT);
				break;
			case FALSE:
				writePush(CONST_SEG, 0);
				break;
			case NULL_KEYWORD:
				writePush(CONST_SEG, 0);
				break;
			case THIS:
				writePush(POINTER_SEG, 0);
				break;
			default:
				printf("unknown keyword constant for term decleration at line %d\n", currentToken->line);
				freeToken();
				
				exit(1);
		}
	}
	else if(tokenType() == IDENTIFIER) //it may be a varName | varName'['expression']' | subroutineCall
	{
		strcpy(functionCallName, identifier());
		if(!hasMoreTokens())
		{
			printf("expected ';' after term identifier at line %d\n", currentToken->line);
			freeToken();
			
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
				compileExpression();
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
					default:	//we should never land into this code but just in case
						return;
				}
				writeArithmetic(ADD);
				writePop(POINTER_SEG, 1);
				writePush(THAT_SEG, 0);
				//since the next token must have been read by compileExpression() just check for ']' character
				if(tokenType() != SYMBOL || symbol() != ']')
				{
					printf("Current token: %s\n", currentToken->stringToken);
					printf("expected a symbol ']' at line %d\n", currentToken->line);
					freeToken();
					
					exit(1);
				}
			}
			else if(symbol() == '(') //process subroutineName'(' expressionList ')'
			{
				numOfParameter = 0;
				numOfParameter++;
				writePush(POINTER_SEG, 0);
				strcpy(tempBuff, functionCallName); //copy method name to temp buff
				memset(functionCallName, 0, 200);
				sprintf(functionCallName, "%s.%s", className, tempBuff); //make a method name as Class.method
				compileExpressionList();
				//since the next token must have been read by compileExpressionList() just check for ')' character
				if(tokenType() != SYMBOL || symbol() != ')')
				{
					printf("Current token: %s\n", currentToken->stringToken);
					printf("expected a symbol ')' at line %d\n", currentToken->line);
					freeToken();
					
					exit(1);
				}
				writeCall(functionCallName, numOfParameter);
			}
			else if(symbol() == '.') //process className|varName.subroutineName '(' expressionList ')'
			{
				//get the type of from buffer functionCallName to check if its object of some class
				numOfParameter = 0;
				objectClass = typeOf(functionCallName);
				if(objectClass)//identifier is a name of object so its a method call
				{
					strcpy(objectName, functionCallName);
					memset(functionCallName, 0, 200);
					strcpy(functionCallName, objectClass);
				}
				strcat(functionCallName, ".");
				if(!hasMoreTokens()) //read subRoutineName
				{
					printf("expected identifier for subroutine name at line %d\n", currentToken->line);
					freeToken();
					
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
					
					exit(1);
				}
				strcat(functionCallName, identifier());
				//read symbol '('
				if(!hasMoreTokens())
				{
					printf("expected a symbol '(' at line %d\n", currentToken->line);
					freeToken();
					
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
					
					exit(1);
				}
				if(objectClass) //its a call to objects method
				{
					numOfParameter++; //increment number of paramenter for pushing object
					//get the memory segment and index for object
					switch(kindOf(objectName))
					{
						case STATIC_SMBL: //dont have logic what to put here
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
				//since the next token must have been read by compileExpressionList() just check for ')' character
				if(tokenType() != SYMBOL || symbol() != ')')
				{
					printf("Current token: %s\n", currentToken->stringToken);
					printf("expected a symbol ')' at line %d\n", currentToken->line);
					freeToken();
					
					exit(1);
				}
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
				return;
			}
		}
	}
	else if(tokenType() == SYMBOL) //'(' expression ')'
	{
		switch(symbol())
		{
			case '(':
				compileExpression();
				//since the next token must have been read by compileExpression() just check for ')' character
				if(tokenType() != SYMBOL || symbol() != ')')
				{
					printf("Current token: %s\n", currentToken->stringToken);
					printf("expected a symbol ')' at line %d\n", currentToken->line);
					freeToken();
					
					exit(1);
				}
				break;
			case '~':
				if(!hasMoreTokens()) //advance for compileTerm
				{
					printf("expected a term at line %d\n", currentToken->line);
					freeToken();
					
					exit(1);
				}
				else
				{
					advance();
				}
				compileTerm();
				writeArithmetic(NOT);
				return;
			case '-':
				if(!hasMoreTokens()) //advance for compileTerm
				{
					printf("expected a term at line %d\n", currentToken->line);
					freeToken();
					
					exit(1);
				}
				else
				{
					advance();
				}
				compileTerm();
				writeArithmetic(NEG);
				return;
			default:
				printf("unknown term format at line %d\n", currentToken->line);
				printf("Symbol: %c\n", symbol());
				freeToken();
				
				exit(1);
		}
	}
	if(!hasMoreTokens()) //advance for compileTerm
	{
		printf("term advance error at line %d\n", currentToken->line);
		freeToken();
		
		exit(1);
	}
	else
	{
		advance();
	}
}
void compileExpressionList()
{
	/* Below is a desi jugad for incrementing number of numOfParameters
	* as some of the logic of empty expressionList lies in functoin
	* compileExpression
	*/
	numOfParameter++;
	compileExpression();
	while(1)
	{
		if(tokenType() != SYMBOL || symbol() != ',')
		{
			return;
		}
		/* Below is a desi jugad for incrementing number of numOfParameters
		 * as some of the logic of empty expressionList lies in functoin
		 * compileExpression
		 */
		numOfParameter++;
		compileExpression();
	}
}
