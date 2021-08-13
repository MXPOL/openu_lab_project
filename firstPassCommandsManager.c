/*
 ====================================================================================
 Module:        firstPassCommandsManager
 Description: 	module validates all the commands during the first pass and adds tags
 ====================================================================================
 */



#include "header.h"
/*----------------------------------------------------------------------------*/
/*
 * Description: parses and validates the commands from input
 * Input:       Data struct, string with tag names (might be null if no tag was found)
 * Output:		1 if successful, 0 otherwise
 */
/*----------------------------------------------------------------------------*/
int firstPassCommandsManager(Data * data, char * tag){
    char command[5];
    int operandNum;
    int commandIndex;

    if (sscanf(data->line, "%4s", command) == 0){
        printf("[Error] on line %d:- command error invalid \n", data->lc);
        data->containError=TRUE;
        return 0;
    }
    commandIndex = getCommandIndex(command);


    if (commandIndex == -1){
        printf("[Error] on line %d:- command error not defined\n", data->lc);
        data->containError=TRUE;
        return 0;
    }

    data->line+=strlen(command);

    if(!isspace(*(data->line))){
        printf("[Error] on line %d:- extra characters or no space after command name\n", data->lc);
        data->containError=TRUE;
        return 0;
    }

    eatSpace(data);

    operandNum = getNumOfOperands(commandIndex);
    printf("command - %s operandNum - %d \n",command,operandNum );


    if(operandNum==0){
        return checkZeroOperands(data,tag,commandIndex);


    }
    else if(operandNum==1){
        return checkOneOperands(data,tag,commandIndex);
    }
    /*
    else if(operandNum==2){
        return checkTwoOperands(data, tag,command);
    }
     */
    return 0;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: gives you the index number of the command
 * Input:       command string
 * Output:		int index of the command, -1 if it doesn't exist
 */
/*----------------------------------------------------------------------------*/
int getCommandIndex(char* command){
    int numOfCommands = NUM_OF_COMMANDS;
    char * commandArr[] = {"add", "sub", "and", "or", "nor", "move", "mvhi", "mvlo",
                           "addi", "subi", "andi", "ori", "nori", "bne", "beq", "btl","bgt","lb","sb","lw","sw","lh","sh",
                           "jmp","la", "call","stop"};
    int i;
    for (i=0;i<numOfCommands;i++){
        if (strcmp(commandArr[i],command)==0){
            return i;
        }
    }
    return -1;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: return the number of operands for the command
 * Input:       the index of the command
 * Output:		int number of operands
 */
/*----------------------------------------------------------------------------*/
int getNumOfOperands(int cmdIndex){
    int operandNum[] = {3, 3, 3, 3, 3, 2, 2, 2,
                        3, 3, 3, 3,3, 3, 3, 3,3,3,3,3,3,3,3,
                        1,1, 1,0};
    return operandNum[cmdIndex];
}

/*----------------------------------------------------------------------------*/
/*
 * Description: validate a command with zero operands
 * Input:       Data struct, tag string
 * Output:		1 if successful, 0 otherwise
 */
/*----------------------------------------------------------------------------*/
int checkZeroOperands(Data * data, char * tag,int kind){
    if(*(data->line)=='\n' || *(data->line)==EOF){
        if(tag != NULL && *tag !='\0'){
            addTag(data,tag,data->dc,kind);
        }
        data->ic++;
        return 1;
    }
    printf("[Error] on line %d: illegal arguments - command should have zero operands.\n", data->lc);
    data->containError=TRUE;
    return 0;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: validate a command with one operand
 * Input:       Data struct, tag string
 * Output:		1 if successful, 0 otherwise
 */
/*----------------------------------------------------------------------------*/
int checkOneOperands(Data * data, char * tag,int kind){
    char operand[MAX_TAG_LEN];
    /* 30s = 30 chars after the command */
    if(sscanf(data->line, "%30s", operand) == 0){
        printf("[Error] on line %d: no operands given - one operand required\n", data->lc);
        data->containError=TRUE;
        return 0;
    }
    data->line+= strlen(operand);
    eatSpace(data);
    /* shouldn't be any additional character or arguments after the operand */
    if(isEndOfLine(data->line) == 0){
        printf("[Error] on line %d: extra argument or characters after the operand \n", data->lc);
        data->containError=TRUE;
        return 0;
    }
    if (isOperandValidSyntax(data,operand) == 0){
        printf("[Error] on line %d: operand is invalid \n", data->lc);
        data->containError=TRUE;
        return 0;
    }
    /* what happens when IC and DC are the same? how does it differentiate address? is DC limited to 100? */
    if(tag != NULL && *tag !='\0'){
        addTag(data,tag,data->dc,kind);
    }
    /* there is one more instruction to allocate memory for at end of first pass */
    data->ic++;
    /* there is one more extra word to allocate memory for at end of first pass */
    data->wc++;
    return 1;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: check if the syntax of the operand is valid
 * Input:       Data struct, tag string, command string
 * Output:		1 if valid, 0 otherwise
 */
/*----------------------------------------------------------------------------*/
int isOperandValidSyntax(Data * data, char * operand){
    if(getAddressingMethod(data,operand) != -1){
        return 1;
    }
    return 0;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: get the addressing method of the operand and check validity
 * Input:       Data struct, operand string
 * Output:		int addressing method code, -1 if invalid
 * NOTE:        might print out errors if invalid
 */
/*----------------------------------------------------------------------------*/
int getAddressingMethod(Data * data, char * operand) {
    if(isEmptyOperand(operand)==1){
        return EMPTY_OPERAND;
    }
    else if(isTagOperand(operand)==1){
        return TAG_OPERAND;
    }
    else if(isRegisterOperand(operand)==1){
        return REGISTER_OPERAND;
    }
    return -1;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: check if the operand is empty or not
 * Input:       string operand
 * Output:		1 if empty, 0 if not empty
 */
/*----------------------------------------------------------------------------*/
int isEmptyOperand(char * operand){
    operand = getCharPtrBeyondSpace(operand);
    if(*operand == '\n' || *operand==EOF){
        return 1;
    }
    return 0;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: check if the addressing method is register addressing or not
 * Input:       string operand
 * Output:		1 if register, 0 if not register
 */
/*----------------------------------------------------------------------------*/
int isTagOperand( char * operand){
    char tempOperand[MAX_TAG_LEN];
    char * c;
    strcpy(tempOperand,operand);
    strcat(tempOperand,"\n");

    c= tempOperand;

    /* operand has a $ sign at the start, so its register operand */
    if (*c == '$') {
        return 0;
    }

    if (checkLetters(*c)== 0){
        return 0;
    }
    c++;
    while(!isspace(*c) && *c != '\n' && *c !=EOF){
        if (checkLetterOrNumber(*c) == 0){
            return 0;
        }
        c++;
    }
    return 1;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: check if the addressing method is register addressing or not
 * Input:       string operand
 * Output:		1 if register, 0 if not register
 */
/*----------------------------------------------------------------------------*/
int isRegisterOperand(char * operand){
    char* regArr[]={"$0","$1","$2","$3","$4","$5","$6","$7",
                    "$8","S9","$10","$11","$12","$13","$14","$15",
                    "$16","$17","$18","$19","$20","$21","$22","$23",
                    "$24","$25","$26","$27","$28","$29","$30","$31",};
    int i;
    for(i=0;i<NUM_OF_REGISTERS;i++){
        if(strcmp(operand,regArr[i])==0){
            return 1;
        }
    }
    return 0;
}

