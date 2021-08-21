/*
 ====================================================================================
 Module:        secondPassCommandManager
 Description: 	functions that implement the second pass instruction algorithm
 ====================================================================================
 */

#include "header.h"

/*----------------------------------------------------------------------------*/
/*
 * Description: directs the parsing of all the commands during the second pass
 * Input:       Data struct
 * Output:		1 if no errors are found , 0 otherwise
 */
/*----------------------------------------------------------------------------*/
int secondPassCommandManager(Data * data){
    char operand1[MAX_LINE_LEN]= {0};
    char operand2[MAX_LINE_LEN]= {0};
    char operand3[MAX_LINE_LEN]= {0};
    char command[5];
    int commandIndex;
    InstructionInfo info;


    /* get the command */
    sscanf(data->line, "%4s", command);
    data->line+=strlen(command);
    commandIndex = getCommandIndex(command);
    getInstructionInfo(commandIndex,&info);

    eatSpace(data);

    getNOperands(data,info.operandsCount,operand1,operand2,operand3);

    secondPassCommandHandler(data,commandIndex,operand1,operand2,operand3);
    data->ic+=4;

    return 1;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: handles commands with no operands
 * Input:       Data struct, command string
 * Output:		1 if no errors are found, 0 otherwise
 */
/*----------------------------------------------------------------------------*/
int secondPassCommandHandler(Data * data, int commandIndex, char * operand1,char* operand2,char *operand3){

    int instructionIndex = (data->ic - 100) / 4;
    InstructionInfo info;
    getInstructionInfo(commandIndex,&info);

    switch (info.instructionType) {
        case TYPE_R:
            return 1;
        case TYPE_I:
            return instruction_I_Handler(data,commandIndex,&(data->instArr[instructionIndex]),operand1,operand2,operand3);
        case TYPE_J:
            return instruction_J_Handler(data,commandIndex,&(data->instArr[instructionIndex]),operand1);
    }

    return 1;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: handles commands with no operands
 * Input:       Data struct, command string
 * Output:		1 if no errors are found, 0 otherwise
 */
/*----------------------------------------------------------------------------*/
int instruction_I_Handler(Data * data, int commandIndex, Instruction *instruction, char * operand1,char* operand2,char *operand3) {
    InstructionInfo info;
    getInstructionInfo(commandIndex,&info);

    int op1Address = getAddress(data,operand1,info.operand1Type);
    int op2Address = getAddress(data,operand2,info.operand2Type);
    int op3Address = getAddress(data,operand3,info.operand3Type);

    /* The tag is not in the entry list */
    if( op3Address == LABEL_NOT_FOUND ){
        if(getItExternTagAddress(data,operand3) != -1){
            printf("[Error] on line %d: The tag cannot be external\n",data->lc);
            data->containError = TRUE;
            return 0;
        }
        printf("[Error] on line %d: tag does not exist2\n",data->lc);
        data->containError = TRUE;
        return 0;
    }


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
        instruction->immed = op3Address - data->ic;
    }
    if (info.opcode>18 && info.opcode<25) {
        instruction->opcode = info.opcode;
        instruction->funct = 0;
        instruction->rs = op1Address;
        instruction->immed = op2Address;
        instruction->rt = op3Address;
    }
}


/*----------------------------------------------------------------------------*/
/*
 * Description: handles commands with no operands
 * Input:       Data struct, command string
 * Output:		1 if no errors are found, 0 otherwise
 */
/*----------------------------------------------------------------------------*/
int instruction_J_Handler(Data * data, int commandIndex, Instruction *instruction, char * operand1) {

    InstructionInfo info;
    int externalIndex = -2 ;
    getInstructionInfo(commandIndex,&info);
    int op1Address = getAddress(data,operand1,info.operand1Type);

    if (op1Address == -1){
        externalIndex = getItExternTagAddress(data,operand1);
        recordExternal(data,externalIndex);
        op1Address = 0;
    }

    if (externalIndex == -1 && info.opcode != 63){
        printf("[Error] on line %d: tag does not exist\n",data->lc);
        data->containError = TRUE;
        return 0;
    }

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

}


int recordExternal(Data * data, int externalIndex) {
    if ( data->externArr[externalIndex].appearance == 0){
        data->externArr[externalIndex].icArr = (int *)malloc(sizeof(int));
        data->externArr[externalIndex].icArr[data->externArr[externalIndex].appearance] = data->ic;
    } else {
        data->externArr[externalIndex].icArr =
                (int*) realloc((data->externArr[externalIndex]).icArr,sizeof(int) + data->externArr[externalIndex].appearance +1 );
        (data->externArr[externalIndex]).icArr[(data->externArr[externalIndex].appearance)] = data->ic;
    }

    data->externArr[externalIndex].appearance++;

    return 1;

}

