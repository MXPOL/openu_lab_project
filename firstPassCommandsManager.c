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

int firstPassCommandsManager(Data *data, char *tag) {
    InstructionInfo instructionInfo;
    char command[MAX_COMMAND_LENGTH + 1];
    char op1[MAX_OPERAND_LENGTH] = {0};
    char op2[MAX_OPERAND_LENGTH] = {0};
    char op3[MAX_OPERAND_LENGTH] = {0};
    int operandNum;
    int commandIndex;

    
    /* get the command from the line*/
    if (sscanf(data->line, "%4s", command) == 0) {
        printf("[Error] on line %d:- command error invalid\n", data->lc);
        data->containError = TRUE;
        return 0;
    }
    /* find the command index */
    commandIndex = getCommandIndex(command);

    if (commandIndex == -1) {
        printf("[Error] on line %d: command error not defined\n", data->lc);
        data->containError = TRUE;
        return 0;
    }

    /* skipping the command in the line*/
    data->line += strlen(command);

    /* check if there are space after command*/
    if (!isspace(*(data->line))) {
        printf("[Error] on line %d:- extra characters or no space after command name\n", data->lc);
        data->containError = TRUE;
        return 0;
    }

    /* skipping the spaces if there are */
    eatSpace(data);

    /* get info about the command*/
    getInstructionInfo(commandIndex, &instructionInfo);
    operandNum = instructionInfo.operandsCount;

    /* command with zero operands*/
    if (operandNum == 0 && getNOperands(data, 0, op1, op2, op3) == 1) {

        /* if there was tag before the command*/
        if (thereIsATag(tag)) {
            addTag(data, tag, data->ic, CODE);
        }

        /* add the instruction to the instruction array*/
        AddInstruction(data, commandIndex, op1, op2, op3);
        data->ic += 4;
        return 1;
    }
    /* command with 1 operands */
    if (operandNum == 1 && getNOperands(data, 1, op1, op2, op3) == 1 &&
        checkNOperands(data,&instructionInfo, 1, op1, op2, op3) == 1) {

        /* if there was tag before the command*/
        if (thereIsATag(tag)) {
            addTag(data, tag, data->ic, CODE);
        }

        /* add the instruction to the array*/
        AddInstruction(data, commandIndex, op1, op2, op3);
        data->ic += 4;
        return 1;
    }

    /* command with 2 operands*/
    if (operandNum == 2 && getNOperands(data, 2, op1, op2, op3) == 1 &&
        checkNOperands(data,&instructionInfo, 2, op1, op2, op3) == 1) {

        /* if there was tag before the command*/
        if (thereIsATag(tag)) {
            addTag(data, tag, data->ic, CODE);
        }
        /* add the instruction to the array*/
        AddInstruction(data, commandIndex, op1, op2, op3);
        data->ic += 4;
        return 1;
    }

    /* command with 3 operands*/
    if (operandNum == 3 && getNOperands(data, 3, op1, op2, op3) == 1 &&
        checkNOperands(data,&instructionInfo, 3, op1, op2, op3) == 1) {

        if (thereIsATag(tag)) {
            addTag(data, tag, data->ic, CODE);
        }
        /* add the instruction to the array*/
        AddInstruction(data, commandIndex, op1, op2, op3);
        data->ic += 4;
        return 1;
    }

    return 0;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: gives you the index number of the command
 * Input:       command string
 * Output:		int index of the command, -1 if it doesn't exist
 */
/*----------------------------------------------------------------------------*/
int getCommandIndex(char *command) {
    int i;
    int numOfCommands = NUM_OF_COMMANDS;
    char *commandArr[] = {"add", "sub", "and", "or", "nor", "move", "mvhi", "mvlo",
                          "addi", "subi", "andi", "ori", "nori", "bne", "beq", "blt", "bgt", "lb", "sb", "lw", "sw",
                          "lh", "sh",
                          "jmp", "la", "call", "stop"};

    for (i = 0; i < numOfCommands; i++) {
        if (strcmp(commandArr[i], command) == 0) {
            return i;
        }
    }
    return -1;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: Check if the string is saved word
 * Input:       command string
 * Output:		1 if it does 0 if it doesn't
 */
/*----------------------------------------------------------------------------*/
int isItReservedWord(char *command) {
    int i;
    char *commandArr[] = {"add", "sub", "and", "or", "nor", "move", "mvhi", "mvlo",
                          "addi", "subi", "andi", "ori", "nori", "bne", "beq", "blt", "bgt", "lb", "sb", "lw", "sw",
                          "lh", "sh",
                          "jmp", "la", "call", "stop","db","dh","dw","asciz","extern","entry"};

    for (i = 0; i < NUM_OF_RESERVED_WORD; i++) {
        if (strcmp(commandArr[i], command) == 0) {
            return 1;
        }
    }
    return 0;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: Checks if the cell contains invalid characters
 * Input:       command string
 * Output:		1 if it does 0 if it doesn't
 */
/*----------------------------------------------------------------------------*/
int checkIllegalChars(char *command) {
    char *p = command;
    int i;

    for (i = 0; *(p+i) != '\0' ; i++) {
        if (isalpha(*(p+i)) ==0 ){
            return 1 ;
        }
    }
    return 0;
}


/*----------------------------------------------------------------------------*/
/*
 * Description: Find the InstructionInfo struct, it holds data about the instuction
 * Input: commandIndex and pointer to instruction structs
 * Output: 1 if successful, 0 if fails(no command)
 */
/*----------------------------------------------------------------------------*/
int getInstructionInfo(int cmdIndex, InstructionInfo *instruction) {
    
    InstructionInfo operandsTypes[NUM_OF_COMMANDS] = {
            {0, 1,               3, REGISTER,         REGISTER,         REGISTER,         TYPE_R},
            {0, 2,               3, REGISTER,         REGISTER,         REGISTER,         TYPE_R},
            {0, 3,               3, REGISTER,         REGISTER,         REGISTER,         TYPE_R},
            {0, 4,               3, REGISTER,         REGISTER,         REGISTER,         TYPE_R},
            {0, 5,               3, REGISTER,         REGISTER,         REGISTER,         TYPE_R},
            {1, 1,               2, REGISTER,         REGISTER,         OPERAND_NOT_USED, TYPE_R},
            {1, 2,               2, REGISTER,         REGISTER,         OPERAND_NOT_USED, TYPE_R},
            {1, 3,               2, REGISTER,         REGISTER,         OPERAND_NOT_USED, TYPE_R},
            {10, FUNCT_NOT_USED, 3, REGISTER,         IMMEDIATE,        REGISTER,         TYPE_I},
            {11, FUNCT_NOT_USED, 3, REGISTER,         IMMEDIATE,        REGISTER,         TYPE_I},
            {12, FUNCT_NOT_USED, 3, REGISTER,         IMMEDIATE,        REGISTER,         TYPE_I},
            {13, FUNCT_NOT_USED, 3, REGISTER,         IMMEDIATE,        REGISTER,         TYPE_I},
            {14, FUNCT_NOT_USED, 3, REGISTER,         IMMEDIATE,        REGISTER,         TYPE_I},
            {15, FUNCT_NOT_USED, 3, REGISTER,         REGISTER,         LABEL,            TYPE_I},
            {16, FUNCT_NOT_USED, 3, REGISTER,         REGISTER,         LABEL,            TYPE_I},
            {17, FUNCT_NOT_USED, 3, REGISTER,         REGISTER,         LABEL,            TYPE_I},
            {18, FUNCT_NOT_USED, 3, REGISTER,         REGISTER,         LABEL,            TYPE_I},
            {19, FUNCT_NOT_USED, 3, REGISTER,         IMMEDIATE,        REGISTER,         TYPE_I},
            {20, FUNCT_NOT_USED, 3, REGISTER,         IMMEDIATE,        REGISTER,         TYPE_I},
            {21, FUNCT_NOT_USED, 3, REGISTER,         IMMEDIATE,        REGISTER,         TYPE_I},
            {22, FUNCT_NOT_USED, 3, REGISTER,         IMMEDIATE,        REGISTER,         TYPE_I},
            {23, FUNCT_NOT_USED, 3, REGISTER,         IMMEDIATE,        REGISTER,         TYPE_I},
            {24, FUNCT_NOT_USED, 3, REGISTER,         IMMEDIATE,        REGISTER,         TYPE_I},
            {30, FUNCT_NOT_USED, 1, REGISTER_LABEL,   OPERAND_NOT_USED, OPERAND_NOT_USED, TYPE_J},
            {31, FUNCT_NOT_USED, 1, LABEL,            OPERAND_NOT_USED, OPERAND_NOT_USED, TYPE_J},
            {32, FUNCT_NOT_USED, 1, LABEL,            OPERAND_NOT_USED, OPERAND_NOT_USED, TYPE_J},
            {63, FUNCT_NOT_USED, 0, EMPTY_OPERAND, OPERAND_NOT_USED, OPERAND_NOT_USED, TYPE_J},
    };

    if (cmdIndex < 0 || cmdIndex > NUM_OF_COMMANDS) {
        return 0;
    }

    /* fill the data to the info structs*/
    (*instruction).opcode = operandsTypes[cmdIndex].opcode;
    (*instruction).funct = operandsTypes[cmdIndex].funct;
    (*instruction).operandsCount = operandsTypes[cmdIndex].operandsCount;
    (*instruction).operand1Type = operandsTypes[cmdIndex].operand1Type;
    (*instruction).operand2Type = operandsTypes[cmdIndex].operand2Type;
    (*instruction).operand3Type = operandsTypes[cmdIndex].operand3Type;
    (*instruction).instructionType = operandsTypes[cmdIndex].instructionType;

    return 1;
}


/*----------------------------------------------------------------------------*/
/*
 * Description: check if the syntax of the operand is valid
 * Input:       Data struct, tag string, command string
 * Output:		-1 if invalid
 */
/*----------------------------------------------------------------------------*/
int getOperandType(Data *data, char *operand,int operandNumber) {

    /* hold the error line to print*/
    char errorLine[300];

    int isEmpty = isEmptyOperand(operand);
    int isImmediate = isImmediateOperand(data, operand,errorLine);
    int isLabel = isTagOperand(operand);
    int isRegister = isRegisterOperand(operand,errorLine);

    if (isEmpty == YES){
        return EMPTY;
    }

    if(isImmediate == YES){
        return IMMEDIATE;
    }

    if(isLabel == YES){
        return LABEL;
    }

    if(isRegister ==YES){
        return  REGISTER;
    }

    if(isImmediate == ERROR){
        printf("[Error] on line %d: operand %dth - %s \n",data->lc, operandNumber,errorLine);
        return ERROR;
    }

    if(isLabel == ERROR){
        return  ERROR;
    }

    if(isRegister == ERROR){
        printf("[Error] on line %d: operand %dth - %s \n",data->lc, operandNumber,errorLine);
        return  ERROR;
    }

    return INVALID;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: check if the operand is empty or not
 * Input:       string operand
 * Output:		1 if empty, 0 if not empty
 */
/*----------------------------------------------------------------------------*/
int isEmptyOperand(char *operand) {
    operand = getCharPtrBeyondSpace(operand);
    if (*operand == '\n' || *operand == EOF) {
        return YES;
    }
    return NO;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: check if the addressing method is register addressing or not
 * Input:       string operand
 * Output:		1 if register, 0 if not register
 */
/*----------------------------------------------------------------------------*/
int isTagOperand(char *operand) {
    char tempOperand[MAX_TAG_LEN];
    char *c;
    strcpy(tempOperand, operand);
    strcat(tempOperand, "\n");

    c = tempOperand;

    /* operand has a $ sign at the start, so its register operand */
    if (*c == '$') {
        return NO;
    }

    if (checkLetters(*c) == 0) {
        return NO;
    }
    c++;
    while (!isspace(*c) && *c != '\n' && *c != EOF) {
        c++;
    }
    return YES;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: check if the addressing method is register addressing or not
 * Input:       string operand
 * Output:		0-31 if register, -1 if not register
 */
/*----------------------------------------------------------------------------*/
int isRegisterOperand(char *operand,char * errorLine) {

    if (*operand != '$') {
        return NO;
    }

    if (getRegisterIndex(operand) == -1) {
        strcpy(errorLine,"malformed register");
        return ERROR;
    };


    return YES;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: check if the addressing method is register addressing or not
 * Input:       string operand
 * Output:		0-31 if register, -1 if not register
 */
/*----------------------------------------------------------------------------*/
int getRegisterIndex(char *operand) {
    char *regArr[] = {"$0", "$1", "$2", "$3", "$4", "$5", "$6", "$7",
                      "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15",
                      "$16", "$17", "$18", "$19", "$20", "$21", "$22", "$23",
                      "$24", "$25", "$26", "$27", "$28", "$29", "$30", "$31",};
    int i;
    int regIndex = -1;
    char temp[80];
    strncpy(temp, operand, strlen(operand));
    temp[strlen(operand)] = '\0';


    for (i = 0; i < NUM_OF_REGISTERS; i++) {
        if (strcmp(temp, regArr[i]) == 0) {
            regIndex = i;
        }
    }
    return regIndex;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: check if the addressing method is immediate or not
 * Input:       Data struct, operand string
 * Output:		1 if immediate, 0 if not immediate
 * NOTE:        might be immediate but invalid, will print out error and still return 1
 */
/*----------------------------------------------------------------------------*/
int isImmediateOperand(Data *data, char *operand,char *errorLine) {
    int num;
    char tempOperand[30];
    char *c;

    deleteSpaces(operand);

    strcpy(tempOperand, operand);
    strcat(tempOperand, "\n");
    c = tempOperand;

    if (*c == '$') {
        return NO;
    }

    /* if there is a + / - sign move one char */
    if (*c == '-' || *c == '+') {
        c++;
    }

    /* not an immediate*/
    if (!isdigit(*c)) {
        return NO;
    }

    /* trying to extract the number  0 means it failed*/
    if ( getImmediateNumber(operand,&num) == 0 ){
        return NO;
    }

    /* checking that the number is in range */
    if ( num < MIN_NUMBER_IMMEDIATE || num > MAX_NUM_IMMEDIATE){
        strcpy(errorLine,"The number exceeds the representation limits of the data type");
        return ERROR;
    }

    /* moving until there are no more digits */
    while (isdigit(*c)) {
        c++;
    }

    /*  an error - there are something extra after the number */
    /*
    if (*c != '\n' && *c != EOF) {
        strcpy(errorLine,"extra characters after number");
        return ERROR;
    }
    */

    return YES;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: parses and validates the commands from input
 * Input:       Data struct, string with tag names (might be null if no tag was found)
 * Output:		1 if successful, 0 otherwise
 */
/*----------------------------------------------------------------------------*/

int getImmediateNumber (char *operand, int * num){
    char tempOperand[80];
    int sign = 1;
    char *c;

    deleteSpaces(operand);

    strcpy(tempOperand, operand);
    strcat(tempOperand, "\n");
    c = tempOperand;


    if (*c == '-') {
        sign = -1;
        c++;
    }
    else if (*c == '+') {
        sign = 1;
        c++;
    }

    if (sscanf(c, "%d", num) == 0) {
        return 0;
    }

    *(num) = *(num) * sign;

    return 1;

}

/*----------------------------------------------------------------------------*/
/*
 * Description:
 * Input:
 * Output:
 */
/*----------------------------------------------------------------------------*/
int createInstruction_R(Data * data,Instruction *newInstruction,InstructionInfo * instructionInfo,
                        int op1Address, int op2Address, int op3Address){

    if (instructionInfo->opcode == 0){
        newInstruction->opcode = instructionInfo->opcode;
        newInstruction->funct = instructionInfo->funct;
        newInstruction->rs = op1Address;
        newInstruction->rt = op2Address;
        newInstruction->rd = op3Address;
        return 1;
    }
    if (instructionInfo->opcode == 1){
        newInstruction->opcode = instructionInfo->opcode;
        newInstruction->funct = instructionInfo->funct;
        newInstruction->rs = op2Address;
        newInstruction->rd = op1Address;
        newInstruction->rt = 0;
        return 1;
    }

    return 0;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: Create I Instruction
 * Input:
 * Output:
 */
/*----------------------------------------------------------------------------*/
int createInstruction_I(Data * data,Instruction *newInstruction,InstructionInfo * instructionInfo,
                        int op1Address, int op2Address, int op3Address){

    if (instructionInfo->opcode>9 && instructionInfo->opcode<14){
        newInstruction->opcode = instructionInfo->opcode;
        newInstruction->funct = 0;
        newInstruction->rs = op1Address;
        newInstruction->rt = op3Address;
        newInstruction->immed = op2Address;
        return 1;
    }
    if (instructionInfo->opcode>13 && instructionInfo->opcode < 19) {
        newInstruction->opcode = instructionInfo->opcode;
        newInstruction->funct = 0;
        newInstruction->rs = op1Address;
        newInstruction->rt = op2Address;
        newInstruction->immed = op3Address!=-1? op3Address - data->ic : 0;
        return 1;
    }
    if (instructionInfo->opcode > 18 && instructionInfo->opcode < 25) {
        newInstruction->opcode = instructionInfo->opcode;
        newInstruction->funct = 0;
        newInstruction->rs = op1Address;
        newInstruction->immed = op2Address;
        newInstruction->rt = op3Address;
        return 1;
    }

    return 0;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: create J instruction
 * Input:
 * Output:
 */
/*----------------------------------------------------------------------------*/
int createInstruction_J(Instruction *newInstruction,InstructionInfo * instructionInfo,
                        int op1Address){

    if (instructionInfo->opcode == 30){
        newInstruction->opcode = instructionInfo->opcode;
        newInstruction->funct = 0;
        newInstruction->reg = op1Address < 32 ? 1 : 0;
        newInstruction->address = op1Address != -1 ? op1Address : -1;
        return 1;
    }
    if (instructionInfo->opcode == 31){
        newInstruction->opcode = instructionInfo->opcode;
        newInstruction->funct = 0;
        newInstruction->reg = 0;
        newInstruction->address = op1Address != -1 ? op1Address : -1;
        return 1;
    }
    if (instructionInfo->opcode == 32){
        newInstruction->opcode = instructionInfo->opcode;
        newInstruction->funct = 0;
        newInstruction->reg = 0;
        newInstruction->address = op1Address != -1 ? op1Address : -1;
        return 1;
    }
    if (instructionInfo->opcode == 63){

        newInstruction->opcode = instructionInfo->opcode;
        newInstruction->funct = 0;
        newInstruction->reg = 0;
        newInstruction->address = 0;
        return 1;
    }

    return 0;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: adds a tag to the tag array inside the Data struct
 * Input:       pointer to Data struct, tag name pointer, address to point tag at
 * Output:	    nothing
 */
/*----------------------------------------------------------------------------*/
void addTag(Data *data, char *tag, int dirAddress, int kind) {
    data->tagArr = realloc(data->tagArr, sizeof(Tag) * (data->tc + 2));
    strcpy(data->tagArr[data->tc].name, tag);
    data->tagArr[data->tc].address = dirAddress;
    data->tagArr[data->tc].kind = kind;
    data->tc++;
}


