/*
 ====================================================================================
 Module:        firstPass
 Description: 	implements the first pass compiler algorithm
 ====================================================================================
 */

#include "header.h"

/*----------------------------------------------------------------------------*/
/*
 * Description: implements the first pass algorithm (from the course booklet)
 * Input:       FILE pointer to assembly language file
 * Output:		1 if successful, 0 otherwise
 */
/*----------------------------------------------------------------------------*/
int firstPassManager(Data *data, FILE *file) {
    char lineHolder[MAX_LINE_HOLDER_LENGTH];

    /* NULL means EOF. When we reach EOF it means we're done */
    while (fgets(lineHolder, MAX_LINE_LEN + 1, file) != NULL) {
        data->line = lineHolder;
        data->lc++;
        lineHandler(data, file);
    }

    if (data->containError == TRUE) {
        return 0;
    }

    /* Update the directive Arr with the right ic address */
    updateDataTable(data);
    

    printDirectiveArr(data);
    printSymbolTable(data);
   /* printInstArr(data);
    printEntryArr(data);
    */

    return 1;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: does basic line parsing and assigns input to other function for further parsing
 * Input:       pointer to Data struct, FILE pointer to assembly language file
 * Output:		1 if successful, 0 otherwise
 */
/*----------------------------------------------------------------------------*/
int lineHandler(Data *data, FILE *file) {
    char tag[MAX_LINE_LEN + 1 ] = { 0 };

    if (lineLengthCheck(data, file) == 0) {
        return 0;
    }
    if (lineEmptyCheck(data) == 1) {
        return 1;
    }
    if (lineCommentCheck(data) == 1) {
        return 1;
    }

    /* there's a tag at the start of the line */
    if (getTag(data, tag) == 1) {
        if (strlen(tag) > 31 ){
            printf("[Error] on line %d: label cant be longer than %d chars\n", data->lc, MAX_TAG_LEN );
            data->containError = TRUE;
            return 0;
        }
        if (isdigit(*tag)){
            printf("[Error] on line %d: label can not start with number\n", data->lc);
            data->containError = TRUE;
            return 0;
        }
        if (isItReservedWord(tag) == 1){
            printf("[Error] on line %d: label can not be saved word\n", data->lc);

            data->containError = TRUE;
            return 0;
        }
        if (checkIllegalChars(tag) == 1){
            printf("[Error] on line %d: label contains invalid characters\n", data->lc);
            data->containError = TRUE;
            return 0;
        }

        if (tagDupCheck(data, tag) == 1) {
            printf("[Error] on line %d: you have the tag %s more then once.\n", data->lc, tag);
            eatLine(data);
            data->containError = TRUE;
            return 0;
        }
        eatSpace(data);
    }

    if (*(data->line) == '.') {
        return directivesManager(data, tag, FIRST_PASS);
    }

    /* making sure that its lowerCase */
    if (checkUpperCase(*(data->line)) == 0) {
        return firstPassCommandsManager(data, tag);
    }

    printf("[Error] on line %d: line isn't valid a input\n", data->lc);
    data->containError = TRUE;
    return 0;
}


/*----------------------------------------------------------------------------*/
/*
 * Description: check if a line is longer than MAX_LINE_LEN
 * Input:       pointer to Data struct, FILE pointer to assembly language file
 * Output:		1 if not longer, 0 otherwise
 */
/*----------------------------------------------------------------------------*/
int lineLengthCheck(Data *data, FILE *file) {

    if (strlen(data->line) >= MAX_LINE_LEN  && (data->line)[MAX_LINE_LEN - 1] != EOF &&
        (data->line)[MAX_LINE_LEN - 1] != '\n') {
        printf("[Error] on line %d: line is longer than %d chars\n", data->lc, MAX_LINE_LEN);
        data->containError = TRUE;
        /* get rid of the rest of the line so we don't get the rest of it with fgets instead of a new one */
        eatLine(file);
        return 0;
    }
    return 1;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: check whether a tag already exists
 * Input:       pointer to Data struct, pointer to a character array
 * Output:		1 if duplicate, 0 otherwise
 */
/*----------------------------------------------------------------------------*/
int tagDupCheck(Data *data, char *tag) {
    int index;
    for (index = 0; index < data->tc; index++) {
        if (strcmp(data->tagArr[index].name, tag) == 0) {
            return 1;
        }
    }

    return 0;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: After the first pass, update the ic address for each directive
 * Input: Pointer to the data structs
 * Output: Nothing
 */
/*----------------------------------------------------------------------------*/
void updateDataTable(Data *data) {
    int i;
    int j;
    int jump;

    for (i = 0; i < (data->dc); i++) {
        /* give the final ic address for each directive*/
        data->directiveArr[i].icf += data->ic;

        /* increase the total ic */
        switch (data->directiveArr[i].kind) {
            case DATA_ASCIZ:
                jump = 1;
                break;
            case DATA_DB:
                jump = 1;
                break;
            case DATA_DH:
                jump = 2;
                break;
            case DATA_DW:
                jump = 4;
                break;
        }

        data->ic += jump - 1;

        /* increasing the total length of the data images*/
        data->dcf += jump;
    }

    /* update the right ic address in the symbol table*/
    for (i = 0; i < data->tc; i++) {
        j = 0;
        if (data->tagArr[i].kind == DATA_DH || data->tagArr[i].kind == DATA_DB ||
            data->tagArr[i].kind == DATA_DW || data->tagArr[i].kind == DATA_ASCIZ) {

            while (i != data->directiveArr[j].tagNumber) {
                j++;
            }

            data->tagArr[i].address = data->directiveArr[j].icf;

        }
    }

}
