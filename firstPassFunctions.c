#include "header.h"

/*----------------------------------------------------------------------------*/
/*
 * Description: validate a command with two operands
 * Input:       Data struct, tag string, command string
 * Output:		1 if successful, 0 otherwise
 */

/*----------------------------------------------------------------------------*/
int getNOperands(Data *data, int count, char *operand1, char *operand2, char *operand3) {

    /* get the 1st operand*/
    if (count == 0) {
        if (*(data->line) == '\n' || *(data->line) == EOF) {
            return 1;
        }
        printf("[Error] on line %d: illegal arguments - command should have zero operands.\n", data->lc);
        data->containError = TRUE;
        return 0;
    }


    if (count > 0) {
        if (sscanf(data->line, "%30[^,\n ]s", operand1) == 0) {
            printf("[Error] on line %d: first operand required\n", data->lc);
            data->containError = TRUE;
            return 0;
        }
        /* check there's a comma between the two operands */

        data->line += strlen(operand1);
        removeSpaceFromEnd(operand1);
        eatSpace(data);
        deleteSpaces(operand1);
        if (count > 1) {
            if (!((*data->line) == ',')) {
                printf("[Error] on line %d: no comma between operands\n", data->lc);
                data->containError = TRUE;
                return 0;
            }
            data->line++;
            eatSpace(data);
            /* get the 2nd operand*/
            if (sscanf(data->line, "%30[^,\n]", operand2) == 0) {
                printf("[Error] on line %d: second operand required\n", data->lc);
                data->containError = TRUE;
                return 0;
            }


            if (sscanf(data->line, "%30[^,\n]", operand2) == 0) {
                printf("[Error] on line %d: second operand required\n", data->lc);
                data->containError = TRUE;
                return 0;
            }
            /* check there's a comma between the two operands */
            data->line += strlen(operand2);
            removeSpaceFromEnd(operand2);
            eatSpace(data);
            deleteSpaces(operand2);

            if (count > 2) {
                if (!((*data->line) == ',')) {
                    printf("[Error] on line %d: no comma between operands\n", data->lc);
                    data->containError = TRUE;
                    return 0;
                }
                data->line++;
                /* get the 3nd operand*/
                if (sscanf(data->line, "%30s", operand3) == 0) {
                    printf("[Error] on line %d: third operand required\n", data->lc);
                    data->containError = TRUE;
                    return 0;
                }
                eatSpace(data);
                data->line += strlen(operand3) + 1;
                removeSpaceFromEnd(operand3);
                deleteSpaces(operand3);
                /* add a terminating character to the operand string */
            }
        }
    }
    /* shouldn't be any additional character or arguments after the 3nd operand */
    if (isEndOfLine(data->line) == 0) {
        printf("[Error] on line %d: extra characters after %dnd operand \n", data->lc, count);
        data->containError = TRUE;
        return 0;
    }
    return 1;
}


/*----------------------------------------------------------------------------*/
/*
 * Description: validate a command with two operands
 * Input:       Data struct, tag string, command string
 * Output:		1 if successful, 0 otherwise
 */
