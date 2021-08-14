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
        temp->codeData.Jcmd.address = address;
        temp->codeData.Jcmd.opcode = opcode;
        temp->codeData.Jcmd.reg = reg;

        if(opcode >= J_CMDS_WITH_LABEL_MIN_OPCODE && opcode <= J_CMDS_WITH_LABEL_MAX_OPCODE && !reg) /*J cmd's that uses a label*/
            strcpy(temp->label,label);


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
        temp->codeData.Icmd.opcode = opcode;
        temp->codeData.Icmd.rs = rs;
        temp->codeData.Icmd.rt = rt;

        if(opcode >= I_BRANCHING_MIN_OPCODE && opcode <= I_BRANCHING_MAX_OPCODE)
            strcpy(temp->label,label);

        else
            temp->codeData.Icmd.immed = immed; /*reg number*/


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


/*updates the i branch node immed field, if was valid update returns 1,otherwise returns 0*/
int updateImmed(codeImgPtr* codeHead,const char* label,long dest){

    int state = VALID;
    long src;
    codeImgPtr curr = (*codeHead);

    for(;curr && state == VALID;curr = curr->next){
        if(!strcmp(curr->label,label)){
            src = curr->nodeAddress;
            state = checkTwoByteSize(dest - src);
            if(state)
                curr->codeData.Icmd.immed = dest - src;
            else{
                ERROR_JUMP_OUT_OF_BOUNDS(getCodeLine(curr));
                state = ERROR;}

            if(state == VALID)/*valid label was found and immed was updated properly*/
                state = EXIT;}
    }

    if(state == EXIT)
        state = 1;

    return state;
}

int updateJbranching(codeImgPtr* codeHead,char* label, long address){

    int state = VALID;
    codeImgPtr curr = (*codeHead);

    for(;curr && state == VALID;curr = curr->next){
        if(!strcmp(curr->label,label)){
            if(address <= MAX_JUMP_LENGTH)
                curr->codeData.Jcmd.address = address;
            else{
                state = ERROR;
                ERROR_JUMP_OUT_OF_BOUNDS(curr->line);
            }

            state = EXIT;
        }
    }

    if(state == EXIT)
        state = 1;

    return state;

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