
#ifndef LAB_PROJECT_STRUCTS_H
#define LAB_PROJECT_STRUCTS_H

#include "defines.h"
/*This file contains */

/*definition of first word type*/
typedef struct firstWord
{
    /*ERA bits*/
    unsigned int era: 2;
    /*Destination operand bits*/
    unsigned int destination: 2;
    /*source operand bits*/
    unsigned int source: 2;
    /*Opcode bits*/
    unsigned int opcode: 4;
    /*Group bits*/
    unsigned int group: 2;
    /*Unused bits - always 111*/
    unsigned int unused: 3;
    /*Last bit, always 0*/
    unsigned int last: 1;
} firstWord;

typedef struct{
    unsigned int opcode : 6;
    unsigned int rs : 5;
    unsigned int rt : 5;
    unsigned int rd : 5;
    unsigned int funct : 5;
    unsigned int unused : 6;
} InstructionR;

typedef struct{
    unsigned int opcode : 6;
    unsigned int rs : 5;
    unsigned int rt : 5;
    unsigned int immed : 16;
} InstructionI;

typedef struct{
    unsigned int opcode : 6;
    unsigned int reg : 1;
    unsigned int address : 25;
} InstructionJ;

/*----------------------------------------------------------------------------*/
/*Label/Tag  structures */
/*----------------------------------------------------------------------------*/

typedef struct{
    char name[MAX_TAG_LEN];
    int address;
    int external;
    int command;
    int kind;
} Tag;

enum SYMBOL_ATTRIBUTE{CODE,DATA,ENTRY, EXTERNAL};

typedef struct{
    char name[MAX_TAG_LEN];
    int value;
    int attribute;
} Symbol;


typedef struct{
    char name[MAX_TAG_LEN];
} Entry;
typedef struct{
    char name[MAX_TAG_LEN];
} Extern;

typedef struct{
    char * line;
    /* tag counter */
    int tc;
    /* line counter */
    int lc;
    /* instruction counter */
    int ic;
    /* extern counter */
    int exc;
    /* entry counter */
    int enc;
    /* extra words counter */
    int wc;

    Tag * tagArr;

    /* Remove In The End*/
    int * directiveArr;
    int dc;

    Extern * externArr;
    Entry * entryArr;

    Symbol * SymbolArr;
    /*
    Instruction * instArr;
    ExtraWord * wordArr;
    */


    /* if there's an error, no need for a second pass */
    int containError;
} Data;


#endif
