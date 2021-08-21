
#ifndef MAMAN14_HANDLEENTEXT_H
#define MAMAN14_HANDLEENTEXT_H

/*---------------------------- includes ----------------------------*/

#include <string.h>
#include <stdlib.h>
#include "errorsHeader.h"


typedef struct entryTable* entryTablePtr;
typedef struct externTable* externTablePtr;


/*-------------------- functions declarations ---------------------*/

int analyzeEntrySymbol(char* label,entryTablePtr* entryHead,long address);

int handleEntNode(char* label,entryTablePtr* entryHead,int line);

entryTablePtr findEntryLabel(entryTablePtr* entryTableHead, char* word);

int addEntry(entryTablePtr* entryHead,char* label,int line);

int addToExtTable(externTablePtr * tableHead, char* word, long address,int line);

char* getEntryLabel(entryTablePtr ptr);

char* getExternLabel(externTablePtr ptr);

long getExtAddress(externTablePtr externPtr);

long getEntAddress(entryTablePtr ptr);

externTablePtr getNextExtNode(externTablePtr ptr);

entryTablePtr getNextEntNode(entryTablePtr ptr);

void setEntryAddress(entryTablePtr* ptr,long address);

#endif //MAMAN14_HANDLEENTEXT_H