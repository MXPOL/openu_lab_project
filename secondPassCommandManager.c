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
    char operand1[MAX_TAG_LEN]= {0};
    char operand2[MAX_TAG_LEN]= {0};
    char operand3[MAX_TAG_LEN]= {0};
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

    /*
    switch (info.operandsCount) {
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
    }
     */

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
            break;
        case TYPE_I:
            return instruction_I_Handler(data,commandIndex,&(data->instArr[instructionIndex]),operand1,operand2,operand3);
        case TYPE_J:
            return instruction_J_Handler(data,commandIndex,&(data->instArr[instructionIndex]),operand1,operand2,operand3);
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

    if( op3Address == -1 ){
            printf("[Error] on line %d: tag does not exist\n",data->lc);
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
int instruction_J_Handler(Data * data, int commandIndex, Instruction *instruction, char * operand1,char* operand2,char *operand3) {
    int i;
    InstructionInfo info;
    getInstructionInfo(commandIndex,&info);
    int op1Address = getAddress(data,operand1,info.operand1Type);
    int op2Address = getAddress(data,operand2,info.operand2Type);
    int op3Address = getAddress(data,operand3,info.operand3Type);

    for (i = 0; i < (data->exc); i++) {
        if (strcmp(data->externArr[i].name, operand1) == 0) {
            op1Address = 0;
        }
    }

    if (op1Address == -1 &&info.opcode != 63 ){
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




