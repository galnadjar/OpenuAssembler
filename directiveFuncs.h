//
// Created by xubuntu on 8/3/21.
//

#ifndef MAMAN14_DIRECTIVEFUNCS_H
#define MAMAN14_DIRECTIVEFUNCS_H

#include "errorsHeader.h"
#include "dataImg.h"
#include "spaceParse.h"
#include "handleNumbers.h"



#define MAX_ROW_LENGTH 81



int analNumLst(int i, char* lineInput, int line, long** numLst, int dir);
int getDir(char* directive);
int checkDirArgs(char* lineInput,char* directive,int i,int line,long* DC,dataImgPtr* ImgHead);

int analAsciz(int i,char* lineInput, int line,char** word);
int checkLastNum(char* lineInput,int i,int line);

#endif //MAMAN14_DIRECTIVEFUNCS_H