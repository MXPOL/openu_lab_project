
#ifndef LAB_PROJECT_DEFINES_H
#define LAB_PROJECT_DEFINES_H

/*This file contains enumerations and defines of constants*/

/*boolean enum*/
enum boolean {FALSE, TRUE};
enum {FIRST_PASS,SECOND_PASS};
enum {YES,NO,ERROR,INVALID};

#define LABEL_NOT_FOUND -1

/*Maximum line buffer size*/
#define MAX_LINE_LEN 80
/* maximum length of a tag is 31 */
#define MAX_TAG_LEN 31
/* maximum length of command*/
#define MAX_COMMAND_LENGTH 4
/* maximum length of operand*/
#define MAX_OPERAND_LENGTH 80

/*Maximum filename size*/
#define MAX_FILENAME 50

/* Maximum line holder length*/
#define MAX_LINE_HOLDER_LENGTH 1000

/* state in getting directives*/
#define EXIT 0
#define GET_NUMBER 1
#define GET_COMMA 2


/*Number of registers*/
#define NUM_OF_REGISTERS 32

/*Directive string constants, to be compared with input*/
#define MAX_DIRECTIVE_LEN 6

#define thereIsATag(tag) (tag != NULL && *tag != '\0')

enum DIRECTIVE_KIND{CODE,DATA_DB, DATA_DH, DATA_DW, DATA_ASCIZ};
enum INSTRUCTION_TYPE {TYPE_R,TYPE_I,TYPE_J};

#define STR_DIRECTIVE_DB "db"
#define STR_DIRECTIVE_DH "dh"
#define STR_DIRECTIVE_DW "dw"

#define STR_DIRECTIVE_ASCIZ "asciz"
#define STR_DIRECTIVE_ENTRY "entry"
#define STR_DIRECTIVE_EXTERN "extern"

/*Maximum and minimum values that can be stored in 16 bits*/
#define MAX_NUM_IMMEDIATE 32767
#define MIN_NUMBER_IMMEDIATE -32768

/*Maximum and minimum values that can be stored in data*/
/* 1 byte = 8 bits */
/*1 byte*/
#define MAX_NUMBER_DATA_DB 128
#define MIN_NUMBER_DATA_DB -127
/*2 bytes*/
#define MAX_NUMBER_DATA_DH 32767
#define MIN_NUMBER_DATA_DH -32768
/*4 bytes*/
#define MAX_NUMBER_DATA_DW 2147483647
#define MIN_NUMBER_DATA_DW -2147483647

/* these define the ascii codes 'a' and 'A' */
#define FIRST_CAPITAL_LETTER_ASCII 65
#define FIRST_LOWER_LETTER_ASCII 97

/*Amoount of opcodes*/
#define NUM_OF_COMMANDS 27
#define NUM_OF_RESERVED_WORD 33

/* operands types */
#define REGISTER 10
#define IMMEDIATE 11
#define LABEL 12
#define EMPTY -1
#define EMPTY_OPERAND -1

#define REGISTER_IMMEDIATE 13
#define REGISTER_LABEL 14
#define OPERAND_NOT_USED 15
#define FUNCT_NOT_USED 15



#endif