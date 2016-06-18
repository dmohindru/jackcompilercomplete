/* jacktokenizer.h
 * This file implements JackTokenizer Module of Jack Compiler
 * for Hack computer
 * Author: Dhruv Mohindru
 * Date: 23/03/2016
 */
#include "jacktokenizer.h"

void constructorTokenizer(char *filename)
{
	//TODO: As check for filename extenstion .asm
	char *strToken, *dupLine, tempStr[100], *strEndInverted;
	int multiLineComment = 0, counter, tempStrCounter;//, strFound=0, distance=0;
	int innerCounter, strConstStart, strConstEnd, strConstLen, strProcessed;
	
	file = fopen ( filename, "r" );
	filelineno = 0;
	head = current = 0;
	if ( file == NULL )
	{
      perror(filename);
    }
    /*Basic logic for tokenizing a file
     * 1. Read a line with fgets
     * 2. increment file line no
     * 3. Use strtok function to loop through all the 
     * 	  string tokens file current file read
     * 4. Look for multiline comments
     * 5. If found loop thorough string tokens and even
     * 	  fgets function till end of multiline character
     * 	  found. Imp keep updading filelineno on ever
     *    successfull read of fgets
     * 6. Then look for Single line comments if found skip the
     *    current line
     * 7. If none of the above is found, then it may be a valid
     *    jack token or combination of two or three tokens or a 
     *    string token which would need special attentiong.
     * 8. Check string lenght of strToken. If its 1 then it might
     *    be possible symbol of jack language. Add it to linked list
     *    and proceed further.
     * 9. If not it may be a combination of symbols and other jack language 
     *    constructs. Now loop through strToken string and look for occurance of 
     *    symbols, string constants quotes
     * 10.If symbol is found add it to linked list. And loop through rest of
     *    string. 
     *    
     * Add it to a linked list of tokens which 
     *    records token string, fileline no where string was found
	 * 8. 
     * 8. End
     */
    while ( fgets ( line, sizeof(line), file ) != NULL ) // read a line 
    {
		filelineno++;
		//printf("Tokens at line %d: ", filelineno);
		dupLine = strdup(line);
		for(strToken = strtok(dupLine," \t\n"); strToken != NULL; strToken = strtok(NULL, " \t\n"))
		{
			if(multiLineComment)
			{
				if(strstr(strToken,"*/")) //look for end of mulitline comment
				{
					multiLineComment = 0; //if found continue to next token in line
				}
			}
			else if(strstr(strToken,"/*")) //its a start of multiline comment
			{
				multiLineComment = 1;
				if(strstr(strToken,"*/")) //look for end of mulitline comment
				{
					multiLineComment = 0; //if found continue to next token in line
				}
			}
			else if(strstr(strToken,"//")) //single line comment found break out of for loop
			{
				break;
			}
			else // it may be a valid token or combination of symbols and tokens
			{
				if(strlen(strToken) == 1) //then it may a valid symbol add it
				{
					
					addTokenToList(strToken); //may be a need of static pointer here
				}
				else //it may contain symbols and string constant quotes "
				{
					memset(tempStr,0,100); //first clear tempString buffer
					tempStrCounter = 0;
					strProcessed = 0;
					for(counter=0;counter<strlen(strToken);counter++)
					{
						if(strToken[counter] == '{' || strToken[counter] == '}' || \
						   strToken[counter] == '(' || strToken[counter] == ')' || \
						   strToken[counter] == '[' || strToken[counter] == ']' || \
						   strToken[counter] == '.' || strToken[counter] == ',' || \
						   strToken[counter] == ';' || strToken[counter] == '+' || \
						   strToken[counter] == '-' || strToken[counter] == '*' || \
						   strToken[counter] == '/' || strToken[counter] == '&' || \
						   strToken[counter] == '|' || strToken[counter] == '<' || \
						   strToken[counter] == '>' || strToken[counter] == '=' || \
						   strToken[counter] == '~')
						   { //we just found a symbol
							   //first check if it was first character
								if(tempStrCounter > 0) //check if there was a previous
								{					   //token being extracted
									tempStr[tempStrCounter] = '\0'; //set the null indicator
									addTokenToList(tempStr);
									memset(tempStr, 0, 100); //clear tempStr buffer
									tempStrCounter = 0; //reset tempStrCounter 
								}
								tempStr[0] = strToken[counter];
								tempStr[1] = '\0';
								addTokenToList(tempStr);
								memset(tempStr, 0, 100);
							}
							else if(strToken[counter] == '\"') //logic for string constant
							{
								if(tempStrCounter > 0) //check if there was a previous
								{					   //token being extracted
									tempStr[tempStrCounter] = '\0'; //set the null indicator
									addTokenToList(tempStr);
									memset(tempStr, 0, 100); //clear tempStr buffer
									tempStrCounter = 0; //reset tempStrCounter 
								}
								strProcessed = 1;
								strConstStart = (strToken+counter) - dupLine;
								strEndInverted = strchr(line+strConstStart+1, '\"');
								if(!strEndInverted) //not found end inverted commas copy till end of line
								{					//may be a bug in this block of if code
									strcpy(tempStr, line+strConstStart);
									addTokenToList(tempStr);
									break; 
								}
								else //we found end inverted commas
								{
									strConstEnd = strEndInverted - line;
									strConstLen = strConstEnd - strConstStart - 1;
									strToken = dupLine + strConstEnd + 1;
									strncpy(tempStr, line+strConstStart, strConstLen+2); //accomadate for two inverted commas
									addTokenToList(tempStr);
									memset(tempStr, 0, 100);
									for(innerCounter=0;innerCounter<strlen(strToken);innerCounter++)
									{
										if(strToken[innerCounter] == '{' || strToken[innerCounter] == '}' || \
										strToken[innerCounter] == '(' || strToken[innerCounter] == ')' || \
										strToken[innerCounter] == '[' || strToken[innerCounter] == ']' || \
										strToken[innerCounter] == '.' || strToken[innerCounter] == ',' || \
										strToken[innerCounter] == ';' || strToken[innerCounter] == '+' || \
										strToken[innerCounter] == '-' || strToken[innerCounter] == '*' || \
										strToken[innerCounter] == '/' || strToken[innerCounter] == '&' || \
										strToken[innerCounter] == '|' || strToken[innerCounter] == '<' || \
										strToken[innerCounter] == '>' || strToken[innerCounter] == '=' || \
										strToken[innerCounter] == '~')
										{ //we found a symbol
											if(tempStrCounter > 0) //check if there was a previous
											{					   //token being extracted
												tempStr[tempStrCounter] = '\0'; //set the null indicator
												addTokenToList(tempStr);
												memset(tempStr, 0, 100); //clear tempStr buffer
												tempStrCounter = 0; //reset tempStrCounter 
											}
											tempStr[0] = strToken[innerCounter];
											tempStr[1] = '\0';
											addTokenToList(tempStr);
											memset(tempStr, 0, 100);
										}
										else
										{
											tempStr[tempStrCounter] = strToken[innerCounter];
											tempStrCounter++;
										}
									}
									if(tempStrCounter > 0) //string had no symbol or string ended with no symbols
									{					   //this block of if may be a redudant code
										tempStr[tempStrCounter] = '\0';
										printf("token: %s\n", tempStr);
										tempStrCounter = 0;
										memset(tempStr, 0, 100);
									}
									break;
								}
							} //sting constant logic ends here
							else
							{
								tempStr[tempStrCounter] = strToken[counter];
								tempStrCounter++;
							}
						}
						if(tempStrCounter > 0) //string had no symbol or string ended with no symbols
						{
							tempStr[tempStrCounter] = '\0';
							addTokenToList(tempStr);
							tempStrCounter = 0;
							memset(tempStr, 0, 100);
						}
						if(strProcessed)
						{
							strProcessed = 0;
							break;
						}
					}
				
			}
		}
	}
	displayTokens();
	current = head;
}
int hasMoreTokens()
{
    if(current)
    {
		return 1;
	}
	return 0;
}
void advance()
{
	currentToken = current;
	current = current->next;
}
token tokenType()
{
	if(!strcmp(currentToken->stringToken,"class") || !strcmp(currentToken->stringToken,"constructor") \
	|| !strcmp(currentToken->stringToken,"function") || !strcmp(currentToken->stringToken,"method") \
	|| !strcmp(currentToken->stringToken,"field") || !strcmp(currentToken->stringToken,"static") \
	|| !strcmp(currentToken->stringToken,"var") || !strcmp(currentToken->stringToken,"int") \
	|| !strcmp(currentToken->stringToken,"char") || !strcmp(currentToken->stringToken,"boolean") \
	|| !strcmp(currentToken->stringToken,"void") || !strcmp(currentToken->stringToken,"true") \
	|| !strcmp(currentToken->stringToken,"false") || !strcmp(currentToken->stringToken,"null") \
	|| !strcmp(currentToken->stringToken,"this") || !strcmp(currentToken->stringToken,"let") \
	|| !strcmp(currentToken->stringToken,"do") || !strcmp(currentToken->stringToken,"if") \
	|| !strcmp(currentToken->stringToken,"else") || !strcmp(currentToken->stringToken,"while") \
	|| !strcmp(currentToken->stringToken,"return"))
	{
		
		return KEYWORD;
	}
	else if(!strcmp(currentToken->stringToken,"{") || !strcmp(currentToken->stringToken,"}") \
	|| !strcmp(currentToken->stringToken,"(") || !strcmp(currentToken->stringToken,")") \
	|| !strcmp(currentToken->stringToken,"[") || !strcmp(currentToken->stringToken,"]") \
	|| !strcmp(currentToken->stringToken,".") || !strcmp(currentToken->stringToken,",") \
	|| !strcmp(currentToken->stringToken,";") || !strcmp(currentToken->stringToken,"+") \
	|| !strcmp(currentToken->stringToken,"-") || !strcmp(currentToken->stringToken,"*") \
	|| !strcmp(currentToken->stringToken,"/") || !strcmp(currentToken->stringToken,"&") \
	|| !strcmp(currentToken->stringToken,"|") || !strcmp(currentToken->stringToken,"<") \
	|| !strcmp(currentToken->stringToken,">") || !strcmp(currentToken->stringToken,"=") \
	|| !strcmp(currentToken->stringToken,"~"))
	{
		return SYMBOL;
	}
	else if(currentToken->stringToken[0] == '\"') //Then it is possibly a string constant
	{
		if(currentToken->stringToken[strlen(currentToken->stringToken)-1] == '\"') //check for ending inverted commas
		{
			return STRING_CONST;
		}
		return ERROR_TOKEN; //return error if inverted commas not found
	}
	else if(isJackValidInt(currentToken->stringToken) > 0) //check for integer constant
	{
		return INT_CONST;
	}
	else if(isJackValidIdentifier(currentToken->stringToken))
	{
		return IDENTIFIER;
	}
	
	// nothing found return error
	return ERROR_TOKEN;
}
keyword keyWord()
{
	if(!strcmp(currentToken->stringToken, "class"))
	{
		return CLASS;
	}
	else if(!strcmp(currentToken->stringToken, "constructor"))
	{
		return CONSTRUCTOR;
	}
	else if(!strcmp(currentToken->stringToken, "function"))
	{
		return FUNCTION;
	}
	else if(!strcmp(currentToken->stringToken, "method"))
	{
		return METHOD;
	}
	else if(!strcmp(currentToken->stringToken, "field"))
	{
		return FIELD;
	}
	else if(!strcmp(currentToken->stringToken, "static"))
	{
		return STATIC;
	}
	else if(!strcmp(currentToken->stringToken, "var"))
	{
		return VAR;
	}
	else if(!strcmp(currentToken->stringToken, "int"))
	{
		return INT;
	}
	else if(!strcmp(currentToken->stringToken, "char"))
	{
		return CHAR;
	}
	else if(!strcmp(currentToken->stringToken, "boolean"))
	{
		return BOOLEAN;
	}
	else if(!strcmp(currentToken->stringToken, "void"))
	{
		return VOID;
	}
	else if(!strcmp(currentToken->stringToken, "true"))
	{
		return TRUE;
	}
	else if(!strcmp(currentToken->stringToken, "false"))
	{
		return FALSE;
	}
	else if(!strcmp(currentToken->stringToken, "null"))
	{
		return NULL_KEYWORD;
	}
	else if(!strcmp(currentToken->stringToken, "this"))
	{
		return THIS;
	}
	else if(!strcmp(currentToken->stringToken, "let"))
	{
		return LET;
	}
	else if(!strcmp(currentToken->stringToken, "do"))
	{
		return DO;
	}
	else if(!strcmp(currentToken->stringToken, "if"))
	{
		return IF;
	}
	else if(!strcmp(currentToken->stringToken, "else"))
	{
		return ELSE;
	}
	else if(!strcmp(currentToken->stringToken, "while"))
	{
		return WHILE;
	}
	else if(!strcmp(currentToken->stringToken, "return"))
	{
		return RETURN;
	}
	return ERROR_KEYWORD;
}
char symbol()
{
	return currentToken->stringToken[0];
}
char* identifier()
{
   return currentToken->stringToken;
}
int intVal()
{
	long ret;
	char *end;
	ret = strtol(currentToken->stringToken, &end, 10);
	return ret;
}
char* stringVal()
{
	static char subString[100];
	memset(subString, 0, 100); //reset previous subString buffer
	int len = strlen(currentToken->stringToken);
	strncpy(subString, currentToken->stringToken+1, len-2);
	subString[len-1] = '\0';//set the null character 
	return subString;
}
void displayTokens()
{
	int total = 0;
	current = head;
	printf("Displaying tokens\n");
	while(current != 0)
	{
		printf("token: %s at line %d\n", current->stringToken, current->line);
		current = current->next;
		total++;
	}
	printf("Total Tokens: %d\n", total);
}
void freeToken()
{
	struct fileToken *temp;
	current = head;
	while(current != 0)
	{
		temp = current;
		current = current->next;
		free(temp);
	}
	current = head = 0;
	
}
void addTokenToList(char *strToken)
{
	struct fileToken *temp;
	if(head == 0) //if it is a first token in file
	{
		head = (struct fileToken*)malloc(sizeof(struct fileToken));
		current = head;
		strcpy(head->stringToken, strToken); //copy token string
		head->line = filelineno;
		head->next = 0;
	}
	else
	{
		temp = (struct fileToken*)malloc(sizeof(struct fileToken));
		strcpy(temp->stringToken, strToken);
		temp->line = filelineno;
		temp->next = 0;
		current->next = temp;
		current = temp;
	}
}
int isJackValidInt(char *strToNum)
{
	long ret;
	char *end;
	ret = strtol(strToNum, &end, 10);
	if(end-strToNum != strlen(strToNum)) //its not a valid number
    {
		return -1;
	}
	else //its a valid number but check for its range
	{
		if(ret < 0 || ret > 32767)
		{
			return -1;
		}
		else
		{
			return 1;
		}
	}
}
int isJackValidIdentifier(char *str)
{
	char *end;
	strtol(str, &end, 10);
	if(end == str) //its a valid identifier
	{
		return 1;
	}
	return 0;
}
