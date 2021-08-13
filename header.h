//
// Created by Maxim Polski on 13/08/2021.
//

#ifndef LAB_PROJECT_HEADER_H
#define LAB_PROJECT_HEADER_H

#ifndef HEADER_H
#define HEADER_H


/*-------------------------------------------*/
/*standard library imports */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/*data structures */
#include "structs.h"

/*enumerations and defines of constants*/
#include "defines.h"

/*-------------------------------------------*/



#define EXIT 0
#define GET_NUMBER 1
#define GET_COMMA 2

#define NUM_OF_COMMANDS 27

/*--------------------------------------------*/
/*data structures */

/*--------------------------------------------------------------------------------------------------*/


/*  passManager function declarations*/
/* firstPassManager function declarations*/
/* firstPassCommandsManager function declarations */
/* secondPassManager function declarations*/
/* commmonFunc declarations*/
/* directivesManager function declarations */
/* secondPassCommandManager function declarations*/
/* output manager */

/*  passManager function declarations*/
int passManager(FILE *file, char *);
void setDataFree(Data *data);
void initializeData(Data *data);

/* firstPassManager function declarations*/
int firstPassManager(Data * data,FILE *file);
int lineHandler(Data * data, FILE *file);
int lineLengthCheck(Data * data, FILE *file);
int tagDupCheck(Data *data, char *tag);
void addTag(Data * data, char * tag, int dirAddress,int kind);


/* commmonFunc declarations*/
int lineCommentCheck(Data * data);
void getTag(Data * data,char * tagGet);
int lineEmptyCheck(Data * data);
void eatLine(FILE * fp);
void eatSpace(Data * data);
void substring(char* stringTo,char* stringFrom,int length);
int checkLetterOrNumber(char c);
int checkLetters(char c);
int checkUpperCase(char c);
int checkInLimit(char c,int startLimit,int length);
char* getCharPtrBeyondSpace(char* pString);
int isEndOfLine(char* pStr);
void getTagOperand(Data * data, char * tagGet);

/* directivesManager function declarations */
int directivesManager(Data * data, char * tag);
int dataDirectiveHandler(Data * data, char * tag,int kind);
void addDirective(Data * data, int directive);
void addDataDirective(Data * data, int directive,int *kind);
int stringDirectiveHandler(Data * data, char * tag);
int entryDirectiveHandler(Data * data, char * tag);
void addEntry(Data * data, char * tag);
int externDirectiveHandler(Data * data, char * tag);
void addExtern(Data * data, char * tag);

/* firstPassCommandsManager function declarations */
int firstPassCommandsManager(Data * data, char * tag);
int getCommandIndex(char* command);
int getNumOfOperands(int cmdIndex);
int checkZeroOperands(Data * data, char * tag,int kind);
int checkOneOperands(Data * data, char * tag,int kind);
int isOperandValidSyntax(Data * data, char * operand);
int getAddressingMethod(Data * data, char * operand);
int isEmptyOperand(char * operand);
int isRegisterOperand(char * operand);
int isTagOperand( char * operand);


#endif


#endif //LAB_PROJECT_HEADER_H
