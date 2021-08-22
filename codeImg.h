#ifndef MAMAN14_CODEIMG_H
#define MAMAN14_CODEIMG_H

/*---------------------------- includes ----------------------------*/
#include "errorsHeader.h"
#include "handleNumbers.h"
#include <stdlib.h>

typedef enum {
    A_BYTE,
    B_BYTE,
    C_BYTE,
    D_BYTE
}byteSelect;


/*---------------------------- defines ----------------------------*/

#define MIN_REG_NUM 0
#define MAX_REG_NUM 31
#define I_BRANCHING_MIN_OPCODE 15
#define I_BRANCHING_MAX_OPCODE 18
#define J_CMDS_WITH_LABEL_MIN_OPCODE 30
#define J_CMDS_WITH_LABEL_MAX_OPCODE 32
#define MAX_JUMP_LENGTH (33554431)


/*-------------------- functions declarations ---------------------*/

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

#endif /*MAMAN14_CODEIMG_H*/
