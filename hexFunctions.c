#include "header.h"
char *binaryToHex(char *binary, int length)
{
    int i;
    char *hex;
    char *paddedBinary = padBinary(binary, length);
    int hexLength = strlen(paddedBinary) / 4;	//	We need one hex symbol for every 4 binary symbols
    hexLength = hexLength + hexLength/2 + 1;	//	Make place for a space after every two symbols + the null terminater
    hex = (char *)malloc(hexLength);
    if(hex == NULL)
        exit(1);
    for(i = 0; i < length; i += 8)
    {
        char halfByte[5];
        // copy the 4 binary digits and decode them to one hex digit
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

int validate(char *binary, int length)
{
    while(*binary != '\0')
    {
        if(*binary == '0' || *binary == '1')
        {
            binary++;
        }
        else
        {
            printf("The input should be a hexadecimal number, containing only digits(0-9) and the first 6 letters(a-f).\n");
            return 0;
        }
    }

    return 1;
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
    int hexLength = strlen(paddedBinary) / 4;	//	We need one hex symbol for every 4 binary symbols
    paddedBinary += strlen(paddedBinary);
    hexLength = hexLength + hexLength/2 + 1;	//	Make place for a space after every two symbols + the null terminater
    hex = (char *)malloc(hexLength);
    if(hex == NULL)
        exit(1);
    for(i = 0; i < length; i += 8)
    {
        char halfByte[5];
        // copy the 4 binary digits and decode them to one hex digit
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

void writeData (char * binary,int length){
    char data [1000000];
    int i;
    int j  = 0;
    for(i = 0; i < length; i += 8)
    {
        char halfByte[5];
        memcpy(halfByte, binary + i, 4);
        halfByte[4] = '\0';
        data [j] = valueOf(halfByte);
        memcpy(halfByte, binary + i + 4 , 4);
        halfByte[4] = '\0';
        j++;
        data [j] = valueOf(halfByte);
        j++;
        data[j] = ' ';
        j++;
    }

    data[j] = '\0';
    printf("buff %s \n", data);
}

char * fix16bits ( char * binary){
    char *fixed;
    char first [9];
    char second [9];
    fixed = (char *)malloc(17);

    strncpy(first,binary,8);
    first[9] = '\0';

    strncpy(second,binary,8);
    second[9] = '\0';

    strcat(fixed,second);
    strcat(fixed,first);

    return fixed;

}

char * littleEnStyle (char * binary){
    size_t length = strlen(binary);
    size_t last = length - 1;

    char* result = malloc(length + 1);
    if (result == NULL)
    {
        return NULL;
    }

    result[length] = 0;

    if (length == 0)
    {
        return result;
    }


    size_t it = 0;

    while (it <= last)
    {
        result[it] = binary[last - it -3 ];
        result[it + 1 ] = binary[last - it -2];
        result[it + 2 ] = binary[last - it -1];
        result[it + 3 ] = binary[last - it];
        it +=4;
    }

    return result;

}


char * fix16 ( char *stringBits){
    char *fixedBitsStr;
    char firstBit [9];
    char secondBit [9];


    strncpy(firstBit,stringBits,8);
    strncpy(secondBit,stringBits+8,8);

    fixedBitsStr = (char *)malloc(20);
    if(fixedBitsStr == NULL)
        exit(1);

    snprintf(fixedBitsStr, 20, "%s%s", secondBit,firstBit);

    return fixedBitsStr;


}

char * fix32 ( char *stringBits){
    char *fixedBitsStr;
    char firstBit [9];
    char secondBit [9];
    char thirdBit [9];
    char fourthBit [9];


    strncpy(firstBit,stringBits,8);
    strncpy(secondBit,stringBits+8,8);
    strncpy(thirdBit,stringBits+16,8);
    strcpy(fourthBit,stringBits+24);

    fixedBitsStr = (char *)malloc(33);
    if(fixedBitsStr == NULL)
        exit(1);

    snprintf(fixedBitsStr, 33, "%s%s%s%s", fourthBit,thirdBit,secondBit,firstBit);

    return fixedBitsStr;

}