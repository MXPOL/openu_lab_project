#include "header.h"

/*----------------------------------------------------------------------------*/
/*
 * Description: validate a command with two operands
 * Input:       Data struct, tag string, command string
 * Output:		1 if successful, 0 otherwise
 */
int getLabelAddress(const Data *data, const char *operand);

/*----------------------------------------------------------------------------*/
int getNOperands(Data * data,int count, char * operand1,char* operand2,char *operand3){

    /* get the 1st operand*/
    if (count == 0){
        if(*(data->line)=='\n' || *(data->line)==EOF){
            return 1;
        }
        printf("[Error] on line %d: illegal arguments - command should have zero operands.\n", data->lc);
        data->containError=TRUE;
        return 0;
    }


    if (count > 0) {
        if (sscanf(data->line, "%30[^,\n ]s", operand1) == 0) {
            printf("[Error] on line %d: no operands given - two operands required\n", data->lc);
            data->containError = TRUE;
            return 0;
        }
        /* check there's a comma between the two operands */

        data->line += strlen(operand1);
        eatSpace(data);
        operand1 = deblank(operand1);
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
            /* check there's a comma between the two operands */
            data->line += strlen(operand2);
            eatSpace(data);
            operand2 = deblank(operand2);
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
                operand3 = deblank(operand3);
                /* add a terminating character to the operand string */
            }
        }
    }
    /* shouldn't be any additional character or arguments after the 3nd operand */
    if(isEndOfLine(data->line) == 0){
        printf("[Error] on line %d: extra characters after %dnd operand \n", data->lc, count);
        data->containError=TRUE;
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
int checkNOperands(Data * data,int count, char * operand1,char* operand2,char *operand3){

    int operand1Type;
    int operand2Type;
    int operand3Type;


//    TODO maybe check someting more
    if (count == 0){
        return 1;
    }

    /* check the first operand is valid syntactically */
    operand1Type = isOperandValidSyntax(data,operand1);
    if (operand1Type == -1){
        printf("[Error] on line %d: first operand is invalid \n", data->lc);
        data->containError=TRUE;
        return 0;

    }
    if (count > 1) {
        /* check the second operand is valid syntactically */
        operand2Type = isOperandValidSyntax(data, operand2);
        if (operand2Type == -1) {
            printf("[Error] on line %d: second operand is invalid \n", data->lc);
            data->containError = TRUE;
            return 0;
        }
        if (count > 2) {


            /* check the third operand is valid syntactically */
            operand3Type = isOperandValidSyntax(data, operand3);
            if (operand3Type == -1) {
                printf("[Error] on line %d: third operand is invalid \n", data->lc);
                data->containError = TRUE;
                return 0;
            }
        }
    }

    return 1;
}

int getLabelAddress(const Data *data, const char *operand) {
    int labelAdress = -1 ;
    int index;
    for (index = 0; index < data->tc; index++){
        if (strcmp(data->tagArr[index].name,operand) == 0){
            labelAdress = data->tagArr[index].address;
        }
    }
    return labelAdress;
}

int getAddress(Data * data, char * operand, int type){
    int res;
    switch (type){
        case REGISTER:
            res = isRegisterOperand(operand);
            return res;
        case LABEL:
            res = getLabelAddress(data,operand);
            return res;
        case IMMEDIATE:
            sscanf(operand, "%d", &res);
            return res;
        case REGISTER_LABEL:
            res = isRegisterOperand(operand);
            if (res == -1 ){
                res = getLabelAddress(data,operand);
            }
            return res;
        case OPERAND_NOT_USED:
            return -1;
        default:
            return -1;
    }
}


/*----------------------------------------------------------------------------*/
/*
 * Description: validate a command with two operands
 * Input:       Data struct, tag string, command string
 * Output:		1 if successful, 0 otherwise
 */
/*----------------------------------------------------------------------------*/
int AddInstruction(Data * data,int commandIndex, char * operand1,char* operand2,char *operand3){
    Instruction *instruction;
    InstructionInfo info;
    int currentIC;

    getInstructionInfo(commandIndex,&info);

    int op1Address = getAddress(data,operand1,info.operand1Type);
    int op2Address = getAddress(data,operand2,info.operand2Type);
    int op3Address = getAddress(data,operand3,info.operand3Type);

    currentIC = ((data->ic - 100) / 4);
    data->instArr = realloc(data->instArr,sizeof (Instruction)*(currentIC+1));

    instruction = &(data->instArr[currentIC]);
    instruction->ic = data->ic;
    instruction -> cmdIndex = commandIndex;

    switch (info.instructionType) {
        case TYPE_R:
            /*
            if (info.opcode == 0){
                instruction->opcode = info.opcode;
                instruction->funct = info.funct;
                instruction->rs = op1Address;
                instruction->rt = op2Address;
                instruction->rd = op3Address;
            }
            if (info.opcode == 1){
                instruction->opcode = info.opcode;
                instruction->funct = info.funct;
                instruction->rs = op2Address;
                instruction->rd = op1Address;
                instruction->rt = 0;
            }
*/
            createInstruction_R(data,instruction,&info,op1Address,op2Address,op3Address);
            break;
        case TYPE_I:
            /*
            if (info.opcode>9 && info.opcode<14){
                instruction->opcode = info.opcode;
                instruction->funct = 0;
                instruction->rs = op1Address;
                instruction->rt = op3Address;
                instruction->immed = op2Address;
            }
            if (info.opcode>13 && info.opcode<19) {
                instruction->opcode = info.opcode;
                instruction->funct = 0;
                instruction->rs = op1Address;
                instruction->rt = op2Address;
                instruction->immed = op3Address!=-1? op3Address - data->ic : 0;
            }
            if (info.opcode>18 && info.opcode<25) {
                instruction->opcode = info.opcode;
                instruction->funct = 0;
                instruction->rs = op1Address;
                instruction->immed = op2Address;
                instruction->rt = op3Address;
            }
             */
            return createInstruction_I(data,instruction,&info,op1Address,op2Address,op3Address);
            break;
        case TYPE_J:
            if (info.opcode == 30){
                instruction->opcode = info.opcode;
                instruction->funct = 0;
                instruction->reg = op1Address < 32 ? 1 : 0;
                instruction->address = op1Address != -1 ? op1Address : -1;
            }
            if (info.opcode == 31){
                instruction->opcode = info.opcode;
                instruction->funct = 0;
                instruction->reg = 0;
                instruction->address = op1Address != -1 ? op1Address : -1;
            }
            if (info.opcode == 32){
                instruction->opcode = info.opcode;
                instruction->funct = 0;
                instruction->reg = 0;
                instruction->address = op1Address != -1 ? op1Address : -1;
            }
            if (info.opcode == 63){

                instruction->opcode = info.opcode;
                instruction->funct = 0;
                instruction->reg = 0;
                instruction->address = 0;
            }
            break;
    }

}


