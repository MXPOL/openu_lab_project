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
    data->wc = 0;

    /*
    data->instArr = NULL;
    data->wordArr = NULL;
    */
    data->containError = FALSE;
    data->tagArr = NULL;
    data->directiveArr = NULL;
    data->entryArr = NULL;
    data->externArr = NULL;

}

/*----------------------------------------------------------------------------*/
/*
 * Description: frees all the dynamically allocated memory in the Data struct
 * Input:       pointer to Data struct
 * Output:	    nothing
 */
/*----------------------------------------------------------------------------*/
void setDataFree(Data * data){
    /* free all the data in the data struct */
    free(data->tagArr);
    free(data->directiveArr);
    free(data->externArr);
    free(data->entryArr);
    /*
    free(data->instArr);
    free(data->wordArr);
    */
}
