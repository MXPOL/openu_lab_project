/*
 ====================================================================================
 Module:        outputManager
 Description: 	generates the output file and writes the code into them
 ====================================================================================
 */

#include "header.h"

/*----------------------------------------------------------------------------*/
/*
 * Description: generates the output file and writes the code into them
 * Input:       Data struct
 * Output:		nothing
/*----------------------------------------------------------------------------*/
void outputManager(Data * data, char * filename){
    char outputFileName[MAX_FILENAME+1];

    strcpy(outputFileName,filename);
    strcat(outputFileName, ".ob");


    /* write the instruction and data lengths to file */
    writeLengthsToFile(data, outputFileName);

    writeInstructionToFile(data,outputFileName);

    writeDirectivesToFile(data,outputFileName);

    /* remove the .ob extension */
    strcpy(outputFileName,filename);

    if(data->exc>0){
        strcat(outputFileName, ".ext");
        writeExternToFile(data,outputFileName);
    }

    /* remove the .ext extension */
    strcpy(outputFileName,filename);

    if(data->enc>0){
        strcat(outputFileName, ".ent");
        writeEntryToFile(data,outputFileName);
    }


}

/*----------------------------------------------------------------------------*/
/*
 * Description: generates the output file for an instruction with no extra words
 * Input:       Data struct, filename string
 * Output:		nothing
 */
