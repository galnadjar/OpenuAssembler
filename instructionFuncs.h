#ifndef MAMAN14_INSTRUCTIONFUNCS_H
#define MAMAN14_INSTRUCTIONFUNCS_H

#include "spaceParse.h"
#include "codeImg.h"
#include "handleNumbers.h"
#include "labelAnalyze.h"
#include "labelAsArg.h"

#define ADD_FUNCT 1
#define SUB_FUNCT 2
#define AND_FUNCT 3
#define OR_FUNCT 4
#define NOR_FUNCT 5
#define MOVE_FUNCT 1
#define MVHI_FUNCT 2
#define MVLO_FUNCT 3

#define REG_OPTION 0
#define LABEL_OPTION 1
#define I_CMD_MIN_OPCODE 10
#define I_CMD_MAX_OPCODE 24


enum opcode {logic_arit,copyOC,addi = 10,subi,andi,ori,nori,bne,beq,blt,bgt,lb,sb,lw,sw,lh,sh,jmp = 30,la,call,stop = 63};
typedef enum opcode opcode;


int getOpCode(char* instruction);

int getFunct(char* instruction,int opcode);

int analJmp(char* lineInput,int i,int line,int* reg,long* address,char** label);

int setRcmdReg(char* lineInput,int i,int line,int* rs,int* rt,int* rd,int opcode);

int setJcmdReg(char* lineInput,int i,int line,int* reg,long* address,int opcode,char** label);

int checkInsArgs(char* lineInput,char* instruction,int i,int line,long* IC,codeImgPtr* imgHead,labelTablePtr* labelTableHead);

int handleRargs(char* lineInput, int i, int line,const long* IC,int opcode,int funct,codeImgPtr* imgHead);

int handleJargs(char* lineInput, int i, int line,const long* IC,int opcode,codeImgPtr* imgHead,labelTablePtr* labelTableHead);

int handleIargs(char* lineInput, int i, int line,const long* IC,int opcode,codeImgPtr* imgHead,labelTablePtr* labelTableHead);

int setIcmdWithLabel(char* lineInput,int i,int line,int* rs,int* rt,char** label);

int setIcmdWithoutLabel(char* lineInput,int i,int line,int* rs,long* immed,int* rt);

int checkAndSetReg(char* lineInput, int i, int line,int* reg,int commaReq);

int handleRegSpace(int digit,int* reg,int regNum,int line);

int handleRegDigit(int regSign,int* digit,int* regNum,int ch,int line);

int handleRegComma(int* reg,int regNum,int line,int ch,int digit,int regSign, int commaReq);

int checkFirstLetter(char* lineInput,int* i);

int checkRegEndCases(int digit,int* reg,int regNum,int line,int regSign);


#endif /*MAMAN14_INSTRUCTIONFUNCS_H*/