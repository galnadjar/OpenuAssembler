#ifndef MAMAN14_SYMBOLTABLE_H
#define MAMAN14_SYMBOLTABLE_H

/*---------------------------- includes ----------------------------*/

#include "errorsHeader.h"
#include "labelAsArg.h"

/*---------------------------- defines ----------------------------*/

typedef enum {
    CODE_AT = 1,
    DATA_AT,
    EXTERN_AT
}attribute;

typedef struct symbol* symbolPtr;


/*-------------------- functions declarations ---------------------*/

int addSymbol(symbolPtr* symbolHead, char* label, long address, int category, int line);

void addNodeToSymbolTable(symbolPtr* head, symbolPtr* desired);

symbolPtr findSymbol(symbolPtr* symbolTableHead, char* word);

int createSymbol(symbolPtr* symbolHead,char* label,long address,int category,int line);

void updateSymbolAddress(symbolPtr* ptr,long num);

char* getSymbolLabel(symbolPtr ptr);

long getSymbolAddress(symbolPtr ptr);

int getSymbolType(symbolPtr ptr);

symbolPtr getSymbolNextNode(symbolPtr ptr);

void setSymbolValues(symbolPtr* ptr,char* label,long address,int category,int line);

#endif //MAMAN14_SYMBOLTABLE_H
