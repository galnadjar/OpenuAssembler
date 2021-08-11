//
// Created by xubuntu on 7/21/21.
//

#ifndef MAMAN14_ENTRYEXTERNDATASTRUCTURES_H
#define MAMAN14_ENTRYEXTERNDATASTRUCTURES_H
#include "stdlib.h"
#include <string.h>
#include <stdio.h>
#include "errorsHeader.h"

#define UNKNOWN_ADDRESS (-1)

typedef struct entryTable* entryTablePtr;
typedef struct externTable* externTablePtr;



int handleEntNode(char* label,entryTablePtr* entryHead,int line);
entryTablePtr findEntryLabel(entryTablePtr* entryTableHead, char* word);
int addEntry(entryTablePtr* entryHead,char* label,long address,int line);

int addToExtTable(externTablePtr * tableHead, char* word, long address);
int getEntryLine(entryTablePtr ptr);

char* getEntryLabel(entryTablePtr ptr);
char* getExternLabel(externTablePtr ptr);
long getExternSize();
long getExtAddress(externTablePtr externPtr);
long getEntAddress(entryTablePtr ptr);
externTablePtr getNextExtNode(externTablePtr ptr);
entryTablePtr getNextEntNode(entryTablePtr ptr);
void setEntryAddress(entryTablePtr* ptr,long address);






#endif //MAMAN14_ENTRYEXTERNDATASTRUCTURES_H
