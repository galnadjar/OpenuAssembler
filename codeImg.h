//
// Created by xubuntu on 7/22/21.
//

#ifndef MAMAN14_CODEIMG_H
#define MAMAN14_CODEIMG_H

#include <string.h>
#include <stdlib.h>
#include "errorsHeader.h"
#include "handleNumbers.h"

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
int updateImmed(codeImgPtr* codeHead,const char* label,long dest);
int updateJbranching(codeImgPtr* codeHead,char* label, long address);
int getCodeDisplay(codeImgPtr,byteSelect);
long getCodeAddress(codeImgPtr ptr);
int getCodeLine(codeImgPtr ptr);
codeImgPtr getNextCodeNode(codeImgPtr ptr);


#define MIN_REG_NUM 0
#define MAX_REG_NUM 31
#define I_BRANCHING_MIN_OPCODE 15
#define I_BRANCHING_MAX_OPCODE 18
#define J_CMDS_WITH_LABEL_MIN_OPCODE 30
#define J_CMDS_WITH_LABEL_MAX_OPCODE 32
#define MAX_JUMP_LENGTH (33554431)

#endif //MAMAN14_CODEIMG_H
