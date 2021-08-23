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

/*  passManager function declarations*/
int passManager(FILE *file, char *);
void setDataFree(Data *data);
void initializeData(Data *data);

/* firstPassManager function declarations*/
int firstPassManager(Data * data,FILE *file);
int lineHandler(Data * data, FILE *file);
int lineLengthCheck(Data * data, FILE *file);
int tagDupCheck(Data *data, char *tag);
int getTagAddress(Data *data, char *operand);
void addTag(Data * data, char * tag, int dirAddress,int kind);
void updateDataTable(Data *data);


/* commmonFunc declarations*/
int lineCommentCheck(Data * data);
int getTag(Data * data,char * tagGet);
int lineEmptyCheck(Data * data);
void eatLine(FILE * fp);
void eatSpace(Data * data);
int checkLetters(char c);
int checkUpperCase(char c);
int checkInLimit(char c,int startLimit,int length);
char* getCharPtrBeyondSpace(char* pString);
int isEndOfLine(char* pStr);
void getTagOperand(Data * data, char * tagGet);
void deleteSpaces (char* input);

/* directivesManager function declarations */
int directivesManager(Data * data, char * tag,int whatPass);
int dataDirectiveHandler(Data * data, char * tag,int kind);
void addDirective(Data *data, int directive,int tagNumber, int kind);
int stringDirectiveHandler(Data * data, char * tag);
int entryDirectiveHandler(Data * data, char * tag);
void addEntry(Data * data, char * tag);
int externDirectiveHandler(Data * data, char * tag);
int entryDirectiveHandlerSecondPass(Data *data);
void addExtern(Data * data, char * tag);

/* firstPassCommandsManager function declarations */
int firstPassCommandsManager(Data * data, char * tag);
int getCommandIndex(char* command);
int isItReservedWord(char *command);
int getOperandType(Data *data, char *operand,int operandNumber);
int isRegisterOperand(char *operan,char * errorLine);
int isEmptyOperand(char * operand);
int getRegisterIndex(char * operand);
int isTagOperand( char * operand);
int isImmediateOperand(Data * data,char * operand,char *errorLine);
int getImmediateNumber (char *operand, int * num);
int getInstructionInfo (int cmdIndex, InstructionInfo * instruction);
int getItExternTagAddress(Data *data, char *operand);
int createInstruction_R(Data * data,Instruction *newInstruction,InstructionInfo * instructionInfo,
                        int op1Address, int op2Address, int op3Address);
int createInstruction_I(Data * data,Instruction *newInstruction,InstructionInfo * instructionInfo,
                        int op1Address, int op2Address, int op3Address);
int createInstruction_J(Instruction *newInstruction,InstructionInfo * instructionInfo,
                        int op1Address);
int checkIllegalChars(char *command);


int getNOperands(Data * data,int count, char * operand1,char* operand2,char *operand3);
int checkNOperands(Data * data,InstructionInfo * instructionInfo,int count, char * operand1,char* operand2,char *operand3);
int AddInstruction(Data * data,int commandIndex, char * operand1,char* operand2,char *operand3);
int getAddress(Data * data, char * operand, int type);

/* secondPassManager function declarations*/
void secondPassManager( Data * data, FILE * file);
int lineHandlerSecondPass(Data * data,FILE * file);

/* secondPassCommandManager function declarations*/
int secondPassCommandManager(Data * data);
int secondPassCommandHandler(Data * data, int commandIndex, char * operand1,char* operand2,char *operand3);
int instruction_I_Handler(Data * data, int commandIndex, Instruction *instruction, char * operand1,char* operand2,char *operand3);
int instruction_J_Handler(Data * data, int commandIndex, Instruction *instruction, char * operand1);
int recordExternal(Data * data, int externalIndex);



/* output manager */
void outputManager(Data * data, char *);
void writeLengthsToFile(Data * data, char * filename);
void writeInstructionToFile(Data *data,char* outputFileName);
void writeExternToFile(Data * data,char * filename);
void writeEntryToFile(Data * data,char * filename);
void writeDirectivesToFile(Data * data,char * outputFileName);
void writeToOutputFile(char * output,char *);
char *decimal_to_binary(int number, int bits);
void print_R_Instruction(char * filename,int instructionIndex, Instruction *instruction);
void print_I_Instruction(char * filename,int instructionIndex, Instruction *instruction);
void print_J_Instruction(char * filename,int instructionIndex, Instruction *instruction);
char * littleEndian16BitsFormat ( char *stringBits);
char * littleEndian32BitsFormat ( char *stringBits);
char *binaryToHex(char *binary, int length);
char *padBinary(char *binary, int length);
char valueOf(char *halfByte);
char * littleEn (char * binary,int length );


/* */

/* dev func*/

void printInstArr (Data * data);

/* remove in the end */
void printEntryArr(Data *data);
void printDirectiveArr(Data *data);
void printInstArr(Data *data);
void printSymbolTable(Data *data);



#endif


#endif
