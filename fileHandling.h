#ifndef MAMAN14_FILEHANDLING_H
#define MAMAN14_FILEHANDLING_H

/*---------------------------- includes ----------------------------*/
#include "symbolTable.h"
#include "lineParsing.h"
#include "directiveFuncs.h"
#include "instructionFuncs.h"
#include "printFile.h"


/*---------------------------- defines ----------------------------*/

#define MAX_PROG_SIZE 33554432
#define EXT_LENGTH 3
#define UNDERLINE "\033[4m"
#define CLOSEUNDERLINE "\033[0m"


/*-------------------- functions declarations ---------------------*/


void openFile(char*);

void readFile(FILE*,char* fileName);

void resetIterVars(int* wasLabel, char** wordSaved, char** labelName, int* i, int* category);

void addICF(dataImgPtr* dataImgHead,symbolPtr* symbolHead,long ICF);

void secondIteration(symbolPtr* symbolTableHead,entryTablePtr* entryTableHead,labelTablePtr* labelTablehead,
                    codeImgPtr* codeImgHead,dataImgPtr* dataImgHead,externTablePtr* externHead,long DCF, long ICF,char* fp);

void releaseDataTables(symbolPtr* symbolTableHead,entryTablePtr* entryTableHead,labelTablePtr* labelTableHead,
                       codeImgPtr* codeImgHead,dataImgPtr* dataImgHead,externTablePtr* externTableHead);

void freeSymSt(symbolPtr* ptr);

void freeEntSt(entryTablePtr* ptr);

void freeExtSt(externTablePtr* ptr);

void freeLabelSt(labelTablePtr* ptr);

void freeCodeSt(codeImgPtr* ptr);

void freeDataSt(dataImgPtr* ptr);
#endif //MAMAN14_FILEHANDLING_H