//
// Created by xubuntu on 7/22/21.
//

#ifndef MAMAN14_CODEIMG_H
#define MAMAN14_CODEIMG_H

#include <string.h>
#include <stdlib.h>
#include "errorsHeader.h"

typedef enum {
    A_BYTE,
    B_BYTE,
    C_BYTE,
    D_BYTE
}byteSelect;


typedef struct codeImg* codeImgPtr;

int addRCodeNode(int rs,int rt,int rd,long IC,int opcode,int funct, codeImgPtr* imgHead,int line);
int addICodeNode(int rs,int rt,long immed,long IC,int opcode,codeImgPtr* imgHead,char* label,int line);
int addJCodeNode(int reg,long IC,long address,int opcode,codeImgPtr* imgHead,char* label,int line);
void updateImmed(codeImgPtr* codeHead,const char* label,long address);
int getCodeDisplay(codeImgPtr,byteSelect);
long getCodeAddress(codeImgPtr ptr);
int getCodeLine(codeImgPtr ptr);
codeImgPtr getNextCodeNode(codeImgPtr ptr);


#define UNKNOWN_ADDRESS (-1)
#define STOP_OPCODE 63

#endif //MAMAN14_CODEIMG_H
