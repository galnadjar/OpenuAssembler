#ifndef MAMAN14_FILEHANDLING_H
#define MAMAN14_FILEHANDLING_H

#include "symbolTable.h"
#include "stdlib.h"
#include "lineParsing.h"
#include "entryExternDataStructures.h"
#include "directiveFuncs.h"
#include "instructionFuncs.h"



#define IC_INITIAL_ADDRESS 100
#define DC_INITIAL_ADDRESS 0
#define EXT_LENGTH 3



void openFile(char*);
void readFile(FILE*,char* fileName);
void resetIterVars(int* wasLabel, char** wordSaved, char** labelName, int* i, int* category);
void addICF(dataImgPtr* dataImgHead,symbolPtr* symbolHead,long ICF);
int analyzeTypeSymbol(labelTablePtr labelPtr, symbolPtr* symbolHead, externTablePtr* externHead, codeImgPtr* codeHead,entryTablePtr* entryHead);
int analyzeEntrySymbol(char* label,entryTablePtr* entryHead,long address);
void secondIteration(symbolPtr* symbolTableHead,entryTablePtr* entryTableHead,labelTablePtr* labelTablehead,
                    codeImgPtr* codeImgHead,dataImgPtr* dataImgHead, long DCF, long ICF,char* fp);

void writeEntry(char* name,entryTablePtr entryPtr);
void writeExtern(char* name, externTablePtr externPtr);
FILE* createWriteFile(char* name,char* extension);

void writeOB(char* name,dataImgPtr dataPtr,codeImgPtr codePtr, long DCF, long ICF);
void printExt(FILE* fp,externTablePtr externPtr);
void printEnt(FILE* fp,entryTablePtr entryPtr);

int analyzeLabelTable(labelTablePtr* labelTablehead, symbolPtr* symbolTableHead,externTablePtr* externTableHead,codeImgPtr* codeHead,entryTablePtr* entryHead);
void printCodeImg(FILE* fp,codeImgPtr codePtr);
void printCounters(FILE* fp ,long IC, long DC);
#endif //MAMAN14_FILEHANDLING_H
