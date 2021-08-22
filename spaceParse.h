#ifndef MAMAN14_SPACEPARSE_H
#define MAMAN14_SPACEPARSE_H

/*---------------------------- includes ----------------------------*/
#include <string.h>
#include <ctype.h>
#include "errorsHeader.h"


/*-------------------- functions declarations ---------------------*/
int isEmptyLine(char* );
int locAfterSpace(char*,int);
int commaAfterSpace(char* lineInput, int i);
int findQuotation(char* lineInput, int i, int line);

#endif /*MAMAN14_SPACEPARSE_H*/
