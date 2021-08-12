//
// Created by xubuntu on 8/4/21.
//

#ifndef MAMAN14_LABELHANDLING_H
#define MAMAN14_LABELHANDLING_H

#include "errorsHeader.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define LABEL_SEARCH 0
#define INSTRUCTION_SEARCH 1
#define SAVED_WORDS_LENGTH 32
#define INSTRUCTION_WORDS_LENGTH 26
#define MAX_LABEL_LENGTH 31
#define MAX_ROW_LENGTH 81

int validLabelORInstruction(char* input, int request);
int analyzeLabel(char* lineInput, int i, int line, char** label);


#endif //MAMAN14_LABELHANDLING_H