/*----------------------------------------------------------------------------*/
int checkNOperands(Data *data, InstructionInfo *instructionInfo, int count, char *operand1, char *operand2,
                   char *operand3) {

    int operand1Type;
    int operand2Type;
    int operand3Type;

    if (count == 0) {
        return 1;
    }

    switch (count) {
        case 3:
            operand3Type = getOperandType(data, operand3,3);
        case 2:
            operand2Type = getOperandType(data, operand2,2);
        case 1:
            operand1Type = getOperandType(data, operand1,1);
    }

    /* operand1 check */
    if(operand1Type == ERROR){
        data->containError = TRUE;
        return 0;
    }
    if (operand1Type == INVALID) {
        printf("[Error] on line %d: first operand is invalid \n", data->lc);
        data->containError = TRUE;
        return 0;
    }
    /* operand1 type check */
    if (instructionInfo->operand1Type == REGISTER && operand1Type != REGISTER) {
        printf("[Error] on line %d: first operand is invalid - should be register\n", data->lc);
        data->containError = TRUE;
        return 0;
    }
    /* operand1 type check */
    if (instructionInfo->operand1Type == IMMEDIATE && operand1Type != IMMEDIATE) {
        printf("[Error] on line %d: first operand is invalid - should be immediate number\n", data->lc);
        data->containError = TRUE;
        return 0;
    }
    /* operand1 type check */
    if (instructionInfo->operand1Type == LABEL && operand1Type != LABEL) {
        printf("[Error] on line %d: first operand is invalid - should be label\n", data->lc);
        data->containError = TRUE;
        return 0;
    }

    /* operand1 type check */
    if (instructionInfo->operand1Type == REGISTER_LABEL && operand1Type != REGISTER && operand1Type != LABEL) {
        printf("[Error] on line %d: first operand is invalid - should be label or register\n", data->lc);
        data->containError = TRUE;
        return 0;
    }

    /* operand2 check */
    if(count >= 2 && operand2Type == ERROR){
        data->containError = TRUE;
        return 0;
    }

    if (count >= 2 && operand2Type == INVALID) {
        printf("[Error] on line %d: second operand is invalid \n", data->lc);
        data->containError = TRUE;
        return 0;
    }

    if (count >= 2 && instructionInfo->operand2Type == REGISTER && operand2Type != REGISTER) {
        printf("[Error] on line %d: second operand is invalid - should be register\n", data->lc);
        data->containError = TRUE;
        return 0;
    }

    if (count >= 2 && instructionInfo->operand2Type == LABEL && operand2Type != LABEL) {
        printf("[Error] on line %d: second operand is invalid - should be label\n", data->lc);
        data->containError = TRUE;
        return 0;
    }

    if (count >= 2 && instructionInfo->operand2Type == REGISTER_LABEL && operand2Type != REGISTER &&
        operand2Type != LABEL) {
        printf("[Error] on line %d: second operand is invalid - should be label or register\n", data->lc);
        data->containError = TRUE;
        return 0;
    }

    if (count >= 2 && instructionInfo->operand2Type == IMMEDIATE && operand2Type != IMMEDIATE) {
        printf("[Error] on line %d: second operand is invalid - should be immediate number\n", data->lc);
        data->containError = TRUE;
        return 0;
    }

    if(count >= 3 && operand3Type == ERROR){
        data->containError = TRUE;
        return 0;
    }


    if (count >= 3 && operand3Type == INVALID ) {
        printf("[Error] on line %d: third operand is invalid \n", data->lc);
        data->containError = TRUE;
        return 0;
    }

    if (count >= 3 && instructionInfo->operand3Type == REGISTER && operand3Type != REGISTER) {
        printf("[Error] on line %d: third operand is invalid \n", data->lc);
        data->containError = TRUE;
        return 0;
    }

    if (count >= 3 && instructionInfo->operand3Type == IMMEDIATE && operand3Type != IMMEDIATE) {
        printf("[Error] on line %d: third operand is invalid - should be immediate number\n", data->lc);
        data->containError = TRUE;
        return 0;
    }

    if (count >= 3 && instructionInfo->operand3Type == LABEL && operand3Type != LABEL) {
        printf("[Error] on line %d: third operand is invalid - should be label\n", data->lc);
        data->containError = TRUE;
        return 0;
    }

    if (count >= 3 && instructionInfo->operand3Type == REGISTER_LABEL && operand3Type != REGISTER &&
        operand3Type != LABEL) {
        printf("[Error] on line %d: third operand is invalid - should be label or register\n", data->lc);
        data->containError = TRUE;
        return 0;
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
int getTagAddress(Data *data, char *operand) {
    int tagAddress = LABEL_NOT_FOUND;
    int index;

    for (index = 0; index < data->tc; index++) {
        if (strcmp(data->tagArr[index].name, operand) == 0) {
            tagAddress = data->tagArr[index].address;
            return tagAddress;
        }
    }
    return tagAddress;
}

/*----------------------------------------------------------------------------*/
/*
 * Description:
 * Input:
 * Output:
 */
/*----------------------------------------------------------------------------*/
int getItExternTagAddress(Data *data,char *operand) {
    int index;
    for (index = 0; index < data->exc; index++) {
        if (strcmp(data->externArr[index].name, operand) == 0) {
           return index;
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

int getAddress(Data *data, char *operand, int type) {
    int res;

    switch (type) {
        case REGISTER:
            res = getRegisterIndex(operand);
            return res;
        case LABEL:
            res = getTagAddress(data, operand);
            return res;
        case IMMEDIATE:
            sscanf(operand, "%d", &res);
            return res;
        case REGISTER_LABEL:
            res = getRegisterIndex(operand);
            if (res == -1) {
                res = getTagAddress(data, operand);
            }
            return res;
        case OPERAND_NOT_USED:
            return 0;
    }

    return 0;
}


/*----------------------------------------------------------------------------*/
/*
 * Description: validate a command with two operands
 * Input:       Data struct, tag string, command string
 * Output:		1 if successful, 0 otherwise
 */
/*----------------------------------------------------------------------------*/
int AddInstruction(Data *data, int commandIndex, char *operand1, char *operand2, char *operand3) {
    Instruction *instruction;
    InstructionInfo info;
    int currentIC;
    int op1Address;
    int op2Address;
    int op3Address;

    getInstructionInfo(commandIndex, &info);

    op1Address = getAddress(data, operand1, info.operand1Type);
    op2Address = getAddress(data, operand2, info.operand2Type);
    op3Address = getAddress(data, operand3, info.operand3Type);

    currentIC = ((data->ic - 100) / 4);
    data->instArr = realloc(data->instArr, sizeof(Instruction) * (currentIC + 1));

    instruction = &(data->instArr[currentIC]);
    instruction->ic = data->ic;
    instruction->cmdIndex = commandIndex;

    switch (info.instructionType) {
        case TYPE_R:
            return createInstruction_R(data, instruction, &info, op1Address, op2Address, op3Address);
        case TYPE_I:
            if( op3Address == LABEL_NOT_FOUND ) {
                if (getItExternTagAddress(data, operand3) != -1) {
                    printf("[Error] on line %d: The tag cannot be external2\n", data->lc);
                    data->containError = TRUE;
                    return 0;
                }
            }
            return createInstruction_I(data, instruction, &info, op1Address, op2Address, op3Address);
        case TYPE_J:
            return createInstruction_J(instruction, &info, op1Address);
    }

    return 0;
}


