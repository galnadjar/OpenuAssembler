#ifndef MAMAN14_SYMBOLTABLE_H
#define MAMAN14_SYMBOLTABLE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "errorsHeader.h"
#include "labelAsArg.h"

typedef enum {
    CODE_AT = 1,
    DATA_AT,
    EXTERN_AT
}attribute;

typedef struct symbol* symbolPtr;


int addSymbol(symbolPtr* symbolHead, char* label, long address, int category, int line); /*first node is not assigned*/
void addNodeToSymbolTable(symbolPtr* head, symbolPtr* desired);
symbolPtr findSymbolLabel(symbolPtr* symbolTableHead, char* word);
int createSymbol(symbolPtr* symbolHead,char* label,long address,int category,int line);
void updateSymbolAddress(symbolPtr* ptr,long num);

char* getSymbolLabel(symbolPtr ptr);
long getSymbolAddress(symbolPtr ptr);
int getSymbolType(symbolPtr ptr);
symbolPtr getSymbolNextNode(symbolPtr ptr);

void setSymbolValues(symbolPtr* ptr,char* label,long address,int category,int line);

#endif //MAMAN14_SYMBOLTABLE_H
