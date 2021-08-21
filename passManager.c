/*
 ====================================================================================
 Module:        passManager
 Description: 	manages data allocation and first and second passes
 ====================================================================================
 */

#include "header.h"

/*----------------------------------------------------------------------------*/
/*
 * Description: manages the first and second pass, and file creation
 * Input:       pointer to the current file being read
 * Output:	    1 if successful, 0 otherwise
 */
/*----------------------------------------------------------------------------*/
int passManager(FILE *file, char *filename) {
    /* create and initialize the data structures */
    Data data;
    initializeData(&data);

    /* go over the code for the first time*/
    firstPassManager(&data, file);

    /* if there's an error, don't continue */
    if (data.containError == TRUE) {
        return 0;
    }

    /*reset the file pointer to beggining of file */
    fseek (file, 0, SEEK_SET);

    /* reset instruction counter to 0 */
    data.ic = 100;

    /* reset line counter to 0 */
    data.lc = 1;

    printf("\n");

    /* go over the code for a second time*/
    secondPassManager(&data,file);

    /* if there's an error, don't continue */
    if(data.containError==TRUE){
        return 0;
    }

    outputManager(&data, filename);
    setDataFree(&data);

    return 1;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: initializes the variables in the Data struct
 * Input:       pointer to Data struct
 * Output:	    nothing
 */
/*----------------------------------------------------------------------------*/
void initializeData(Data *data) {
    int IC = 100;
    int DC = 0;

    /* initialize all the data in the data struct */
    data->tc = 0;
    data->lc = 0;
    data->dc = DC;
    data->ic = IC;
    data->exc = 0;
    data->enc = 0;
    data->dcf =0;

    data->containError = FALSE;

    data->tagArr = NULL;
    data->directiveArr = NULL;
    data->directiveArrIndex = NULL;
    data->entryArr = NULL;
    data->externArr = NULL;
    data->instArr = NULL;
}

/*----------------------------------------------------------------------------*/
/*
 * Description: frees all the dynamically allocated memory in the Data struct
 * Input:       pointer to Data struct
 * Output:	    nothing
 */
/*----------------------------------------------------------------------------*/
void setDataFree(Data * data){
    int i;
    /* free all the data in the data struct */
    free(data->tagArr);
    free(data->directiveArr);

    free(data->entryArr);

    for (i = 0 ; i < data->exc ; i++){
        free(data->externArr[i].icArr);
    }

    free(data->externArr);
    free(data->instArr);

}
