/*
 ====================================================================================
 Module:        directivesManager
 Description: 	responsible for handling all the directive code lines during the first pass
 ====================================================================================
 */

#include "header.h"


/*----------------------------------------------------------------------------*/
/*
 * Description: in charge of directing the handling the different directives
 * Input:       Data struct, string tag
 * Output:		1 if letter or number, 0 otherwise
 */
/*----------------------------------------------------------------------------*/
int directivesManager(Data *data, char *tag, int whatPass) {

    char s[MAX_DIRECTIVE_LEN + 1];
    /* skip the dot */
    data->line++;
    /* get the directive */
    sscanf((data->line), "%s", s);
    data->line += strlen(s) + 1;
    eatSpace(data);

    if (whatPass == FIRST_PASS){
        if ( strcmp(s, STR_DIRECTIVE_DB) == 0) {
            return dataDirectiveHandler(data, tag, DATA_DB);
        }
        if (strcmp(s, STR_DIRECTIVE_DH) == 0) {
            return dataDirectiveHandler(data, tag, DATA_DH);
        }
        if (strcmp(s, STR_DIRECTIVE_DW) == 0) {
            return dataDirectiveHandler(data, tag, DATA_DW);
        }
        if (strcmp(s, STR_DIRECTIVE_ASCIZ) == 0) {
            return stringDirectiveHandler(data, tag);
        }
        if (strcmp(s, STR_DIRECTIVE_ENTRY) == 0) {
            return entryDirectiveHandler(data, tag);
        }

        if (strcmp(s, STR_DIRECTIVE_EXTERN) == 0) {
            return externDirectiveHandler(data, tag);
        }

        printf("[Error] on line %d: %s is not a valid directive\n", data->lc, s);
        data->containError = TRUE;

        return 0;

    }

    if (whatPass == SECOND_PASS) {
        if (strcmp(s, STR_DIRECTIVE_ENTRY) == 0) {
            return entryDirectiveHandlerSecondPass(data);
        }

        return 1;
    }


}

/*----------------------------------------------------------------------------*/
/*
 * Description: handles the data directive parsing and add to array
 * Input:       Data struct, string tag
 * Output:		1 if letter or number, 0 otherwise
 */
