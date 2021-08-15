//
// Created by xubuntu on 7/12/21.
//

#ifndef MAMAN14_LINEPARSING_H
#define MAMAN14_LINEPARSING_H

#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "errorsHeader.h"
#include "symbolTable.h"
#include "spaceParse.h"
#include "labelHandling.h"


#define EMPTY_CATEGORY_FLAG (-1)
#define LABEL_FLAG 0
#define ENTRY_FLAG 1
#define EXTERN_FLAG 2
#define DIRECTIVE_FLAG 3
#define INSTRUCTION_FLAG 4


int handleLabelOrInstruction(int* category,char* word,int line);

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
