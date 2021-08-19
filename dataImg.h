#ifndef MAMAN14_DATAIMG_H
#define MAMAN14_DATAIMG_H

/*---------------------------- includes ----------------------------*/

#include <stdlib.h>
#include "errorsHeader.h"
#include "handleNumbers.h"


/*---------------------------- defines ----------------------------*/

#define DC_INITIAL 0
#define BYTES_LINE_LEN 4
#define WORD_SIZE 4
#define HALF_WORD_SIZE 2


/*-------------------- functions declarations ---------------------*/

typedef struct dataImg* dataImgPtr;

void printDataImg(FILE* fp,dataImgPtr dataPtr,long ICF,long DCF);

void fillImgData(int dir,dataImgPtr* curr,const char* word,const long* numLst,int i,long* DC);

int addDirNodes(dataImgPtr* imgHead, char* word, long* numLst, int dir, long* DC,int line,int count);

void printDataDisplay(FILE* fp,dataImgPtr ptr,int* bytesCounter,long* DC,long DCF);

void setDataAddress(dataImgPtr*,long address);

long getDataAddress(dataImgPtr ptr);

dataImgPtr getNextDataNode(dataImgPtr ptr);

void newLineOrTab(FILE* fp,int bytesCounter,long* DC,long DCF);

#endif //MAMAN14_DATAIMG_H
