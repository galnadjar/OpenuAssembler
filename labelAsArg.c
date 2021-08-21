
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

/*adds a new label that was given as an argument to the label table if memory is available and returns 1,
 * otherwise returns -1*/
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


/*returns the type of the labelTable node*/
int getLabelBranch(labelTablePtr ptr){
    return ptr->branch;
}


/*returns the line of the labelTable node*/
int getLabelTableLine(labelTablePtr ptr){
    return ptr->line;
}


/*returns the address of the labelTable node*/
long getLabelTableAddress(labelTablePtr ptr){
    return ptr->address;
}


/*returns the name of the labelTable node*/
char* getLabelTableName(labelTablePtr ptr){
    return ptr->labelName;
}