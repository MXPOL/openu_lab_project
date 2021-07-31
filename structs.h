#ifndef STRUCTS_H
#define STRUCTS_H
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

#endif