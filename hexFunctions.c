#include <stdio.h>

#include "header.h"

/* REMOVED IN THE END */

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
    printf("[dataIndex,tagNumber,icf,Kind,Data] ");
    for (i = 0; i < (data->dc); i++) {
        printf("[");
        printf("%d,", i);
        printf("%d,", data->directiveArr[i].tagNumber);
        printf("%d,", data->directiveArr[i].icf);
        switch ((data->directiveArr[i].kind)) {
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
        printf("%d", data->directiveArr[i].data);
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