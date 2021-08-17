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
int addDirNodes(dataImgPtr* imgHead, char* word, long* numLst, int dir, long* DC,int line,int counter){

    int i,state = VALID;
    dataImgPtr curr,temp;

    if((*DC) != DC_INITIAL){
        curr = *imgHead;
        for(;curr->next;curr = getNextDataNode(curr)); /*gets the pointer to the end the data list*/
        curr->next = (dataImgPtr) calloc(1,sizeof(dataImg));
        curr = getNextDataNode(curr);}

    else{
        (*imgHead) = (dataImgPtr) calloc(1,sizeof(dataImg));
        curr = *imgHead;}

    if(curr){
        for(i = 0;i < counter && state == VALID;i++,curr = getNextDataNode(curr)){
            curr->address = *DC;
            curr->type = dir;
            curr->line = line;
            fillImgData(dir,&curr,word,numLst,i,DC);

            /*set up for next node*/
            if(dir == ASCIZ_DIR || i+1 < counter){
                temp = (dataImgPtr) calloc(1,sizeof(dataImg));

                if(temp)
                    curr->next = temp;
                else
                    state = ERROR;
            }
        }
        if(numLst)
            free(numLst);
        if(word)
            free(word);

        if(dir == ASCIZ_DIR){
            curr->type = ASCIZ_DIR;
            curr->address = (*DC)++;}
    }

    else
        state = ERROR;

    return state;
}

/*allocate memory to a node and if the allocation was successful,copy a dataNode values and returns 1,otherwise,return -1*/
int copyDataNodeVals(dataImgPtr* dest, dataImgPtr src){

    int state = VALID;

        (*dest) = calloc(1,sizeof(dataImg));
        if(*dest){
            (*dest)->address = src->address;
            (*dest)->type = src->type;
            (*dest)->line = src->line;
            (*dest)->data.display.d1 = src->data.display.d1;
            (*dest)->data.display.d2 = src->data.display.d2;
            (*dest)->data.display.d3 = src->data.display.d3;
            (*dest)->data.display.d4 = src->data.display.d4;
        }
        else
            state = ERROR;

    return state;
}


/*prints the data img to the file based on the type of each node given*/
void printDataDisplay(FILE* fp,dataImgPtr ptr,int* bytesCounter,long* DC,long const DCF){

    switch (ptr->type) {

        case ASCIZ_DIR:
        case DB_DIR:

            fprintf(fp,"%02X",(int)ptr->data.display.d1);
            (*bytesCounter)++;
            newLineOrTab(fp,*bytesCounter,DC,DCF);

            break;

        case DH_DIR:
            fprintf(fp,"%02X",(int)ptr->data.display.d1);
            (*bytesCounter) ++;
            newLineOrTab(fp,*bytesCounter,DC,DCF);

            fprintf(fp,"%02X",(int)ptr->data.display.d2);
            (*bytesCounter) ++;
            newLineOrTab(fp,*bytesCounter,DC,DCF);

            break;

        case DW_DIR:
            fprintf(fp,"%02X",(int)ptr->data.display.d1);
            (*bytesCounter) ++;

            newLineOrTab(fp,*bytesCounter,DC,DCF);
            fprintf(fp,"%02X",(int)ptr->data.display.d2);
            (*bytesCounter) ++;

            newLineOrTab(fp,*bytesCounter,DC,DCF);
            fprintf(fp,"%02X",(int)ptr->data.display.d3);
            (*bytesCounter) ++;

            newLineOrTab(fp,*bytesCounter,DC,DCF);
            fprintf(fp,"%02X",(int)ptr->data.display.d4);
            (*bytesCounter) ++;
            newLineOrTab(fp,*bytesCounter,DC,DCF);

            break;
    }
}


/*fills the data node, in the matching field according to the type of it and raises DC accordingly*/
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

/*a getter for a data address of the current data node*/
long getDataAddress(dataImgPtr ptr){
    return ptr->address;
}

/*a setter for a data address of the current data node*/
void setDataAddress(dataImgPtr* ptr,long address){
    (*ptr)->address = address;
}

/*a getter for the next data node*/
dataImgPtr getNextDataNode(dataImgPtr ptr){
    return ptr->next;
}

/*a function which:
 * prints new line if reached to 4 bytes in a row.
 * if the new DC is lower than DCF also prints the new address.
 * otherwise, prints a tab*/

void newLineOrTab(FILE* fp,const int bytesCounter,long* DC,const long DCF){

    if(bytesCounter % BYTES_LINE_LEN == 0){
        fputc('\n',fp);
        (*DC) += BYTES_LINE_LEN;
        if(*DC < DCF)
            fprintf(fp,"%04ld\t",*DC);}

    else
        fputc('\t',fp);
}