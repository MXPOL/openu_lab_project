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
void addSymbol(Data * data, char * tag, int dirAddress,int kind);
int createInstruction_R(Data * data,Instruction *newInstruction,InstructionInfo * instructionInfo,
                        int op1Address, int op2Address, int op3Address);
int createInstruction_I(Data * data,Instruction *newInstruction,InstructionInfo * instructionInfo,
                        int op1Address, int op2Address, int op3Address);


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
char* deblank(char* input);

/* directivesManager function declarations */
int directivesManager(Data * data, char * tag,int whatPass);
int dataDirectiveHandler(Data * data, char * tag,int kind);
void addDirective(Data * data, int directive,int kind);
void addDirective2(Data *data, int directive,int tagNumber, int kind);
void addDataDirective(Data * data, int directive,int *kind);
int stringDirectiveHandler(Data * data, char * tag);
int entryDirectiveHandler(Data * data, char * tag);
void addEntry(Data * data, char * tag);
int externDirectiveHandler(Data * data, char * tag);
int entryDirectiveHandlerSecondPass(Data *data);
void addExtern(Data * data, char * tag);

/* firstPassCommandsManager function declarations */
int firstPassCommandsManager(Data * data, char * tag);
int getCommandIndex(char* command);
int getNumOfOperands(int cmdIndex);
int checkZeroOperands(Data * data, char * tag,int kind);
int checkOneOperands(Data * data, char * tag,int kind);
char * getOneOperands(Data * data, char * tag,int kind);
int isOperandValidSyntax(Data * data, char * operand);
int getAddressingMethod(Data * data, char * operand);
int isEmptyOperand(char * operand);
int isRegisterOperand(char * operand);
int isTagOperand( char * operand);
int addOneOperandCommand(Data * data, char * tag,int commandIndex);
int getOpCode(int cmdIndex);
int getFunct(int cmdIndex);
int checkTwoOperands(Data * data, char * tag,char* command);
int checkThreeOperands(Data * data, char * tag,char* command);
int isImmediateOperand(Data * data,char * operand);
int getFirstOperandsType(int cmdIndex);
int getInstructionInfo (int cmdIndex, InstructionInfo * instruction);


int getNOperands(Data * data,int count, char * operand1,char* operand2,char *operand3);
int checkNOperands(Data * data,int count, char * operand1,char* operand2,char *operand3);
int AddInstruction(Data * data,int commandIndex, char * operand1,char* operand2,char *operand3);
int getAddress(Data * data, char * operand, int type);

/* secondPassManager function declarations*/
void secondPassManager( Data * data, FILE * file);
int lineHandlerSecondPass(Data * data,FILE * file);

/* secondPassCommandManager function declarations*/
int secondPassCommandManager(Data * data);
int secondPassCommandHandler(Data * data, int commandIndex, char * operand1,char* operand2,char *operand3);
int instruction_I_Handler(Data * data, int commandIndex, Instruction *instruction, char * operand1,char* operand2,char *operand3);
int instruction_J_Handler(Data * data, int commandIndex, Instruction *instruction, char * operand1,char* operand2,char *operand3);



/* output manager */
void outputManager(Data * data, char *);
void writeLengthsToFile(Data * data, char * filename);
char* decimalToBase32(unsigned long int decNum);
char *decimalToBinary(int number, int bits);
void createOutputZeroExtra(Data * , char * , int);
void writeToOutputFile(char * output,char *);
char *decimal_to_binary(int number, int bits);
void create_R_Instruction(Data * data, char * filename, Instruction *instruction);
void create_I_Instruction(Data * data, char * filename, Instruction *instruction);
void create_J_Instruction(Data * data, char * filename, Instruction *instruction);
int binary2decimal(char * bin);

/* dev func*/

void printInstArr (Data * data);

/* */
char *binaryToHex(char *binary, int length);
char *padBinary(char *binary, int length);
int validate(char *hex, int length);
char valueOf(char *halfByte);
char * littleEn (char * binary,int length );
void writeData (char * binary,int length);
char * fix16bits ( char * binary);
char * littleEnStyle (char * binary);
char * fix16 ( char *stringBits);
char * fix32 ( char *stringBits);


#endif


#endif
