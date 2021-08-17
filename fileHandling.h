#ifndef MAMAN14_FILEHANDLING_H
#define MAMAN14_FILEHANDLING_H

#include "symbolTable.h"
#include "stdlib.h"
#include "lineParsing.h"
#include "entryExternDataStructures.h"
#include "directiveFuncs.h"
#include "instructionFuncs.h"


#define MAX_PROG_SIZE 33554432
#define IC_INITIAL_ADDRESS 100
#define DC_INITIAL_ADDRESS 0
#define EXT_LENGTH 3



void openFile(char*);
void readFile(FILE*,char* fileName);
void resetIterVars(int* wasLabel, char** wordSaved, char** labelName, int* i, int* category);
void addICF(dataImgPtr* dataImgHead,symbolPtr* symbolHead,long ICF);
int handleEntOrExtCategory(int* i, char* lineInput, char* labelName, int line, int category, entryTablePtr* entryHead, symbolPtr* symHead);
int handleLabelCategory(int* i,char* lineInput,char** labelName,char** wordSaved,int* category,int line);
int analyzeTypeSymbol(labelTablePtr labelPtr, symbolPtr* symbolHead, externTablePtr* externHead, codeImgPtr* codeHead,entryTablePtr* entryHead);
int analyzeEntrySymbol(char* label,entryTablePtr* entryHead,long address);
void secondIteration(symbolPtr* symbolTableHead,entryTablePtr* entryTableHead,labelTablePtr* labelTablehead,
                    codeImgPtr* codeImgHead,dataImgPtr* dataImgHead,externTablePtr* externHead,long DCF, long ICF,char* fp);

void writeEntry(char* name,entryTablePtr entryPtr);
void writeExtern(char* name, externTablePtr externPtr);
FILE* createWriteFile(char* name,char* extension);

void writeOB(char* name,dataImgPtr dataPtr,codeImgPtr codePtr, long DCF, long ICF);
void printExt(FILE* fp,externTablePtr externPtr);
void printEnt(FILE* fp,entryTablePtr entryPtr);

int analyzeLabelTable(labelTablePtr* labelTablehead, symbolPtr* symbolTableHead,externTablePtr* externTableHead,codeImgPtr* codeHead,entryTablePtr* entryHead);
void printCodeImg(FILE* fp,codeImgPtr codePtr);
void printCounters(FILE* fp ,long IC, long DC);

void releaseDataTables(symbolPtr* symbolTableHead,entryTablePtr* entryTableHead,labelTablePtr* labelTableHead,
                       codeImgPtr* codeImgHead,dataImgPtr* dataImgHead,externTablePtr* externTableHead);

void freeSymSt(symbolPtr* ptr);

void freeEntSt(entryTablePtr* ptr);

void freeExtSt(externTablePtr* ptr);

void freeLabelSt(labelTablePtr* ptr);

void freeCodeSt(codeImgPtr* ptr);

void freeDataSt(dataImgPtr* ptr);
#endif //MAMAN14_FILEHANDLING_H