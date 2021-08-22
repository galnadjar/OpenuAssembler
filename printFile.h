#ifndef MAMAN14_PRINTFILE_H
#define MAMAN14_PRINTFILE_H

/*---------------------------- includes ----------------------------*/

#include "handleEntExt.h"
#include "codeImg.h"
#include "dataImg.h"


/*---------------------------- defines ----------------------------*/

#define IC_INITIAL_ADDRESS 100
#define DC_INITIAL_ADDRESS 0


/*-------------------- functions declarations ---------------------*/

void writeEntry(char* name,entryTablePtr entryPtr);

void writeExtern(char* name, externTablePtr externPtr);

FILE* createWriteFile(char* name,char* extension);

void writeOB(char* name,dataImgPtr dataPtr,codeImgPtr codePtr, long DCF, long ICF);

void printExt(FILE* fp,externTablePtr externPtr);

void printEnt(FILE* fp,entryTablePtr entryPtr);

void printCodeImg(FILE* fp,codeImgPtr codePtr);

void printCounters(FILE* fp ,long IC, long DC);

#endif /*MAMAN14_PRINTFILE_H*/