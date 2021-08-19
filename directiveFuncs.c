#include "directiveFuncs.h"

/*a function that gets the current directive and turns it into numeric value
 * returns the directive value based on the type given*/
int getDir(char* directive){

    int dir;

    if(!strcmp(".asciz",directive))
        dir = ASCIZ_DIR;

    else if(!strcmp(".db",directive))
        dir = DB_DIR;

    else if(!strcmp(".dh",directive))
        dir = DH_DIR;

    else/*case its dw*/
    dir = DW_DIR;

    return dir;
}


/* a function that analyzes the directive arguments based on the directive given,
 * if not enough memory available for saving the inputs or if the directive args were invalid return 0
 * otherwise returns 1*/
int checkDirArgs(char* lineInput,char* directive, int i,int line,long* DC,dataImgPtr* ImgHead){

    int dir = getDir(directive),state = VALID;
    char* word = NULL;
    long* numLst = NULL;
    int wordCnt,numberCnt = 0;

    switch (dir) {

        case ASCIZ_DIR:
            word = calloc(1,strlen(lineInput)-1);
            state = analAsciz(i, lineInput, line, &word);
            wordCnt = (int)strlen(word);
            break;

            case DB_DIR:
            case DW_DIR:
            default: /*case DH_DIR*/
                numLst = (long*) calloc(strlen(lineInput)-1,sizeof(long));
                state = analNumLst(i, lineInput, line, &numLst, dir,&numberCnt);
                break;
    }

    if(state != ERROR){
        state = addDirNodes(ImgHead, word, numLst, dir, DC,line,(dir == ASCIZ_DIR?wordCnt:numberCnt));
        if(state == ERROR)
            ERROR_MEMORY_MAXED_OUT(line);}


    if(dir == ASCIZ_DIR)
        free(word);
    else
        free(numLst);

    return state;
}


/*analyzes asciz string, if the analysis was valid return 1, otherwise returns -1*/
int analAsciz(int i, char* lineInput, int line, char** word){

    int state = VALID,wordInd = 0,ch;
    i = findQuotation(lineInput, i, line);

    if(i){
        for(;i < strlen(lineInput) -1 && state;i++){
            ch = (int)lineInput[i];

            if(ch == '"')
                state = EXIT;

            else{
                if(isprint(ch))
                    (*word)[wordInd++] = (char)ch;
                else{
                    state = ERROR;
                    ERROR_INVALID_ASCIZ(line);}
            }
        }

        if(i == strlen(lineInput) -1 && state == VALID){ /*reached the end without finding closing quotation mark */
            ERROR_MISSING_QUOTATION(line);
            state = ERROR;}

        else if(locAfterSpace(lineInput,i) != strlen(lineInput)-1){
            ERROR_EXTRANEOUS_END_OF_CMD(line);
            state = ERROR;}
    }
    else
        state = ERROR;

    if(state == EXIT)
        state = VALID;

    return state;
}


/* analyzes numbers ,given as an argument, to a directive which is different than asciz,
 * if the numbers corresponds to size of the type of variable holding them, saves them into a list and returns 1
 * otherwise returns -1*/
int analNumLst(int i, char* lineInput, int line, long** numLst, int dir,int* numberCnt){

    int numInd = 0,state = VALID;
    long num,minVal,maxVal;
    int comma = 0;

    if(*numLst){
        adjustValues(&minVal,&maxVal,dir);

        for(;i < strlen(lineInput) -1 && state == VALID;i++){
            i = checkAndSetNum(lineInput,i,line,&num,1,minVal,maxVal,&comma); /*looks for first number and save it if valid*/
            if(i != ERROR){
                (*numberCnt)++;
                state = checkLastNum(lineInput,i,line,comma);
                if(state == EXIT)
                    (*numLst)[numInd] = num;

                else{
                    i = commaAfterSpace(lineInput,i); /*looks for comma since its not the last number*/
                    if(i > 0){ /*a comma was found, then we can add the number*/
                        (*numLst)[numInd++] = num;
                        i--;
                    }

                    else{
                        ERROR_MISSING_COMMA(line);
                        state = ERROR;}
                }
            }
            else
                state = ERROR;
        }
        if(state == EXIT)
            state = VALID;
    }

    else{
        state = ERROR;
        ERROR_MEMORY_MAXED_OUT(line);
    }
    return state;
}


/*check if the current saved num is the last one,
 * if it's not the last number returns 1,
 * if it is the last number and a comma wasn't saved returns 0
 * ,if last and ended with comma returns -1*/
int checkLastNum(char* lineInput,int i,int line,int comma){
    int state = VALID;

    if(locAfterSpace(lineInput,i+1) >= strlen(lineInput) -1){ /*is the last number*/
        if(!comma){/*check if the last number ended with a comma*/
            state = EXIT;}

        else{
            ERROR_EXTRANEOUS_END_OF_CMD(line);
            state = ERROR;
        }
    }
    return state;
}