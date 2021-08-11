#ifndef MAMAN14_SPACEPARSE_H
#define MAMAN14_SPACEPARSE_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "errorsHeader.h"

int isEmptyLine(char* );
int locAfterSpace(char*,int);
int commaAfterSpace(char* lineInput, int i);
int findQuotation(char* lineInput, int i, int line);

#endif //MAMAN14_SPACEPARSE_H