/*----------------------------------------------------------------------------*/
int dataDirectiveHandler(Data *data, char *tag, int kind) {

    int i = 0;
    int num;
    int dcounter = 0;
    int numberArray[80];
    int count;
    int tagNumber = -1;

    int maxNumber;
    int minNumber;

    switch (kind) {
        case DATA_DB:
            maxNumber = MAX_NUMBER_DATA_DB;
            minNumber = MIN_NUMBER_DATA_DB;
            break;
        case DATA_DH:
            maxNumber = MAX_NUMBER_DATA_DH;
            minNumber = MIN_NUMBER_DATA_DH;
            break;
        case DATA_DW:
            maxNumber = MAX_NUMBER_DATA_DW;
            minNumber = MIN_NUMBER_DATA_DW;
            break;
    }

    /* state 1 means check number, state 2 means check comma, state 0 means exit */
    int state = GET_NUMBER;

    /* check for an empty data directive */
    if (*(data->line) == '\n' || *(data->line) == EOF) {
        printf("[Error] on line %d: malformed data directive - empty directive parameters \n", data->lc);
        data->containError = TRUE;
        return 0;
    }

    /* get the numbers */
    while (state != EXIT) {
        eatSpace(data);
        if (state == GET_NUMBER) {
            if (sscanf(data->line, "%d", &num) == 0  || *(data->line) == '\n' || *(data->line) == EOF ) {
                printf("[Error] on line %d:- malformed data directive - missing number before/after comma \n", data->lc);
                data->containError = TRUE;
                return 0;
            }
            if ( num < minNumber || num > maxNumber){
                printf("[Error] on line %d: The number exceeds the representation limits of the data type \n", data->lc);
                data->containError = TRUE;
                return 0;
            }
            numberArray[dcounter] = num;
            dcounter++;
            /* get the line location pointer (line_loc) beyond what sscanf took in */
            if (*(data->line) == '-' || *(data->line) == '+') {
                data->line++;
            }
            while (isdigit(*(data->line))) {
                data->line++;
            }
            state = GET_COMMA;
        } else if (state == GET_COMMA) {
            eatSpace(data);
            if (*(data->line) != ',' && *(data->line) != '\n' && *(data->line) != EOF) {
                printf("[Error] on line %d: - malformed data directive - unidentified characters in addition to numeric parameters\n",
                       data->lc);
                data->containError = TRUE;
                return 0;
            }
            if (*(data->line) == '\n' || *(data->line) == EOF) {
                state = EXIT;
            } else {
                state = GET_NUMBER;;
                data->line++;
            }
        }
    }

    /* if there are tag, add this line to the symbol table */
    if (tag != NULL && *tag != '\0') {
        tagNumber = data -> tc;
        addTag(data, tag, data->dc, kind);
    }
    /* add the numbers to Directive table */
    for (i = 0; i < dcounter; i++) {
        addDirective(data, numberArray[i],tagNumber,kind);
    }

    return 1;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: adds a directive to array
 * Input:       Data struct, directive address
 * Output:		nothing
 */
/*----------------------------------------------------------------------------*/
void addDirective(Data *data, int directive,int tagNumber, int kind) {
    data->directiveArr = realloc(data->directiveArr, sizeof(Directive) * (data->dc + 1));
    data->directiveArr[data->dc].data = directive;
    data->directiveArr[data->dc].tagNumber = tagNumber;
    data->directiveArr[data->dc].kind = kind;
    data->directiveArr[data->dc].icf = data->dc;
    data->dc++;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: handles the string directive parsing and add to array
 * Input:       Data struct, string tag
 * Output:		1 if letter or number, 0 otherwise
 */
/*----------------------------------------------------------------------------*/
int stringDirectiveHandler(Data *data, char *tag) {
    int i;
    int counter = 0;
    char charArray[100];
    char temp[100];
    int tagNumber;

    strcpy(temp,data->line);


    if (*(data->line) == '\n' || *(data->line) == EOF) {
        printf("[Error] on line %d: empty directive\n", data->lc);
        data->containError = TRUE;
        return 0;
    }
    /* check for beggining character of the string (") */
    if (*(data->line) == '"') {
        /* put all the characters of the string into the directives table*/
        data->line++;
        while (*(data->line) != '"' && *(data->line) != '\n' && *(data->line) != EOF) {
            charArray[counter] = *(data->line);
            counter++;
            data->line++;
        }
        /* if the last character of the string is not a closing quotation mark - it's an invalid string*/
        if (*(data->line) != '"') {
            printf("[Error] on line %d: malformed string directive - no ending quotation mark in string\n", data->lc);
            data->containError = TRUE;
            return 0;
        }
        data->line++;

        /* check for extra characters after the string ending (it's not a valid string directive that way) */
        eatSpace(data);

        if (*(data->line)  != '\0'  && *(data->line) != '\n' && *(data->line) != EOF  ) {
            printf("[Error] on line %d: malformed string directive - extra characters after string ending\n", data->lc);
            data->containError = TRUE;
            return 0;
        }
        /* if the string directive contains no string - it's an invalid directive */
    } else {
        printf("[Error] on line %d: malformed string directive - no leading quotation mark in string\n", data->lc);
        data->containError = TRUE;
        return 0;
    }

    if (tag != NULL && *tag != '\0') {
        tagNumber = data -> tc;
        addTag(data, tag, data->dc, DATA_ASCIZ);
    }
    for (i = 0; i < counter; i++) {
        addDirective(data, charArray[i],tagNumber,DATA_ASCIZ);
    }
    addDirective(data, 0,tagNumber,DATA_ASCIZ);
    return 1;
}


/*----------------------------------------------------------------------------*/
/*
 * Description: handles the entry directive parsing and add to array
 * Input:       Data struct, string tag
 * Output:		1 if letter or number, 0 otherwise
 */
/*----------------------------------------------------------------------------*/
int entryDirectiveHandler(Data *data, char *tag) {
    char tagName[MAX_TAG_LEN];
    int i;
    tagName[0] = 0;
    if (*tag != '\0') {
        printf("[Warning] on line %d: a .entry directive started with a tag\n", data->lc);
//        data->containError = TRUE;
//        return 0;
    }
    if (*(data->line) == '\n' || *(data->line) == EOF) {
        printf("[Error] on line %d: empty directive\n", data->lc);
        data->containError = TRUE;
        return 0;
    }
    getTagOperand(data, tagName);
    if (tagName == NULL) {
        printf("[Error] on line %d: malformed .entry directive\n", data->lc);
        data->containError = TRUE;
        return 0;
    }
    for (i = 0; i <= (data->enc); i++) {
        if (data->entryArr != NULL) {
            if (strcmp(tagName, "\0") != 0) {
                if (strcmp(data->entryArr[i].name, tagName) == 0) {
                    printf("[Error] on line %d: .entry directive name already exists\n", data->lc);
                    data->containError = TRUE;
                    return 0;
                }
            }
        }
    }
    for (i = 0; i <= (data->enc); i++) {
        if (data->externArr != NULL) {
            if (strcmp(data->externArr[i].name, tagName) == 0) {
                printf("[Error] on line %d: .entry directive name already defined as .extern\n", data->lc);
                data->containError = TRUE;
                return 0;
            }
        }
    }
    addEntry(data, tagName);
    return 1;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: adds a directive to array
 * Input:       Data struct, directive address
 * Output:		nothing
 */
/*----------------------------------------------------------------------------*/
void addEntry(Data *data, char *tag) {
    data->entryArr = realloc(data->entryArr, sizeof(Entry) * (data->enc + 1));
    strcpy((data->entryArr[(data->enc)]).name, tag);
    (data->entryArr[(data->enc)]).address = 0;
    data->enc++;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: adds an external tag to array
 * Input:       Data struct, string extrn tag name
 * Output:		nothing
 */
/*----------------------------------------------------------------------------*/
void addExtern(Data *data, char *tag) {
    data->externArr = realloc(data->externArr, sizeof(Extern) * (data->exc + 1));
    strcpy(data->externArr[data->exc].name, tag);
    data->externArr[data->exc].appearance = 0;
    data->exc++;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: handles the external directive parsing and add to array
 * Input:       Data struct, string tag
 * Output:		1 if letter or number, 0 otherwise
 */
/*----------------------------------------------------------------------------*/
int externDirectiveHandler(Data *data, char *tag) {
    char tagName[MAX_TAG_LEN];
    int i = 0;
    tagName[0] = 0;
    if (*tag != '\0') {
        printf("[Error] on line %d: a .extern directive can't start with a tag\n", data->lc);
        data->containError = TRUE;
        return 0;
    }
    if (*(data->line) == '\n' || *(data->line) == EOF) {
        printf("[Error] on line %d: empty directive\n", data->lc);
        data->containError = TRUE;
        return 0;
    }
    getTagOperand(data, tagName);
    if (tagName == NULL) {
        printf("[Error] on line %d: malformed .extern directive\n", data->lc);
        data->containError = TRUE;
        return 0;
    }
    /* search if tag already exists in entry array */
    for (i = 0; i < (data->enc); i++) {
        if (data->entryArr != NULL) {
            if (strcmp(tagName, "\0") != 0) {
                if (strcmp(data->entryArr[i].name, tagName) == 0) {
                    printf("[Error] on line %d: .extern directive name already defined as .entry\n", data->lc);
                    data->containError = TRUE;
                    return 0;
                }
            }
        }
    }
    /* search if tag already exists in extern array */
    for (i = 0; i < (data->exc); i++) {
        if (data->externArr != NULL) {
            if (strcmp(data->externArr[i].name, tagName) == 0) {
                printf("[Error] on line %d: .extern directive name already exists\n", data->lc);
                data->containError = TRUE;
                return 0;
            }
        }
    }
    addExtern(data, tagName);
    return 1;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: handles the entry directive parsing and add to array
 * Input:       Data struct, string tag
 * Output:		1 if letter or number, 0 otherwise
 */
/*----------------------------------------------------------------------------*/
int entryDirectiveHandlerSecondPass(Data *data) {
    char tagName[MAX_LINE_LEN] = { 0 };
    int tagIndex = -1;
    int i;
    tagName[0] = 0;

    getTagOperand(data, tagName);

    for (i = 0; i <= (data->tc); i++) {
        if ( strcmp(data->tagArr[i].name,tagName) ==0 ) {
            tagIndex = i;
            data->tagArr[i].entry = 1;
        }
    }

    if (tagIndex == -1) {
        printf("[Error] on line %d: .entry directive have been defined but wasn't found!\n", data->lc);
        data->containError = TRUE;
        return 0;
    }

    for(i = 0 ; i<(data->enc); i++){
        if (strcmp(tagName,data->entryArr[i].name) == 0) {
            data->entryArr[i].address = data->tagArr[tagIndex].address ;
            return 1;
        }
    }



    return 1;
}


