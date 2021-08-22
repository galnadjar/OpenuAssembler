
#ifndef MAMAN14_LABELANALYZE_H
#define MAMAN14_LABELANALYZE_H

/*---------------------------- includes ----------------------------*/

#include "errorsHeader.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/*---------------------------- defines ----------------------------*/

#define LABEL_SEARCH 0
#define INSTRUCTION_SEARCH 1
#define SAVED_WORDS_LENGTH 32
#define INSTRUCTION_WORDS_LENGTH 26


/*-------------------- functions declarations ---------------------*/

int validLabelORInstruction(char* input, int request);

int analyzeLabel(char* lineInput, int i, int line, char** label);


#endif /*MAMAN14_LABELANALYZE_H*/
