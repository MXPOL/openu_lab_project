/*
 ====================================================================================
 Module:        firstPass
 Description: 	implements the first pass compiler algorithm
 ====================================================================================
 */

#include "header.h"

void printSymbolTable(Data *data) {
    int i;
    printf("\n* symbol table * \n");
    for (i = 0; i < data->tc; i++) {
        printf("[%s,%d,", data->tagArr[i].name, data->tagArr[i].address);
        switch (data->tagArr[i].kind) {
            case CODE:
                printf("CODE]\n");
                break;
            case DATA_DB:
                printf("DATA_DB]\n");
                break;
            case DATA_DH:
                printf("DATA_DH]\n");
                break;
            case DATA_DW:
                printf("DATA_DW]\n");
                break;
            case DATA_ASCIZ:
                printf("DATA_ASCIZ]\n");
                break;
            default:
                printf("]\n");
                break;
        }
    }
}

void printInstArr(Data *data) {
    int i;
    printf("\n");

    printf("Inst Count: %d  \n", data->ic);
    for (i = 0; i < ((data->ic) - 100) / 4; i++) {
        printf("-------------------------\n");
        printf("ic: %d \n", data->instArr[i].ic);
        printf("op code: %d \n", data->instArr[i].opcode);
        printf("funct code: %d \n", data->instArr[i].funct);
        printf("funct rs: %d \n", data->instArr[i].rs);
        printf("funct rt: %d \n", data->instArr[i].rt);
        printf("funct rd: %d \n", data->instArr[i].rd);
        printf("funct im: %d \n", data->instArr[i].immed);
        printf("funct address: %d \n", data->instArr[i].address);
        printf("-------------------------\n");
    }

}

void printDirectiveArr(Data *data) {
    int i;
    printf("DATA COUNT: %d  \n", data->dc);
/*
    for(i=0;i<(data->dc);i++){
        printf("(%d,",i);
        switch ((data->directiveArrIndex[i])) {
            case DATA_ASCIZ:
                printf("ASCIZ,");
                break;
            case DATA_DB:
                printf("DB,");
                break;
            case DATA_DH:
                printf("DH,");
                break;
            case DATA_DW:
                printf("DW,");
                break;
            default:
                printf("(  ,");
                break;
        }
        printf( "%d)-" ,data->directiveArr[i]);
    }
    printf("\n");*/
    printf("[dataIndex,tagNumber,icf,Kind,Data] ");
    for (i = 0; i < (data->dc); i++) {
        printf("[");
        printf("%d,", i);
        printf("%d,", data->directiveArr2[i].tagNumber);
        printf("%d,", data->directiveArr2[i].icf);
        switch ((data->directiveArr2[i].kind)) {
            case DATA_ASCIZ:
                printf("ASCIZ,");
                break;
            case DATA_DB:
                printf("DB,");
                break;
            case DATA_DH:
                printf("DH,");
                break;
            case DATA_DW:
                printf("DW,");
                break;
            default:
                printf(" ,");
                break;
        }
        printf("%d", data->directiveArr2[i].data);
        printf("] ");
    }
}

void printEntryArr(Data *data) {
    int i;
    printf("\n* EntryArr * \n");
    for (i = 0; i < data->enc; i++) {
        printf("[%s]", data->entryArr[i].name);
    }
    printf("\n");
    printf("\n* ExternArrArr * \n");
    for (i = 0; i < data->exc; i++) {
        printf("[%s]", data->externArr[i].name);
    }

}

void updateDataTable(Data *data) {
    int i;
    int j;
    int jump;


    for (i = 0; i < (data->dc); i++) {
        data->directiveArr2[i].icf += data->ic;
        switch (data->directiveArr2[i].kind) {
            case DATA_ASCIZ:
            case DATA_DB:
                jump = 0;
                break;
            case DATA_DH:
                jump = 1;
                break;
            case DATA_DW:
                jump = 3;
                break;
        }

        data->ic += jump;
        data->dcf += jump + 1;

    }


    for (i = 0; i < data->tc; i++) {
        if (data->tagArr[i].kind == DATA_DH || data->tagArr[i].kind == DATA_DB ||
            data->tagArr[i].kind == DATA_DW || data->tagArr[i].kind == DATA_ASCIZ) {
            while (i != data->directiveArr2[j].tagNumber) {
                j++;
            }

            data->tagArr[i].address = data->directiveArr2[j].icf;

        }
    }
    printf("\n");

}


/*----------------------------------------------------------------------------*/
/*
 * Description: implements the first pass algorithm (from the course booklet)
 * Input:       FILE pointer to assembly language file
 * Output:		1 if successful, 0 otherwise
 */
/*----------------------------------------------------------------------------*/
int firstPassManager(Data *data, FILE *file) {
    char lineHolder[MAX_LINE_LEN + 1];

    /* NULL means EOF. When we reach EOF it means we're done */
    while (fgets(lineHolder, MAX_LINE_LEN, file) != NULL) {
        data->line = lineHolder;
        data->lc++;
        lineHandler(data, file);
    }


    updateDataTable(data);
    printSymbolTable(data);
    printInstArr(data);
    printDirectiveArr(data);
    printEntryArr(data);


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
    char tag[MAX_TAG_LEN] = { 0 };

    if (lineLengthCheck(data, file) == 0) {
        return 0;
    }
    if (lineEmptyCheck(data) == 1) {
        return 1;
    }
    if (lineCommentCheck(data) == 1) {
        return 1;
    }

    getTag(data, tag);

    /* there's a tag at the start of the line */
    if (*tag != '\0') {
        if (tagDupCheck(data, tag) == 1) {
            printf("[Error] on line %d: you have the tag %s more then once.\n", data->lc, tag);
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
    if (strlen(data->line) >= MAX_LINE_LEN - 1 && (data->line)[MAX_LINE_LEN - 1] != EOF &&
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
 * Description: adds a tag to the tag array inside the Data struct
 * Input:       pointer to Data struct, tag name pointer, address to point tag at
 * Output:	    nothing
 */
/*----------------------------------------------------------------------------*/
void addTag(Data *data, char *tag, int dirAddress, int kind) {
    data->tagArr = realloc(data->tagArr, sizeof(Tag) * (data->tc + 2));
    strcpy(data->tagArr[data->tc].name, tag);
    data->tagArr[data->tc].address = dirAddress;
    data->tagArr[data->tc].kind = kind;
    data->tc++;
}
