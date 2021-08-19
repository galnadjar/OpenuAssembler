#include "codeImg.h"


/*structure for the code img*/

typedef struct codeImg{

    long nodeAddress;
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



/*a function that adds R-Type code nodes to the Rcmd inner structs of codeImg
 * returns 1 if allocated properly, otherwise returns 0*/
int addRCodeNode(int rs,int rt,int rd,long IC,int opcode,int funct, codeImgPtr* imgHead,int line){

    int state = VALID;
    codeImgPtr temp = (codeImgPtr) calloc(1,sizeof(codeImg));

    if(temp) {
        codeImgPtr curr = *imgHead;

        temp->nodeAddress = IC;
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


/*a function that adds J-Type code nodes to the Jcmd inner structs of codeImg
 * returns 1 if allocated properly, otherwise returns 0*/
int addJCodeNode(int reg,long IC,long address,int opcode,codeImgPtr* imgHead,char* label,int line){

    int state = 1;
    codeImgPtr temp = (codeImgPtr) calloc(1,sizeof(codeImg));

    if(temp) {
        codeImgPtr curr = *imgHead;
        temp->nodeAddress = IC;
        temp->line = line;
        temp->codeData.Jcmd.address = address;
        temp->codeData.Jcmd.opcode = opcode;
        temp->codeData.Jcmd.reg = reg;

        if(opcode >= J_CMDS_WITH_LABEL_MIN_OPCODE && opcode <= J_CMDS_WITH_LABEL_MAX_OPCODE && !reg) /*J cmd's that uses a label*/
            strcpy(temp->label,label);


        if(*imgHead){
            for(;curr->next; curr = curr->next);
            curr->next = temp;}

        else
            *imgHead = temp;
    }
    else
        state = 0;

    return state;
}


/*a function that adds I-Type code nodes to the Icmd inner structs of codeImg
* returns 1 if allocated properly, otherwise returns 0*/
int addICodeNode(int rs,int rt,long immed,long IC,int opcode,codeImgPtr* imgHead,char* label,int line){

    int state = 1;
    codeImgPtr temp = (codeImgPtr) calloc(1,sizeof(codeImg));

    if(temp) {
        codeImgPtr curr = *imgHead;

        temp->nodeAddress = IC;
        temp->line = line;
        temp->codeData.Icmd.opcode = opcode;
        temp->codeData.Icmd.rs = rs;
        temp->codeData.Icmd.rt = rt;

        if(opcode >= I_BRANCHING_MIN_OPCODE && opcode <= I_BRANCHING_MAX_OPCODE)
            strcpy(temp->label,label);

        else
            temp->codeData.Icmd.immed = immed; /*reg number*/


        if(IC > 100){
            for (;curr->next; curr = curr->next);
            curr->next = temp;
        }

        else
            *imgHead = temp;
    }
    else
        state = 0;

    return state;
}


/*updates the i branch node immed field, if was valid update returns 1,otherwise returns -1*/
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

/*updates the j branching address field if the leaping destination is valid and returns 1, otherwise returns -1*/
int updateJbranching(codeImgPtr* codeHead,char* label, long address){

    int state = VALID;
    codeImgPtr curr = (*codeHead);

    for(;curr && state == VALID;curr = curr->next){
        if(!strcmp(curr->label,label)){
            if(address <= MAX_JUMP_LENGTH){
                curr->codeData.Jcmd.address = address;
                state = EXIT;}

            else{
                state = ERROR;
                ERROR_JUMP_OUT_OF_BOUNDS(curr->line);
            }
        }
    }

    if(state == EXIT)
        state = 1;

    return state;
}


/*gets the code display of the current code node*/
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


/*gets the current node of codeImg address*/
long getCodeAddress(codeImgPtr ptr){
    return ptr->nodeAddress;
}


/*gets the next node to the given node of codeImg*/
codeImgPtr getNextCodeNode(codeImgPtr ptr){
    return ptr->next;
}


/*gets the current codeImg node line*/
int getCodeLine(codeImgPtr ptr){
    return ptr->line;
}