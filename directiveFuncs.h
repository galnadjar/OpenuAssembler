#ifndef MAMAN14_DIRECTIVEFUNCS_H
#define MAMAN14_DIRECTIVEFUNCS_H

/*---------------------------- includes ----------------------------*/

#include "errorsHeader.h"
#include "dataImg.h"
#include "handleNumbers.h"



/*-------------------- functions declarations ---------------------*/

int analNumLst(int i, char* lineInput, int line, long** numLst, int dir,int* numberCnt);

int getDir(char* directive);

int checkDirArgs(char* lineInput,char* directive,int i,int line,long* DC,dataImgPtr* ImgHead);

int analAsciz(int i,char* lineInput, int line,char** word);

int checkLastNum(char* lineInput,int i,int line,int comma);

#endif //MAMAN14_DIRECTIVEFUNCS_H