/*----------------------------------------------------------------------------*/
void print_R_Instruction( char * filename,int instructionIndex, Instruction *instruction){
    char buf[33];
    char * hexString;

    char * opcode = decimal_to_binary(instruction->opcode,6);
    char * rs = decimal_to_binary(instruction->rs,5);
    char * rt = decimal_to_binary(instruction->rt,5);
    char * rd = decimal_to_binary(instruction->rd,5);
    char * funct = decimal_to_binary(instruction->funct,5);
    char * not_in_used = decimal_to_binary(0,6);

    snprintf(buf, sizeof(buf), "%s%s%s%s%s%s", opcode, rs,rt,rd,funct,not_in_used);

    hexString = littleEn(buf,strlen(buf));

    free(hexString);

    snprintf(buf, sizeof(buf), "%d\t%s", instructionIndex,hexString);

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
void print_I_Instruction(char * filename,int instructionIndex, Instruction *instruction){
    char buf[33];
    char * hex;
    char * opcode = decimal_to_binary(instruction->opcode,6);
    char * rs = decimal_to_binary(instruction->rs,5);
    char * rt = decimal_to_binary(instruction->rt,5);
    char * immed = decimal_to_binary(instruction->immed,16);

    /* 32 bits array for this instruction*/
    snprintf(buf, sizeof(buf), "%s%s%s%s", opcode, rs,rt,immed);
    /* generate the Hex number as little Endian format*/
    hex = littleEn(buf,strlen(buf));
    /* generate the line for the file*/
    snprintf(buf, sizeof(buf), "%d\t%s", instructionIndex,hex);

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
void print_J_Instruction(char * filename,int instructionIndex, Instruction *instruction){
    char buf[80];

    char *hex;
    char * opcode = decimal_to_binary(instruction->opcode,6);
    char * reg = decimal_to_binary(instruction->reg,1);
    char * address = decimal_to_binary(instruction->address,25);


    snprintf(buf, sizeof(buf), "%s%s%s", opcode, reg, address);

    hex = littleEn(buf,strlen(buf));

    snprintf(buf, sizeof(buf), "%d\t%s", instructionIndex,hex);
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
    char buff [80];

    /* create line for print */
    sprintf(buff, "\t%d %d", ((data -> ic) -100),data -> dcf);

    file = fopen(filename, "w");

    /* Write instructions to file */
    fputs(buff, file);


    /* code is separated by a new line */
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
    /* code is separated by a new line */
    fputc('\n', file);
    fclose(file);
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

/*----------------------------------------------------------------------------*/
/*
 * Description: write all the external variables to file
 * Input:       Data sturct, string filename
 * Output:		nothing
 */
/*----------------------------------------------------------------------------*/
void writeExternToFile(Data * data,char * filename) {
    FILE *file;
    int i=0;
    int j =0;
    char line[100];

    file = fopen(filename, "w");

    for (i= 0 ; i < data->exc  ; i++) {
        for (j = 0 ; j <data->externArr[i].appearance; j++){
            snprintf(line, sizeof(line), "%s\t%d", data->externArr[i].name,data->externArr[i].icArr[j]);
            fputs(line, file);
            fputc('\n', file);
        }

    }
    fclose(file);



}

/*----------------------------------------------------------------------------*/
/*
 * Description: write all the entry variables to file
 * Input:       Data sturct, string filename
 * Output:		nothing
 */
/*----------------------------------------------------------------------------*/
void writeEntryToFile(Data * data,char * filename) {
    FILE *file;
    char line[100];
    int i=0;

    file = fopen(filename, "w");
    for(i=0;i<data->enc;i++){
        snprintf(line, sizeof(line), "%s\t%d", data->entryArr[i].name,data->entryArr[i].address);
        fputs(line, file);
        fputc('\n', file);
    }
    fclose(file);
}

/*----------------------------------------------------------------------------*/
/*
 * Description: write all the Instruction to file
 * Input:       Data struct, string output file name
 * Output:		nothing
 */
/*----------------------------------------------------------------------------*/
void writeInstructionToFile(Data * data,char * outputFileName) {
    int instructionIndex;
    int currentInstruction;
    Instruction * inst;
    InstructionInfo info;



/* write all the instructions to file */
    for(instructionIndex=100;instructionIndex < (data -> ic) ;instructionIndex = instructionIndex + 4) {
        currentInstruction = (instructionIndex - 100) / 4;
        inst = &(data -> instArr[currentInstruction]);
        getInstructionInfo(inst->cmdIndex,&info);

/* print the instruction depends on type*/
        switch (info.instructionType) {
            case TYPE_R:
                print_R_Instruction(outputFileName,instructionIndex,inst);
                break;
            case TYPE_I:
                print_I_Instruction(outputFileName,instructionIndex,inst);
                break;
            case TYPE_J:
                print_J_Instruction(outputFileName,instructionIndex,inst);
                break;
        }
    }
}

/*----------------------------------------------------------------------------*/
/*
 * Description: write all the data variables to file
 * Input:       Data sturct, string output file name
 * Output:		nothing
 */
/*----------------------------------------------------------------------------*/
void writeDirectivesToFile(Data * data,char * outputFileName) {
    char temp[33];
    char line[80];
    int i;
    char *tempPointer;
    char *bitsString;
    char *littleEndianBitString;
    char *hexString;
    char * buff = (char *)calloc(1, sizeof(char));
    int bitsCount = 0;


    for (i = 0 ; i < data->dc  ; i ++) {
        switch (data->directiveArr[i].kind) {
            case DATA_DB:
            case DATA_ASCIZ:
                bitsString = decimal_to_binary(data->directiveArr[i].data, 8);
                buff = (char *) realloc(buff, bitsCount + 8 + 1);
                strcat(buff, bitsString);
                free(bitsString);
                bitsCount+=8;
                break;
            case DATA_DH:
                bitsString = decimal_to_binary(data->directiveArr[i].data, 16);
                buff = (char *) realloc(buff, bitsCount + 16 + 1);
                littleEndianBitString = littleEndian16BitsFormat(bitsString);
                strcat(buff, littleEndianBitString);
                free(bitsString);
                free(littleEndianBitString);
                bitsCount+=16;
                break;
            case DATA_DW:
                bitsString = decimal_to_binary(data->directiveArr[i].data, 32);
                buff = (char *) realloc(buff, bitsCount +32 + 1);
                littleEndianBitString = littleEndian32BitsFormat(bitsString);
                strcat(buff, littleEndianBitString);
                free(bitsString);
                free(littleEndianBitString);
                bitsCount+=32;
                break;
        }

    }
    /* save the buff address for free function in the end*/
    tempPointer = buff;

    for(i = 0 ; i < bitsCount / 32 ; i++ ){
        strncpy( temp, buff , 32);
        temp[33] = '\0';
        hexString = binaryToHex(temp, strlen(temp) - 1);
        snprintf(line, sizeof(line), "%d\t%s", data->ic,hexString);
        writeToOutputFile(line, outputFileName);

        data->ic +=4;
        buff += 32;

    }

    strncpy( temp, buff , bitsCount%32);
    temp[bitsCount%32] = '\0';
    hexString = binaryToHex(temp, strlen(temp) - 1);
    snprintf(line, sizeof(line), "%d\t%s", data->ic,hexString);
    writeToOutputFile(line, outputFileName);

    /* free buff string array*/
    free(tempPointer);

}

/*----------------------------------------------------------------------------*/
/*
 * Description: return 16 bits string as little endian format
 * Input:       string with 16 bits
 * Output:		little endian 16 bits string
 */
/*----------------------------------------------------------------------------*/
char * littleEndian16BitsFormat ( char *stringBits){

    char *fixedBitsStr;
    int i;
    int j;

    j = 0;

    fixedBitsStr = (char *)calloc(17, sizeof(char));

    for(i = 8 ; i < 16 ; i ++){
        *(fixedBitsStr+j) = *(stringBits+i);
        j++;
    }

    for(i = 0 ; i < 8 ; i ++){
        *(fixedBitsStr+j) = *(stringBits+i);
        j++;
    }

    *(fixedBitsStr+j) = '\0';
    return fixedBitsStr;


}

/*----------------------------------------------------------------------------*/
/*
 * Description: return 32 bits string as little endian format
 * Input:       string with 32 bits
 * Output:		little endian 32 bits string
 */
/*----------------------------------------------------------------------------*/

char * littleEndian32BitsFormat ( char *stringBits){
    char *fixedBitsStr;
    int i;
    int j;

    fixedBitsStr = (char *)calloc(33, sizeof(char));
    j =0;

    for(i = 24 ; i < 32 ; i ++){
        *(fixedBitsStr+j) = *(stringBits+i);
        j++;
    }

    for(i = 16 ; i < 24 ; i ++){
        *(fixedBitsStr+j) = *(stringBits+i);
        j++;
    }

    for(i = 8 ; i < 16 ; i ++){
        *(fixedBitsStr+j) = *(stringBits+i);
        j++;
    }

    for(i = 0 ; i < 8 ; i ++){
        *(fixedBitsStr+j) = *(stringBits+i);
        j++;
    }

    *(fixedBitsStr+j) = '\0';

    return fixedBitsStr;

}


char *binaryToHex(char *binary, int length)
{
    int i;
    char *hex;
    char *paddedBinary = padBinary(binary, length);
    int hexLength = strlen(paddedBinary) / 4;	/*	We need one hex symbol for every 4 binary symbols */
    hexLength = hexLength + hexLength/2 + 1;	/*	Make place for a space after every two symbols + the null terminater */
    hex = (char *)malloc(hexLength);
    if(hex == NULL)
        exit(1);
    for(i = 0; i < length; i += 8)
    {
        char halfByte[5];
        memcpy(halfByte, paddedBinary, 4);
        halfByte[4] = '\0';
        *hex++ = valueOf(halfByte);
        paddedBinary += 4;

        memcpy(halfByte, paddedBinary, 4);
        halfByte[4] = '\0';
        *hex++ = valueOf(halfByte);
        paddedBinary += 4;

        *hex++ = ' ';
    }
    *hex = '\0';
    hex -= hexLength - 1;

    return hex;
}

char *padBinary(char *binary, int length)
{
    int i;
    int padding = 8 - length % 8;
    int paddedBinaryLength = padding + length + 1;
    char *paddedBinary;
    if(padding == 8 || padding == 0)
        return binary;
    paddedBinary = (char *)malloc(paddedBinaryLength);
    if(paddedBinary == NULL)
        exit(1);
    for(i = 0; i < padding; ++i)
        *paddedBinary++ = '0';
    while(*binary != '\0')
        *paddedBinary++ = *binary++;
    *paddedBinary = '\0';
    paddedBinary -= paddedBinaryLength - 1;
    return paddedBinary;
}

char valueOf(char *halfByte)
{
    if(strcmp(halfByte, "0000") == 0)
        return '0';
    if(strcmp(halfByte, "0001") == 0)
        return '1';
    if(strcmp(halfByte, "0010") == 0)
        return '2';
    if(strcmp(halfByte, "0011") == 0)
        return '3';
    if(strcmp(halfByte, "0100") == 0)
        return '4';
    if(strcmp(halfByte, "0101") == 0)
        return '5';
    if(strcmp(halfByte, "0110") == 0)
        return '6';
    if(strcmp(halfByte, "0111") == 0)
        return '7';
    if(strcmp(halfByte, "1000") == 0)
        return '8';
    if(strcmp(halfByte, "1001") == 0)
        return '9';
    if(strcmp(halfByte, "1010") == 0)
        return 'A';
    if(strcmp(halfByte, "1011") == 0)
        return 'B';
    if(strcmp(halfByte, "1100") == 0)
        return 'C';
    if(strcmp(halfByte, "1101") == 0)
        return 'D';
    if(strcmp(halfByte, "1110") == 0)
        return 'E';
    if(strcmp(halfByte, "1111") == 0)
        return 'F';
    return 0;
}

char * littleEn (char * binary,int length ){
    int i;
    char *hex;
    char *paddedBinary = padBinary(binary, length);
    int hexLength = strlen(paddedBinary) / 4;	/*	We need one hex symbol for every 4 binary symbols */
    paddedBinary += strlen(paddedBinary);
    hexLength = hexLength + hexLength/2 + 1;	/*	Make place for a space after every two symbols + the null terminater*/
    hex = (char *)malloc(hexLength);
    if(hex == NULL)
        exit(1);
    for(i = 0; i < length; i += 8)
    {
        char halfByte[5];
        memcpy(halfByte  , paddedBinary - 8 , 4);
        halfByte[4] = '\0';
        *hex++ = valueOf(halfByte);


        memcpy(halfByte, paddedBinary - 4 , 4);
        halfByte[4] = '\0';
        *hex++ = valueOf(halfByte);
        paddedBinary -= 8;

        *hex++ = ' ';
    }
    *hex = '\0';
    hex -= hexLength - 1;

    return hex;

}