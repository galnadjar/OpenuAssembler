#include "codeImg.h"


typedef struct codeImg{

    long nodeAddress;
    int opcode; /*an op code used to help understand which kind of node is held*/
    char label[MAX_LABEL_LENGTH+1];
    int line;
    codeImgPtr next;

    union {

        struct {

            long : 6;
            unsigned long funct: 5;
            unsigned long rd: 5;
            unsigned long rt: 5;
            unsigned long rs: 5;
            unsigned long opcode: 6;

        } Rcmd;

        struct {

            long immed: 16;
            unsigned long rt: 5;
            unsigned long rs: 5;
            unsigned long opcode: 6;

        } Icmd;

        struct {

            long address: 25;
            unsigned long reg: 1;
            unsigned long opcode: 6;

        } Jcmd;

        struct { /*the display to the screen*/

            unsigned long d1: 8;
            unsigned long d2: 8;
            unsigned long d3: 8;
            unsigned long d4: 8;

        }display;
    }codeData;

}codeImg;



/*returns 1 if allocated properly, otherwise returns 0*/
int addRCodeNode(int rs,int rt,int rd,long IC,int opcode,int funct, codeImgPtr* imgHead,int line){

    int state = VALID;
    codeImgPtr temp = (codeImgPtr) calloc(1,sizeof(codeImg));

    if(temp) {
        codeImgPtr curr = *imgHead;

        temp->nodeAddress = IC;
        temp->opcode = opcode;
        temp->line = line;


        temp->codeData.Rcmd.opcode = opcode;
        temp->codeData.Rcmd.funct = funct;
        temp->codeData.Rcmd.rs = rs;
        temp->codeData.Rcmd.rt = rt;
        temp->codeData.Rcmd.rd = rd;

        if(IC > 100){
            for (; curr->next; curr = curr->next);
            curr->next = temp;}

        else
            *imgHead = temp;
    }
    else
        state = 0;

    return state;
}

/*returns 1 if allocated properly, otherwise returns 0*/
int addJCodeNode(int reg,long IC,long address,int opcode,codeImgPtr* imgHead,char* label,int line){

    int state = 1;
    codeImgPtr temp = (codeImgPtr) calloc(1,sizeof(codeImg));

    if(temp) {
        codeImgPtr curr = *imgHead;
        temp->opcode = opcode;
        temp->nodeAddress = IC;
        temp->line = line;


        temp->codeData.Jcmd.opcode = opcode;
        temp->codeData.Jcmd.reg = reg;

        if(label) /*we would like to copy the label so we will know the ref*/
            strcpy(temp->label,label);

        if(opcode == STOP_OPCODE)
            temp->codeData.Jcmd.address = 0;


        if(IC > 100){
            for(; curr->next; curr = curr->next);
            curr->next = temp;}

        else
            *imgHead = temp;
    }
    else
        state = 0;

    return state;
}

/*returns 1 if allocated properly, otherwise returns 0*/
int addICodeNode(int rs,int rt,long immed,long IC,int opcode,codeImgPtr* imgHead,char* label,int line){

    int state = 1;
    codeImgPtr temp = (codeImgPtr) calloc(1,sizeof(codeImg));

    if(temp) {
        codeImgPtr curr = *imgHead;

        temp->nodeAddress = IC;
        temp->opcode = opcode;
        temp->line = line;


        if(label)
            strcpy(temp->label,label);


        temp->codeData.Icmd.opcode = opcode;
        temp->codeData.Icmd.rs = rs;
        temp->codeData.Icmd.rt = rt;

        if(!label)/*reg and immed number requested*/
            temp->codeData.Icmd.immed = immed; /*reg number*/

        else /*label used*/
            temp->nodeAddress *= (-1);

        if(IC > 100){
            for (; curr->next; curr = curr->next);
            curr->next = temp;
        }

        else
            *imgHead = temp;
    }
    else
        state = 0;

    return state;
}

/*updates the i branch node immed field*/
void updateImmed(codeImgPtr* codeHead,const char* label,long address){

    int state = 1;
    long immed;
    codeImgPtr curr = (*codeHead);

    for(;curr && state;curr = curr->next){
        if(!strcmp(curr->label,label)){
            immed = curr->nodeAddress;
            curr->codeData.Icmd.immed = address + immed;
            state = 0;}
    }
}

int getCodeDisplay(codeImgPtr ptr ,byteSelect byte){

    int byteSelected;

    switch (byte) {

        case A_BYTE:
            byteSelected = (int)ptr->codeData.display.d1;
            break;

        case B_BYTE:
            byteSelected = (int)ptr->codeData.display.d2;
            break;

        case C_BYTE:
            byteSelected = (int)ptr->codeData.display.d3;
            break;

        default:
            byteSelected = (int)ptr->codeData.display.d4;
            break;
    }
    return byteSelected;
}

long getCodeAddress(codeImgPtr ptr){
    return ptr->nodeAddress;
}

codeImgPtr getNextCodeNode(codeImgPtr ptr){
    return ptr->next;
}

int getCodeLine(codeImgPtr ptr){
    return ptr->line;
}