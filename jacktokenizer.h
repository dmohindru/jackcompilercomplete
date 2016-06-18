/* jacktokenizer.h
 * This file implements JackTokenizer Module of Jack Compiler
 * for Hack computer
 * Author: Dhruv Mohindru
 * Date: 23/03/2016
 */
#ifndef JACKTOKENIZER_H
#define JACKTOKENIZER_H
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
 FILE *file; //current jack file pointer for reading
 DIR *dir; //current dir pointer for reading
 int filelineno;
 char line[200], curToken[200];
 struct fileToken
 {
	 char stringToken[100];
	 int line;
	 struct fileToken *next;
 };
 struct fileToken *head, *current, *currentToken;
 typedef enum token
 {
	 KEYWORD, SYMBOL, IDENTIFIER, INT_CONST, STRING_CONST, ERROR_TOKEN
 } token;
 typedef enum keyword
 {
	 CLASS, METHOD, FUNCTION, CONSTRUCTOR, INT, 
	 BOOLEAN, CHAR, VOID, VAR, STATIC, FIELD, LET, DO, 
	 IF, ELSE, WHILE, RETURN, TRUE, FALSE, NULL_KEYWORD,
	 THIS, ERROR_KEYWORD
 } keyword;
 void constructorTokenizer(char *filename);
 int hasMoreTokens();
 void advance();
 token tokenType();
 keyword keyWord();
 char symbol();
 char* identifier();
 int intVal();
 char* stringVal();
 //temp function
 void displayTokens();
 void freeTokens();
 void addTokenToList(char *strToken);
 int isJackValidInt(char *strToNum);
 int isJackValidIdentifier(char *str);
 //
 #endif
 
 
 
