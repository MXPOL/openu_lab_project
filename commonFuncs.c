
/*
 ====================================================================================
 Module:        commonFunctions
 Description: 	a library of function which are either generic or used by multiple modules
 ====================================================================================
 */


#include "header.h"

/*----------------------------------------------------------------------------*/
/*
 * Description: checks if character is a letter or number
 * Input:       character
 * Output:		1 if letter or number, 0 otherwise
 */
/*----------------------------------------------------------------------------*/
int checkLetterOrNumber(char c)
{

    if (checkLetters(c) == 1){


        return 1;
    }
    if(c>= '0' && c <= '9'){

        return 1;
    }
    return 0;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: move the file pointer to the start of the next line
 * Input:       FILE pointer to assembly language file
 * Output:		nothing
 */
/*----------------------------------------------------------------------------*/
void eatLine(FILE * fp) {
    char c= fgetc(fp);
    while (c != '\n' && c!=EOF) {
        c=fgetc(fp);
    }
}


/*----------------------------------------------------------------------------*/
/*
 * Description: Deletes spaces whether it's at the beginning or at the end
 * Input: pointer to a string
 * Output: nothing
 */
/*----------------------------------------------------------------------------*/

void deleteSpaces(char* input)
{
    /*

    int endOfString = strlen(input) - 1;
    char * tempStr =  (char*) malloc (strlen(input));
    strcpy(tempStr,input);

    while (input[endOfString]== ' '){
        endOfString--;
    }

    *(tempStr + endOfString + 1 ) ='\0';
    strcpy(input,tempStr);
    free(tempStr);
    */
}


/*----------------------------------------------------------------------------*/
/*
 * Description: ove the string pointer beyond all the spaces in the line
 * Input:       pointer to Data struct
 * Output:		nothing
 */
/*----------------------------------------------------------------------------*/
void eatSpace(Data * data){
    data->line = getCharPtrBeyondSpace(data->line);
}

int isEndOfLine(char* pStr){
    while (*(pStr) != '\n' && *(pStr) != '\0' && *(pStr) != EOF){
        if (isspace(*pStr)){
            pStr = pStr+1;
        }
        else{
            return 0;
        }
    }
    return 1;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: ove the string pointer beyond all the spaces in the line
 * Input:       pointer to Data struct
 * Output:		nothing
 */
/*----------------------------------------------------------------------------*/
char* getCharPtrBeyondSpace(char * ptr){
    while(isspace(*ptr)){
        if (*ptr == '\n' || *ptr== EOF){
            return ptr;
        }
        ptr++;
    }
    return ptr;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: check whether a character is a capital or lowercase letter
 * Input:       character
 * Output:		1 if letter, 0 otherwise
 */
/*----------------------------------------------------------------------------*/
int checkLetters(char c){
    int numOfLetters = 26;
    if (checkInLimit(c,FIRST_CAPITAL_LETTER_ASCII,numOfLetters) == 1){
        return 1;
    }
    if (checkInLimit(c,FIRST_LOWER_LETTER_ASCII,numOfLetters) == 1){
        return 1;
    }
    return 0;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: check whether a character is a lowercase letter
 * Input:       character
 * Output:		1 if lowercase letter, 0 otherwise
 */
/*----------------------------------------------------------------------------*/
int checkLowerCase(char c){
    int numOfLetters = 26;
    if (checkInLimit(c,FIRST_LOWER_LETTER_ASCII,numOfLetters) == 1){
        return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------*/
/*
 * Description: check whether a character is an uppercase letter
 * Input:       character
 * Output:		1 if uppercase letter, 0 otherwise
 */
/*----------------------------------------------------------------------------*/
int checkUpperCase(char c){
    int numOfLetters = 26;
    if (checkInLimit(c,FIRST_CAPITAL_LETTER_ASCII,numOfLetters) == 1){
        return 1;
    }
    return 0;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: check if a char ascii value is between the limits
 * Input:       character, lower limit, length
 * Output:		1 if in limit, 0 otherwise
 */
/*----------------------------------------------------------------------------*/
int checkInLimit(char c,int startLimit,int length){
    int index;
    for (index = 0; index < length;index++){
        if ((int)c == startLimit+index){
            return 1;
        }
    }
    return 0;
}
/*----------------------------------------------------------------------------*/
/*
 * Description: check if a line is empty or not
 * Input:       pointer to Data struct
 * Output:		1 if empty, 0 otherwise
 */
/*----------------------------------------------------------------------------*/
int lineEmptyCheck(Data * data){
    eatSpace(data);
    if(*(data->line)=='\n' || *(data->line)==EOF){
        return 1;
    }


    return 0;
}
/*----------------------------------------------------------------------------*/
/*
 * Description: check whether it's a comment line
 * Input:       pointer to Data struct
 * Output:		1 if comment, 0 otherwise
 */
/*----------------------------------------------------------------------------*/
int lineCommentCheck(Data * data){
    if(*(data->line)==';'){
        return 1;
    }
    return 0;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: get the tag from beggining of a line
 * Input:       pointer to Data struct, pointer to a character array
 * Output:	    if tag is found tagGet will hold the tag, tagGet will be NULL otherwise
 */
/*----------------------------------------------------------------------------*/
void getTag(Data * data,char * tagGet){

    char tag[MAX_LINE_LEN];
    int counter = 0;
    char * c = data-> line + 1;
/*
    if (checkLetters(*(data->line))== 0){
        *tagGet='\0';
        return;
    }
    */
    while(!isspace(*c) && *c != ':' && *c != '\n'){
        if (checkLetterOrNumber(*c) == 0){
            *tagGet='\0';
            return;
        }
        counter++;
        c++;
    }

    if(isspace(*c) || *c=='\n'){

        *tagGet='\0';
        return;
    }
    if (*c == ':'){
        counter++;
        strncpy(tag, data->line,counter);
        data->line += counter+2;
        tag[counter] = '\0';
        strcpy(tagGet,tag);
    }
}

void stringRemoveNonAlphaNum(char *str)
{
    unsigned long i = 0;
    unsigned long j = 0;
    char c;

    while ((c = str[i++]) != '\0')
    {
        if (isalnum(c))
        {
            str[j++] = c;
        }
    }
    str[j] = '\0';

}




/*----------------------------------------------------------------------------*/
/*
 * Description: get the tag from NOT begginig of the line
 * Input:       pointer to Data struct, pointer to a character array
 * Output:	    if tag is found tagGet will hold the tag, tagGet will be NULL otherwise
 */
/*----------------------------------------------------------------------------*/
void getTagOperand(Data * data, char * tagGet){
    char tag[MAX_TAG_LEN];
    char * c = tag;

    sscanf(data->line, " %s", c);
    stringRemoveNonAlphaNum(c);
    strcpy(tagGet,tag);
}
