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

    if (sscanf(data->line, "%4s", command) == 0) {
        printf("[Error] on line %d:- command error invalid\n", data->lc);
        data->containError = TRUE;
        return 0;
    }
    commandIndex = getCommandIndex(command);

    if (commandIndex == -1) {
        printf("[Error] on line %d: command error not defined\n", data->lc);
        data->containError = TRUE;
        return 0;
    }

    data->line += strlen(command);

    if (!isspace(*(data->line))) {
        printf("[Error] on line %d:- extra characters or no space after command name\n", data->lc);
        data->containError = TRUE;
        return 0;
    }

    eatSpace(data);

    getInstructionInfo(commandIndex, &instructionInfo);
    operandNum = instructionInfo.operandsCount;

    if (operandNum == 0 && getNOperands(data, 0, op1, op2, op3) == 1) {

        if (thereIsATag(tag)) {
            addTag(data, tag, data->ic, CODE);
        }

        AddInstruction(data, commandIndex, op1, op2, op3);
        data->ic += 4;
        return 1;
    }

    if (operandNum == 1 && getNOperands(data, 1, op1, op2, op3) == 1 &&
        checkNOperands(data, 1, op1, op2, op3) == 1) {

        if (thereIsATag(tag)) {
            addTag(data, tag, data->ic, CODE);
        }

        AddInstruction(data, commandIndex, op1, op2, op3);
        data->ic += 4;
        return 1;
    }

    if (operandNum == 2 && getNOperands(data, 2, op1, op2, op3) == 1 &&
        checkNOperands(data, 2, op1, op2, op3) == 1) {

        if (thereIsATag(tag)) {
            addTag(data, tag, data->ic, CODE);
        }
        AddInstruction(data, commandIndex, op1, op2, op3);
        data->ic += 4;
        return 1;
    }

    if (operandNum == 3 && getNOperands(data, 3, op1, op2, op3) == 1 &&
        checkNOperands(data, 3, op1, op2, op3) == 1) {

        if (thereIsATag(tag)) {
            addTag(data, tag, data->ic, CODE);
        }
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
 * Description:
 * Input:
 * Output:
 */
/*----------------------------------------------------------------------------*/
int getInstructionInfo(int cmdIndex, InstructionInfo *instruction) {
    int i;
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
            {63, FUNCT_NOT_USED, 0, OPERAND_NOT_USED, OPERAND_NOT_USED, OPERAND_NOT_USED, TYPE_J},
    };

    if (cmdIndex < 0 || cmdIndex > NUM_OF_COMMANDS) {
        return -1;
    }

    (*instruction).opcode = operandsTypes[cmdIndex].opcode;
    (*instruction).funct = operandsTypes[cmdIndex].funct;
    (*instruction).operandsCount = operandsTypes[cmdIndex].operandsCount;
    (*instruction).operand1Type = operandsTypes[cmdIndex].operand1Type;
    (*instruction).operand2Type = operandsTypes[cmdIndex].operand2Type;
    (*instruction).operand3Type = operandsTypes[cmdIndex].operand3Type;
    (*instruction).instructionType = operandsTypes[cmdIndex].instructionType;

    return 0;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: validate a command with zero operands
 * Input:       Data struct, tag string
 * Output:		1 if successful, 0 otherwise
 */
/*----------------------------------------------------------------------------*/
int checkZeroOperands(Data *data, char *tag, int kind) {
    if (*(data->line) == '\n' || *(data->line) == EOF) {
        if (tag != NULL && *tag != '\0') {
            addTag(data, tag, data->ic, CODE);
        }
        data->ic += 4;
        return 1;
    }
    printf("[Error] on line %d: illegal arguments - command should have zero operands.\n", data->lc);
    data->containError = TRUE;
    return 0;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: validate a command with one operand
 * Input:       Data struct, tag string
 * Output:		1 if successful, 0 otherwise
 */
/*----------------------------------------------------------------------------*/
int checkOneOperands(Data *data, char *tag, int commandIndex) {
    char operand[MAX_TAG_LEN];
    int operandType;
    /* 30s = 30 chars after the command */
    // TODO check what to do about %30s, should i change it?
    if (sscanf(data->line, "%30s", operand) == 0) {
        printf("[Error] on line %d: no operands given - one operand required\n", data->lc);
        data->containError = TRUE;
        return 0;
    }
    data->line += strlen(operand);
    eatSpace(data);
    /* shouldn't be any additional character or arguments after the operand */
    if (isEndOfLine(data->line) == 0) {
        printf("[Error] on line %d: extra argument or characters after the operand \n", data->lc);
        data->containError = TRUE;
        return 0;
    }
    operandType = isOperandValidSyntax(data, operand);
    if (operandType == -1) {
        printf("[Error] on line %d: operand is invalid \n", data->lc);
        data->containError = TRUE;
        return 0;
    }

    /* what happens when IC and DC are the same? how does it differentiate address? is DC limited to 100? */
    if (tag != NULL && *tag != '\0') {
        addTag(data, tag, data->ic, commandIndex);
    }
    /*addOneOperandCommand(data,commandIndex,operand,66);*/
    /* there is one more instruction to allocate memory for at end of first pass */
    data->ic += 4;


//    TODO remove this wc++ in the end
    /* there is one more extra word to allocate memory for at end of first pass */
    data->wc++;
    printf("OneOperands - 1:%d \n", operandType);
    return 1;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: check if the syntax of the operand is valid
 * Input:       Data struct, tag string, command string
 * Output:		-1 if invalid
 */
/*----------------------------------------------------------------------------*/
int isOperandValidSyntax(Data *data, char *operand) {
    int Address;
    Address = getAddressingMethod(data, operand);
    if (Address != -1) {
        return Address;
    }
    return -1;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: get the addressing method of the operand and check validity
 * Input:       Data struct, operand string
 * Output:		int addressing method code, -1 if invalid
 * NOTE:        might print out errors if invalid
 */
/*----------------------------------------------------------------------------*/
int getAddressingMethod(Data *data, char *operand) {
    if (isEmptyOperand(operand) == 1) {
        return EMPTY_OPERAND;
    }
    if (isImmediateOperand(data, operand) == 1) {
        return IMMEDIATE_OPERAND;
    } else if (isTagOperand(operand) == 1) {
        return TAG_OPERAND;
    } else if (isRegisterOperand(operand) != -1) {
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
int isEmptyOperand(char *operand) {
    operand = getCharPtrBeyondSpace(operand);
    if (*operand == '\n' || *operand == EOF) {
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
int isTagOperand(char *operand) {
    char tempOperand[MAX_TAG_LEN];
    char *c;
    strcpy(tempOperand, operand);
    strcat(tempOperand, "\n");

    c = tempOperand;

    /* operand has a $ sign at the start, so its register operand */
    if (*c == '$') {
        return 0;
    }

    if (checkLetters(*c) == 0) {
        return 0;
    }
    c++;
    while (!isspace(*c) && *c != '\n' && *c != EOF) {
        if (checkLetterOrNumber(*c) == 0) {
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
 * Output:		0-31 if register, -1 if not register
 */
/*----------------------------------------------------------------------------*/
int isRegisterOperand(char *operand) {
    char *regArr[] = {"$0", "$1", "$2", "$3", "$4", "$5", "$6", "$7",
                      "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15",
                      "$16", "$17", "$18", "$19", "$20", "$21", "$22", "$23",
                      "$24", "$25", "$26", "$27", "$28", "$29", "$30", "$31",};
    int i;
    char temp[3];
    strncpy(temp, operand, 2);
    temp[2] = '\0';
    for (i = 0; i < NUM_OF_REGISTERS; i++) {
        if (strcmp(temp, regArr[i]) == 0) {
            return i;
        }
    }
    return -1;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: handles commands with no operands
 * Input:       Data struct, command string, operand string, int addressing method
 * Output:		1 if no errors are found, 0 otherwise
 */
/*----------------------------------------------------------------------------*/
int addOneOperandCommand(Data *data, char *tag, int commandIndex) {
    char operand[MAX_TAG_LEN];
    int operandType;
    /* 30s = 30 chars after the command */
    // TODO check what to do about %30s, should i change it?
    if (sscanf(data->line, "%30s", operand) == 0) {
        printf("[Error] on line %d: no operands given - one operand required\n", data->lc);
        data->containError = TRUE;
        return 0;
    }
    data->line += strlen(operand);
    eatSpace(data);
    /* shouldn't be any additional character or arguments after the operand */
    if (isEndOfLine(data->line) == 0) {
        printf("[Error] on line %d: extra argument or characters after the operand \n", data->lc);
        data->containError = TRUE;
        return 0;
    }
    operandType = isOperandValidSyntax(data, operand);
    if (operandType == -1) {
        printf("[Error] on line %d: operand is invalid \n", data->lc);
        data->containError = TRUE;
        return 0;
    }

    /* what happens when IC and DC are the same? how does it differentiate address? is DC limited to 100? */
    if (tag != NULL && *tag != '\0') {
        addTag(data, tag, data->ic, commandIndex);
    }
    /*addOneOperandCommand(data,commandIndex,operand,66);*/
    /* there is one more instruction to allocate memory for at end of first pass */
    data->ic += 4;


//    TODO remove this wc++ in the end
    /* there is one more extra word to allocate memory for at end of first pass */
    data->wc++;
    printf("OneOperands - 1:%d \n", operandType);
    return 1;

    return 1;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: validate a command with one operand
 * Input:       Data struct, tag string
 * Output:		1 if successful, 0 otherwise
 */
/*----------------------------------------------------------------------------*/
char *getOneOperands(Data *data, char *tag, int kind) {
    char operand[MAX_TAG_LEN];
    /* 30s = 30 chars after the command */
    if (sscanf(data->line, "%30s", operand) == 0) {
        printf("[Error] on line %d: no operands given - one operand required\n", data->lc);
        data->containError = TRUE;
        return 0;
    }
    data->line += strlen(operand);
    eatSpace(data);
    /* shouldn't be any additional character or arguments after the operand */
    if (isEndOfLine(data->line) == 0) {
        printf("[Error] on line %d: extra argument or characters after the operand \n", data->lc);
        data->containError = TRUE;
        return 0;
    }
    if (isOperandValidSyntax(data, operand) == 0) {
        printf("[Error] on line %d: operand is invalid \n", data->lc);
        data->containError = TRUE;
        return 0;
    }
    /* what happens when IC and DC are the same? how does it differentiate address? is DC limited to 100? */
    if (tag != NULL && *tag != '\0') {
        addTag(data, tag, data->dc, kind);
    }
    /*addOneOperandCommand(data,"la",operand,66);*/
    /* there is one more instruction to allocate memory for at end of first pass */
    data->ic += 4;
    /* there is one more extra word to allocate memory for at end of first pass */
    data->wc++;
    return 1;
}


/*----------------------------------------------------------------------------*/
/*
 * Description: validate a command with two operands
 * Input:       Data struct, tag string, command string
 * Output:		1 if successful, 0 otherwise
 */
/*----------------------------------------------------------------------------*/
int checkTwoOperands(Data *data, char *tag, char *command) {
    char operand1[MAX_TAG_LEN];
    char operand2[MAX_TAG_LEN];
    int operand1Type;
    int operand2Type;

    /* get the 1st operand*/
    if (sscanf(data->line, "%30[^,\n ]s", operand1) == 0) {
        printf("[Error] on line %d: no operands given - two operands required\n", data->lc);
        data->containError = TRUE;
        return 0;
    }
    /* check there's a comma between the two operands */
    data->line += strlen(operand1);
    eatSpace(data);
    if (!(*data->line) == ',') {
        printf("[Error] on line %d: no comma between operands\n", data->lc);
        data->containError = TRUE;
        return 0;
    }
    data->line++;

    /* get the 2nd operand*/
    if (sscanf(data->line, "%30s", operand2) == 0) {
        printf("[Error] on line %d: second operand required\n", data->lc);
        data->containError = TRUE;
        return 0;
    }
    data->line += strlen(operand2) + 1;

    /* add a terminating character to the operand string */
    eatSpace(data);

    /* shouldn't be any additional character or arguments after the 2nd operand */
    if (isEndOfLine(data->line) == 0) {
        printf("[Error] on line %d: extra characters after 2nd operand \n", data->lc);
        data->containError = TRUE;
        return 0;
    }
    operand1Type = isOperandValidSyntax(data, operand1);
    /* check the first operand is valid syntactically */
    if (operand1Type == -1) {
        printf("[Error] on line %d: first operand is invalid \n", data->lc);
        data->containError = TRUE;
        return 0;

    }
    operand2Type = isOperandValidSyntax(data, operand2);
    /* check the second operand is valid syntactically */
    if (operand2Type == -1) {
        printf("[Error] on line %d: second operand is invalid \n", data->lc);
        data->containError = TRUE;
        return 0;

    }
    /* add the address under the tag if it was given */
    if (tag != NULL && *tag != '\0') {
        /*addTag(data,tag,data->dc);*/
        addTag(data, tag, data->ic, CODE);
    }
    /* there is one more instruction to allocate memory for at end of first pass */
    data->ic += 4;

    // TODO remove this wc++ in the end
    /* there are two more extra memory words to allocate memory for at end of first pass */
    data->wc += 2;
    printf("TwoOperands - 1:%d 2:%d  \n", operand1Type, operand2Type);

    return 1;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: validate a command with two operands
 * Input:       Data struct, tag string, command string
 * Output:		1 if successful, 0 otherwise
 */
/*----------------------------------------------------------------------------*/
int checkThreeOperands(Data *data, char *tag, char *command) {
    char operand1[MAX_TAG_LEN];
    char operand2[MAX_TAG_LEN];
    char operand3[MAX_TAG_LEN];
    int operand1Type;
    int operand2Type;
    int operand3Type;

    /* get the 1st operand*/
    if (sscanf(data->line, "%30[^,\n ]s", operand1) == 0) {
        printf("[Error] on line %d: no operands given - two operands required\n", data->lc);
        data->containError = TRUE;
        return 0;
    }
    /* check there's a comma between the two operands */
    data->line += strlen(operand1);
    eatSpace(data);
    if (!(*data->line) == ',') {
        printf("[Error] on line %d: no comma between operands\n", data->lc);
        data->containError = TRUE;
        return 0;
    }
    data->line++;
    /* get the 2nd operand*/
    if (sscanf(data->line, "%30[^,\n ]s", operand2) == 0) {
        printf("[Error] on line %d: second operand required\n", data->lc);
        data->containError = TRUE;
        return 0;
    }
    /* check there's a comma between the two operands */
    data->line += strlen(operand2);
    eatSpace(data);
    if (!(*data->line) == ',') {
        printf("[Error] on line %d: no comma between operands\n", data->lc);
        data->containError = TRUE;
        return 0;
    }
    data->line++;
    /* get the 3nd operand*/
    if (sscanf(data->line, "%30[^,\n ]s", operand3) == 0) {
        printf("[Error] on line %d: third operand required\n", data->lc);
        data->containError = TRUE;
        return 0;
    }
    data->line += strlen(operand3) + 1;
    /* add a terminating character to the operand string */
    eatSpace(data);

    /* shouldn't be any additional character or arguments after the 2nd operand */
    if (isEndOfLine(data->line) == 0) {
        printf("[Error] on line %d: extra characters after 2nd operand \n", data->lc);
        data->containError = TRUE;
        return 0;
    }
    /* check the first operand is valid syntactically */
    operand1Type = isOperandValidSyntax(data, operand1);
    if (operand1Type == -1) {
        printf("[Error] on line %d: first operand is invalid \n", data->lc);
        data->containError = TRUE;
        return 0;

    }
    /* check the second operand is valid syntactically */
    operand2Type = isOperandValidSyntax(data, operand2);
    if (operand2Type == -1) {
        printf("[Error] on line %d: second operand is invalid \n", data->lc);
        data->containError = TRUE;
        return 0;
    }

    /* check the third operand is valid syntactically */
    operand3Type = isOperandValidSyntax(data, operand3);
    if (operand3Type == -1) {
        printf("[Error] on line %d: third operand is invalid \n", data->lc);
        data->containError = TRUE;
        return 0;
    }

    /* add the address under the tag if it was given */
    if (tag != NULL && *tag != '\0') {
        /*addTag(data,tag,data->dc);*/
        addTag(data, tag, data->ic, CODE);
    }
    /* there is one more instruction to allocate memory for at end of first pass */
    data->ic += 4;
    /* there are two more extra memory words to allocate memory for at end of first pass */
    data->wc += 2;
    printf("ThreeOperands - 1:%d 2:%d 3:%d  \n", operand1Type, operand2Type, operand3Type);
    return 1;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: check if the addressing method is immediate or not
 * Input:       Data struct, operand string
 * Output:		1 if immediate, 0 if not immediate
 * NOTE:        might be immediate but invalid, will print out error and still return 1
 */
/*----------------------------------------------------------------------------*/
int isImmediateOperand(Data *data, char *operand) {
    int num;
    int isItNegative = FALSE;
    char tempOperand[30];
    char *c;

    operand = deblank(operand);

    strcpy(tempOperand, operand);
    strcat(tempOperand, "\n");
    c = tempOperand;


    /* operand hasn't got a #  at the start*/
    /* it's  not immediate addressing method */

    /* register */
    if (*c == '$') {
        return 0;
    }

    if (isalpha(*c) == 1) {
        return 0;
    }

    if (*c == '-') {
        isItNegative = TRUE;
        c++;
    }

    if (*c == '+') {
        isItNegative = FALSE;
        c++;
    }


    if (sscanf(c, "%d", &num) == 0) {
        printf("[Error] on line %d:- malformed immediate number - no number after  + / - sign\n", data->lc);
        data->containError = TRUE;
        return 1;
    }

    while (isdigit(*c)) {
        c++;
    }

    if (*c != '\n' && *c != EOF) {
        printf("[Error] on line %d:- malformed immediate addressing - extra characters after number\n", data->lc);
        data->containError = TRUE;
        return 1;
    }

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
 * Description:
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