
#include "labelAsArg.h"

typedef struct labelTable{

    char labelName[MAX_LABEL_LENGTH+1];
    long address;
    int line;
    type branch;

    labelTablePtr next;
}labelTable;

labelTablePtr getNextLabelNode(labelTablePtr ptr){
    return ptr->next;
}

/*check why my method causes leak*/
int addToLabelTable(labelTablePtr *head, char* labelName, long address,type branch,int line){
    int state = VALID;

    labelTablePtr p = (*head),temp;

    temp = (labelTablePtr) calloc(1,sizeof(labelTable));

    if(temp){
        strcpy(temp->labelName,labelName);
        temp->address = address;
        temp->branch = branch;
        temp->line = line;

        if(!(*head)){/*head is empty*/
            (*head) = temp;}

        else{
            for(;p->next;p = p->next);
            p->next = temp;}
    }
    else
        state = ERROR;

    return state;
}

int getLabelBranch(labelTablePtr ptr){
    return ptr->branch;
}

int getLabelTableLine(labelTablePtr ptr){
    return ptr->line;
}

long getLabelTableAddress(labelTablePtr ptr){
    return ptr->address;
}

char* getLabelTableName(labelTablePtr ptr){
    return ptr->labelName;
}

int getLabelTableSize(){
    return sizeof(labelTable);
}