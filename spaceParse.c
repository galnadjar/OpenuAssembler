#include "spaceParse.h"

/*return 1 if emptyLine and 0 if not empty*/
int isEmptyLine(char* lineInput){

    int ch;
    int state = VALID,countCh = 0;
    int i = 0;

    while(state == VALID && i < strlen(lineInput)-1){
        ch = (int)lineInput[i];
        if(!countCh && ch == ';') /*if its the first actual letter and its ; it means its a comment line*/
            state = EXIT;

        else if(isspace(ch))
            i++;

        else{ /*found something that isn't a white spacing / ';' char*/
            state = ERROR;
            countCh++;
        }
    }
    if(i == strlen(lineInput)-1 && state == VALID)
        state = EXIT;

    return !state;
}

/*returns the first letter encountered / end of line*/
int locAfterSpace(char* lineInput,int i){
    int state = VALID,ch;

    while(state == VALID && i < strlen(lineInput)-1){
        ch = (int)lineInput[i];

        if(isspace(ch))
            i++;
        else
            state = EXIT;
    }
    return i;
}

/*returns the first index of ',' if found,otherwise if another char returns 0 ,if no comma found until end of line returns -1 */
int commaAfterSpace(char* lineInput, int i){

    int state = VALID;
    int ch;
    for(;i < MAX_ROW_LENGTH-1 && state == VALID;i++) {

        ch = (int) lineInput[i];
        if (isspace(ch))
            i = locAfterSpace(lineInput, i) - 1;

        else if (ch == ',')
            state = EXIT;

        else { /*theres a char which is not a space / comma*/
            i = 0;
            state = ERROR;
        }
    }
    if(i == strlen(lineInput) -1 && state == VALID) /*didn't find comma*/
        i = ERROR;

    return i;
}

/*look for quotation as requested, if found returns the index,otherwise returns 0*/
int findQuotation(char* lineInput, int i, int line){

    int state = VALID,ch;

    for(;i < strlen(lineInput) -1 && state == VALID;i++){
        ch = (int)lineInput[i];

        if(ch == '"')
            state = EXIT;

        else if(isspace(ch))
            i = locAfterSpace(lineInput,i)-1;

        else
            state = ERROR;
    }
    if(i == strlen(lineInput) -1 || state == ERROR){
        ERROR_MISSING_QUOTATION(line);
        i = 0;}

    return i;
}