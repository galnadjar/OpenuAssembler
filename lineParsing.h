//
// Created by xubuntu on 7/12/21.
//

#ifndef MAMAN14_LINEPARSING_H
#define MAMAN14_LINEPARSING_H

/*---------------------------- includes ----------------------------*/

#include "errorsHeader.h"
#include "symbolTable.h"
#include "spaceParse.h"
#include "labelHandling.h"
#include "entryExternDataStructures.h"
#include "codeImg.h"


/*---------------------------- defines ----------------------------*/

#define EMPTY_CATEGORY_FLAG (-1)
#define LABEL_FLAG 0
#define ENTRY_FLAG 1
#define EXTERN_FLAG 2
#define DIRECTIVE_FLAG 3
#define INSTRUCTION_FLAG 4


/*-------------------- functions declarations ---------------------*/

int handleLabelOrInstruction(int* category,char* word,int line);

int handleEntOrExtCategory(int* i, char* lineInput, char* labelName, int line, int category, entryTablePtr* entryHead, symbolPtr* symHead);

int handleLabelCategory(int* i,char* lineInput,char** labelName,char** wordSaved,int* category,int line);

int analyzeLabelTable(labelTablePtr* labelTablehead, symbolPtr* symbolTableHead,externTablePtr* externTableHead,codeImgPtr* codeHead,entryTablePtr* entryHead);

int analyzeTypeSymbol(labelTablePtr labelPtr, symbolPtr* symbolHead, externTablePtr* externHead, codeImgPtr* codeHead,entryTablePtr* entryHead);

int validDirectiveName(char*);

int parseCategory(int* i, char* lineInput, char** wordSaved, int* category, int line);

int validDotStart(int ,int ,int );

void invalidCategoryArgs(int line, int category);

void labelNameError(int line,char* word);

void directiveCategorySelector(int* category,char* word);

int handleColon(char* word,int* category,int firstLetter,int line,int ch);

int analyzeChar(int* dot,int ch,int* firstLetter,int line);

int handleDirCase(int* category, char* word, int line);


#endif //MAMAN14_LINEPARSING_H
