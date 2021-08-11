
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


int addToLabelTable(labelTablePtr* head, char* labelName, long address,type branch){
    int state = VALID;

    labelTablePtr p = (*head);
    labelTablePtr temp;

    temp = (labelTablePtr) calloc(1,sizeof(labelTable));

    if(temp){
        strcpy(temp->labelName,labelName);
        temp->address = address;
        temp->branch = branch;

        if(!(getLabelBranch(p))){/*head is empty*/
            strcpy(p->labelName,temp->labelName);
            p->address = temp->address;}

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