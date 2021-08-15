//
// Created by xubuntu on 7/22/21.
//

#include "dataImg.h"


typedef struct dataImg{

    long address;
    int line;
    dataType type;
    dataImgPtr next;

    union {
        struct {
            char ch;
        }letter;

        struct{
            int intNum:16;
        }halfWord;

        struct{ /*32*/
            long longNum;
        }word;

        struct { /*the display to the screen*/

            unsigned long d1: 8;
            unsigned long d2: 8;
            unsigned long d3: 8;
            unsigned long d4: 8;

        }display;
    }data;
}dataImg;

/*adds directive nodes,returns 1 if memory allocated properly,and -1 otherwise*/
int addDirNodes(dataImgPtr* imgHead, char* word, long* numLst, int dir, long* DC,int line){

    int i,state = VALID,dcBefore = *DC;
    dataImgPtr arrayHead = (dataImgPtr) calloc(1, sizeof(dataImg));
    dataImgPtr curr = arrayHead;
    dataImgPtr temp;

    if(arrayHead){
        /*for asciz: until the word list ends. for the rest: until the num list ends*/
        for(i = 0;(dir == ASCIZ_DIR? word[i]:numLst[i]) && state == VALID;i++,curr = curr->next){
            curr->address = *DC;
            curr->type = dir;
            curr->line = line;
            fillImgData(dir,&curr,word,numLst,i,DC);

            /*set up for next node*/
            temp = (dataImgPtr) calloc(1,sizeof(dataImg));

            if(temp)
                curr->next = temp;
            else
                state = ERROR;
        }
        if(numLst)
            free(numLst);
        if(word)
            free(word);

        if(dir == ASCIZ_DIR){
            curr->type = ASCIZ_DIR;
            curr->address = *DC;
            (*DC)++;}

        else
            free(temp);

        if(dcBefore == DC_INITIAL)
            (*imgHead) = arrayHead;

        else{
            curr = *imgHead;
            for(;curr->next;curr = curr->next); /*gets the pointer to the end of the list*/
            curr->next = arrayHead;}
    }

    else
        state = ERROR;

    return state;
}

void printDataDisplay(FILE* fp,dataImgPtr ptr,int* bytesCounter,long* DC){

    switch (ptr->type) {

        case ASCIZ_DIR:
        case DB_DIR:

            fprintf(fp,"%02X",(int)ptr->data.display.d1);
            (*bytesCounter)++;
            newLineOrTab(fp,*bytesCounter,DC);

            break;

        case DH_DIR:
            fprintf(fp,"%02X",(int)ptr->data.display.d1);
            (*bytesCounter) ++;
            newLineOrTab(fp,*bytesCounter,DC);

            fprintf(fp,"%02X",(int)ptr->data.display.d2);
            (*bytesCounter) ++;
            newLineOrTab(fp,*bytesCounter,DC);

            break;

        case DW_DIR:
            fprintf(fp,"%02X",(int)ptr->data.display.d1);
            (*bytesCounter) ++;

            newLineOrTab(fp,*bytesCounter,DC);
            fprintf(fp,"%02X",(int)ptr->data.display.d2);
            (*bytesCounter) ++;

            newLineOrTab(fp,*bytesCounter,DC);
            fprintf(fp,"%02X",(int)ptr->data.display.d3);
            (*bytesCounter) ++;

            newLineOrTab(fp,*bytesCounter,DC);
            fprintf(fp,"%02X",(int)ptr->data.display.d4);
            (*bytesCounter) ++;
            newLineOrTab(fp,*bytesCounter,DC);

            break;
    }
}

void fillImgData(int dir,dataImgPtr* curr,const char* word,const long* numLst,int i,long* DC){
    switch (dir) {

        case DB_DIR:
            (*curr)->data.letter.ch = (char)numLst[i];
            (*DC)++;
            break;

        case DH_DIR:
            (*curr)->data.halfWord.intNum = numLst[i];
            (*DC)+= HALF_WORD_SIZE;
            break;

        case DW_DIR:
            (*curr)->data.word.longNum = numLst[i];
            (*DC)+= WORD_SIZE;
            break;

        default: /*means case asciz*/
        (*curr)->data.letter.ch = (char)word[i];
        (*DC)++;
        break;
    }
}

long getDataAddress(dataImgPtr ptr){
    return ptr->address;
}

void setDataAddress(dataImgPtr* ptr,long address){
    (*ptr)->address = address;
}

dataImgPtr getNextDataNode(dataImgPtr ptr){
    return ptr->next;
}

long getDirType(dataImgPtr ptr){
    return ptr->type;
}
void newLineOrTab(FILE* fp,const int bytesCounter,long* DC){

    if(bytesCounter % BYTES_LINE_LEN == 0){
        fputc('\n',fp);
        (*DC) += BYTES_LINE_LEN;
        fprintf(fp,"%04ld\t",*DC);}

    else
        fputc('\t',fp);
}