/*
 ====================================================================================
 Module:        outputManager
 Description: 	generates the output file and writes the code into them
 ====================================================================================
 */


#include "header.h"
#define BASE32 32
#define MAX_NAME_LEN 80

char *decimal_to_binary2(int number, int bits)
{
    int c, d, count;
    char *pointer;
    int useFlag = number<0 ? 1 : 0;
    int flag = 0;

    count = 0;


    number = number < 0 ? number *-1 : number;
    pointer = (char*)malloc(bits);
    if ( pointer == NULL )
        exit(EXIT_FAILURE);


    for ( c = bits-1 ; c >= 0 ; c-- )
    {
        d = number >> c;
        if ( d & 1 )
            *(pointer+count) = '1';
        else
            *(pointer+count) = '0';
        count++;
    }

    for ( c = bits-1 ; c >= 0 ; c-- )
    {
        if (flag == 1 ){
            *(pointer+c) = *(pointer+c) == '1' ? '0' : '1';
        }
        if ( useFlag == 1 && *(pointer+c) == '1' ){
            flag = 1;
        }

    }


    return pointer;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: generates the output file and writes the code into them
 * Input:       Data struct
 * Output:		nothing
 */
/*----------------------------------------------------------------------------*/
void outputManager(Data * data, char * filename){
    char outputFileName[MAX_NAME_LEN];
    char temp[33];
    int instructionIndex;
    int currentInstruction;
    int dataIndex=0;
    int wordIndex=0;
    char * buff = (char *) malloc(1);
    char * p;
    int num;
    int j = 0;
    int i;
    char *hex;
    Instruction * inst;
    InstructionInfo info;
    char *finalLine = (char *) malloc(1);
    int totalInstruction = (data -> ic);
    strcpy(outputFileName,filename);
    strcat(outputFileName, ".ob");


    /* write the instruction and data lengths to file */
    writeLengthsToFile(data, outputFileName);

    /* write all the instructions to file */
    for(instructionIndex=100;instructionIndex < totalInstruction;instructionIndex = instructionIndex + 4) {
        currentInstruction = (instructionIndex - 100) / 4;
        inst = &(data -> instArr[currentInstruction]);
        getInstructionInfo(inst->cmdIndex,&info);
        printf("%d : ",instructionIndex);
        switch (info.instructionType) {
            case TYPE_R:
                create_R_Instruction(data,outputFileName,inst);
                break;
            case TYPE_I:
                create_I_Instruction(data,outputFileName,inst);
                break;
            case TYPE_J:
                create_J_Instruction(data,outputFileName,inst);
                break;
        }
    }
    j = 0;

    for (dataIndex = 0 ; dataIndex < data->dc  ; dataIndex ++) {
        switch (data->directiveArr2[dataIndex].kind) {
            case DATA_DB:
            case DATA_ASCIZ:
                p = decimal_to_binary(data->directiveArr2[dataIndex].data, 8);
                hex = littleEn(p, strlen(p));
                buff = (char *) realloc(buff, strlen(buff) + strlen(p) + 1);
                strcat(buff, p);
                free(p);
                break;
            case DATA_DH:
                p = decimal_to_binary(data->directiveArr2[dataIndex].data, 16);
                buff = (char *) realloc(buff, strlen(buff) + strlen(p) + 1);
                hex = fix16(p);
                strcat(buff, hex);
                free(p);
                break;
            case DATA_DW:
                p = decimal_to_binary(data->directiveArr2[dataIndex].data, 32);
                buff = (char *) realloc(buff, strlen(buff) + strlen(p) + 1);
                hex = fix32(p);
                strcat(buff, hex);
                free(p);
                break;
        }


    }

    while ( *buff ){
        memcpy( temp, buff , 32);
        temp[33] = '\0';
        hex = binaryToHex(temp, strlen(temp) - 1);
        printf("%d temp :%s %s\n", data->ic, temp, hex,strlen(temp));
        data->ic +=4;
        buff += 32;

    }

}

/*----------------------------------------------------------------------------*/
/*
 * Description: generates the output file for an instruction with no extra words
 * Input:       Data struct, filename string
 * Output:		nothing
 */
/*----------------------------------------------------------------------------*/
void createOutputZeroExtra(Data * data, char * filename, int instructionIndex){
//    unsigned long int output=0;
//    char * base32output;
//    output+=decimalToBinary(data->instArr[instructionIndex].e_r_a);
//    output+=decimalToBinary(data->instArr[instructionIndex].destination_addressing)*100;
//    output+=decimalToBinary(data->instArr[instructionIndex].source_addressing)*10000;
//    output+=decimalToBinary(data->instArr[instructionIndex].opcode)*1000000;
//    output+=decimalToBinary(data->instArr[instructionIndex].group)*10000000000;
//
//    output = binaryToDecimal(output);
//
//    base32output = decimalToBase32(output);
//    writeToOutputFile(base32output, filename);
//    free(base32output);

    char instructionLength [80];
    sprintf(instructionLength, "%d", instructionIndex);
    writeToOutputFile(instructionLength, filename);

}

/*----------------------------------------------------------------------------*/
/*
 * Description: generates the output file for an instruction with no extra words
 * Input:       Data struct, filename string
 * Output:		nothing
 */
/*----------------------------------------------------------------------------*/
void create_R_Instruction(Data * data, char * filename, Instruction *instruction){
    char buf[33];
    char * hex;

    int num;

    char * opcode = decimal_to_binary(instruction->opcode,6);
    char * rs = decimal_to_binary(instruction->rs,5);
    char * rt = decimal_to_binary(instruction->rt,5);
    char * rd = decimal_to_binary(instruction->rd,5);
    char * funct = decimal_to_binary(instruction->funct,5);
    char * not_in_used = decimal_to_binary(0,6);
    snprintf(buf, sizeof(buf), "%s%s%s%s%s%s", opcode, rs,rt,rd,funct,not_in_used);
//    snprintf(buf, sizeof(buf), "%s%s%s%s%s%s", not_in_used, funct,rd,rt,rs,opcode);
    printf("%s ",buf);
    num = binary2decimal(buf);
    hex = littleEn(buf,strlen(buf));

    printf("%08x %s\n", num,hex);

    free(hex);

    writeToOutputFile(buf, filename);
    free(opcode);
    free(rs);
    free(rt);
    free(rd);
    free(funct);
    free(not_in_used);
}

/*----------------------------------------------------------------------------*/
/*
 * Description: generates the output file for an instruction with no extra words
 * Input:       Data struct, filename string
 * Output:		nothing
 */
/*----------------------------------------------------------------------------*/
void create_I_Instruction(Data * data, char * filename, Instruction *instruction){
    char buf[33];
    int num;
    char * hex;
    char * opcode = decimal_to_binary(instruction->opcode,6);
    char * rs = decimal_to_binary(instruction->rs,5);
    char * rt = decimal_to_binary(instruction->rt,5);
    char * immed = decimal_to_binary(instruction->immed,16);
    snprintf(buf, sizeof(buf), "%s%s%s%s", opcode, rs,rt,immed);
//    snprintf(buf, sizeof(buf), "%s%s%s%s", immed, rt,rs,opcode);

    printf("%s ",buf);
    num = binary2decimal(buf);
    hex = littleEn(buf,strlen(buf));
    printf("%08x %s\n", num,hex);
    writeToOutputFile(buf, filename);
    free(opcode);
    free(rs);
    free(rt);
    free(immed);
}

/*----------------------------------------------------------------------------*/
/*
 * Description: generates the output file for an instruction with no extra words
 * Input:       Data struct, filename string
 * Output:		nothing
 */
/*----------------------------------------------------------------------------*/
void create_J_Instruction(Data * data, char * filename, Instruction *instruction){
    char buf[33];
    int num;
    char *hex;
    char * opcode = decimal_to_binary(instruction->opcode,6);
    char * reg = decimal_to_binary(instruction->reg,1);
    char * address = decimal_to_binary(instruction->address,25);
    snprintf(buf, sizeof(buf), "%s%s%s", opcode, reg, address);


    printf("%s ",buf);
    num = binary2decimal(buf);
    hex = littleEn(buf,strlen(buf));

    printf("%08x %s\n", num,hex);
    writeToOutputFile(buf, filename);
    free(opcode);
    free(reg);
    free(address);
}




/*----------------------------------------------------------------------------*/
/*
 * Description: writes the lengths at the start of the file
 * Input:       output string, filename string
 * Output:		nothing
 */
/*----------------------------------------------------------------------------*/
void writeLengthsToFile(Data * data, char * filename){
    FILE *file;
    char * base32InstructionLength = NULL;
    char instructionLength [80];
    char dataLength [80];

    char * base32DataLength = NULL;

    sprintf(instructionLength, "%d", ((data -> ic) -100));
    sprintf(dataLength, "%d", data -> dcf);

    file = fopen(filename, "w");

    /* Write instructions to file */
    base32InstructionLength = decimalToBase32(data->ic);
    fputs(instructionLength, file);
    free(base32InstructionLength);

    /* seperate by space between instruction and data length */
    fputc(' ', file);

    /* Write data length to file */
    base32DataLength = decimalToBase32(data->dc);
    fputs(dataLength,file);
    free(base32DataLength);

    /* code is seperated by a new line */
    fputc('\n', file);

    fclose(file);

}

/*----------------------------------------------------------------------------*/
/*
 * Description: writes the output to file
 * Input:       output string, filename string
 * Output:		nothing
 */
/*----------------------------------------------------------------------------*/
void writeToOutputFile(char * output,char * filename) {
    FILE *file;

    if(*output=='\0'){
        return;
    }

    file = fopen(filename, "a");
    fseek(file, -1 , SEEK_END);
    /* Write instructions to file */
    fputs(output, file);
    /* code is seperated by a new line */
    fputc('\n', file);
    fclose(file);
}













/*----------------------------------------------------------------------------*/
/*
 * Description: convert a number in base 10 to a string in base32
 * Input:       int
 * Output:		string
 */
/*----------------------------------------------------------------------------*/
char* decimalToBase32(unsigned long int decNum){
    char * output = NULL;
    int remainder = 0;
    int tempNum;
    int counter=0;

    /* special handling in case of 0, because it won't go into while loop */
    if(decNum==0){
        output = realloc(output,sizeof(char));
        *output = '0';
    }


    while (decNum != 0) {
        output = realloc(output,sizeof(char) * (counter+1));
        tempNum=decNum / BASE32;
        remainder = decNum - tempNum * BASE32;
        decNum = tempNum;

        /* ascii numbers and characters are not a continuious */
        if(remainder>9){
            /* 'A' is ascii 65 */
            output[counter]='A'+remainder-10;
        }else{
            /* '0' is ascii 48 */
            output[counter]='0'+remainder;
        }

        counter++;
    }
    output = realloc(output,sizeof(char) * (counter+1));
    output[counter]='\0';
    return output;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: convert a  decimal to a int representign a binary
 * Input:       number , amount of bits
 * Output:		pointer to string
 */
/*----------------------------------------------------------------------------*/
char *decimalToBinary(int number, int bits)
{
    int c, d, count;
    char *pointer;
    int useFlag = number<0 ? 1 : 0;
    int flag = 0;
    count = 0;

    number = number < 0 ? number *-1 : number;
    pointer = (char*)malloc(bits+1);
    if ( pointer == NULL )
        exit(EXIT_FAILURE);


    for ( c = bits-1 ; c >= 0 ; c-- )
    {
        d = number >> c;
        if ( d & 1 )
            *(pointer+count) = '1';
        else
            *(pointer+count) = '0';
        count++;
    }
    printf("count: %d \n",count);
    *(pointer+count) = '\0';
    for ( c = bits-1 ; c >= 0 ; c-- )
    {
        if (flag == 1 ){
            *(pointer+c) = *(pointer+c) == '1' ? '0' : '1';
        }
        if ( useFlag == 1 && *(pointer+c) == '1' ){
            flag = 1;
        }

    }

    return pointer;
}

char *decimal_to_binary(int number, int bits)
{
    int c, d, count;
    char *pointer;
    int useFlag = number<0 ? 1 : 0;
    int flag = 0;
    count = 0;

    number = number < 0 ? number *-1 : number;
    pointer = (char*)malloc(bits+1);
    if ( pointer == NULL )
        exit(EXIT_FAILURE);


    for ( c = bits-1 ; c >= 0 ; c-- )
    {
        d = number >> c;
        if ( d & 1 )
            *(pointer+count) = '1';
        else
            *(pointer+count) = '0';
        count++;
    }

    *(pointer+count) = '\0';
    for ( c = bits-1 ; c >= 0 ; c-- )
    {
        if (flag == 1 ){
            *(pointer+c) = *(pointer+c) == '1' ? '0' : '1';
        }
        if ( useFlag == 1 && *(pointer+c) == '1' ){
            flag = 1;
        }

    }


    return pointer;
}

int string_length(char s[])
{
    int i=0;

    while(s[i]!='\0')
        i++;

    return i;
}

int binary2decimal(char * bin)
{
    char *a = bin;
    int converted = 0;
    int num = 0;
    do {
        int b = *a=='1'?1:0;
        num = (num<<1)|b;
        a++;
    } while (*a);

    return  num;



    converted |= ((0xff & num) << 24);
    converted |= (((0xff << 8) & num) <<8);
    converted |= (((0xff << 16) & num) >> 8);
    converted |= (((0xff << 24) & num) >> 24);

    return converted;

}

