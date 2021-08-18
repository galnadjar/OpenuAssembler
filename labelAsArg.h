//
// Created by xubuntu on 8/5/21.
//

#ifndef MAMAN14_LABELASARG_H
#define MAMAN14_LABELASARG_H

/*---------------------------- includes ----------------------------*/

#include <stdlib.h>
#include <string.h>
#include "errorsHeader.h"


/*---------------------------- defines ----------------------------*/

typedef struct labelTable* labelTablePtr;
typedef enum type{
    J_BRANCHING = 1,
    I_BRANCHING
}type;


/*-------------------- functions declarations ---------------------*/
int addToLabelTable(labelTablePtr* head, char* labelName, long address,type branch,int line);

int getLabelBranch(labelTablePtr ptr);

long getLabelTableAddress(labelTablePtr ptr);

int getLabelTableLine(labelTablePtr ptr);

labelTablePtr getNextLabelNode(labelTablePtr ptr);

char* getLabelTableName(labelTablePtr ptr);

#endif //MAMAN14_LABELASARG_H