
#ifndef LAB_PROJECT_STRUCTS_H
#define LAB_PROJECT_STRUCTS_H

#include "defines.h"
/*This file contains */

typedef struct{
    unsigned int opcode : 6;
    unsigned int funct : 5;
    unsigned int rs : 5;
    unsigned int rt : 5;
    unsigned int rd : 5;
    unsigned int unused : 6;
    unsigned int reg : 1;
    unsigned int address : 25;
    int cmdIndex;
    int immed;
    int ic;
    int icf;
} Instruction;

typedef struct{
    int opcode;
    int funct;
    int operandsCount;
    int operand1Type;
    int operand2Type;
    int operand3Type;
    int instructionType;
} InstructionInfo;


/*----------------------------------------------------------------------------*/
/*Label/Tag  structures */
/*----------------------------------------------------------------------------*/
typedef struct{
    char name[MAX_TAG_LEN];
    int address;
    int external;
    int entry;
    int command;
    int kind;
} Tag;

typedef struct{
    char name[MAX_TAG_LEN];
    int value;
    int external;
    int attribute;
} Symbol;

typedef struct{
    int tagNumber;
    int icf;
    int data;
    int kind;
} Directive;


typedef struct{
    char name[MAX_TAG_LEN];
} Entry;


typedef struct{
    char name[MAX_TAG_LEN];
} Extern;

typedef struct{
    /* LINE */
    char * line;
    /* line counter */
    int lc;

    /* Data */
    Directive * directiveArr2;
    int * directiveArrIndex;
    int * directiveArr;
    int dc;
    int dcf;

    /* Symbols */
    Tag * tagArr;
    /* tag counter */
    int tc;

    /* Instruction*/
    /* instruction counter */
    int ic;
    Instruction * instArr;


    /* Extern */
    /* extern counter */
    int exc;
    Extern * externArr;

    /* Entry */
    /* entry counter */
    int enc;
    Entry * entryArr;

    /* Remove for sure*/
    /* extra words counter */
    int wc;

    /* Remove In The End*/





    Symbol * symbolArr;
    int symbolCounter;
    /*
    ExtraWord * wordArr;
    */


    /* if there's an error, no need for a second pass */
    int containError;
} Data;


#endif
