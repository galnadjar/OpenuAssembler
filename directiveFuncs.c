//
// Created by xubuntu on 8/3/21.
//

#include "directiveFuncs.h"
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

/* if not enough memory available or directive args were invalid return 0 otherwise return 1*/
int checkDirArgs(char* lineInput,char* directive, int i,int line,long* DC,dataImgPtr* ImgHead){

    int dir = getDir(directive),state = VALID;
    char* word;
    long* numLst = NULL;

    switch (dir) {

        case ASCIZ_DIR:
            state = analAsciz(i, lineInput, line, &word);
            break;

            case DB_DIR:
            case DW_DIR:
            default: /*case DH_DIR*/
                state = analNumLst(i, lineInput, line, &numLst, dir);
                break;
    }

    if(state){
        state = addDirNodes(ImgHead, word, numLst, dir, DC,line);
        if(state == ERROR){
            state = 0;
            ERROR_MEMORY_MAXED_OUT(line);}
    }
    return state;
}

/*analyze asciz string, if the analysis was valid return 1, otherwise returns 0*/
int analAsciz(int i, char* lineInput, int line, char** word){

    int state = VALID,wordInd = 0,ch,escapeChar = 0;
    (*word) = calloc(1,strlen(lineInput)-1);
    i = findQuotation(lineInput, i, line);

    if(i){
        for(;i < strlen(lineInput) -1 && state;i++){
            ch = (int)lineInput[i];

            if(ch == '"')
                state = EXIT;
            /*todo handle the asciz escape char*/
//            else if(ch == '\')
            else
                (*word)[wordInd++] = (char)ch;
        }

        if(i == strlen(lineInput) -1 && state == VALID){ /*reached the end without finding quotation mark*/
            state = ERROR;
            ERROR_MISSING_QUOTATION(line);}
    }
    else
        state = ERROR;

    if(state == ERROR)
        state = 0;

    else
        state = VALID;

    return state;
}

/*returns 1 if was valid operation, otherwise 0*/
int analNumLst(int i, char* lineInput, int line, long** numLst, int dir){
    (*numLst) = (long*) calloc(strlen(lineInput)-1,sizeof(long));
    int numInd = 0,state = VALID;
    long num,minVal,maxVal;
    adjustValues(&minVal,&maxVal,dir);

    for(;i < strlen(lineInput) -1 && state == VALID;i++){
        i = checkAndSetNum(lineInput,i,line,&num,1,minVal,maxVal); /*looks for first number and save it if valid*/
        if(i != ERROR){

            state = checkLastNum(lineInput,i,line);
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
    }
    if(state == EXIT)
        state = VALID;

    else
        state = 0;

    return state;
}


/*check if last num,if not returns 1,if last and ended with comma returns -1,otherwise if proper last number returns 0*/
int checkLastNum(char* lineInput,int i,int line){
    int state = VALID;

    if(locAfterSpace(lineInput,i+1) >= strlen(lineInput) -1){ /*is the last number*/
        if(lineInput[i] != ','){/*check if the last number ended with a comma*/
            state = EXIT;}

        else{
            ERROR_EXTRANEOUS_END_OF_CMD(line);
            state = ERROR;
        }
    }
    return state;
}