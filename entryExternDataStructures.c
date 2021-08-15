#include "entryExternDataStructures.h"

typedef struct entryTable{

    char label[MAX_LABEL_LENGTH+1];
    long address;
    int line;
    entryTablePtr next;

}entryTable;

typedef struct externTable{

    char label[MAX_LABEL_LENGTH+1];
    long address;
    int line;
    externTablePtr next;

}externTable;

int addEntry(entryTablePtr* entryHead,char* label,int line){

    entryTablePtr ptrEnt;
    int state = VALID;

     ptrEnt = findEntryLabel(entryHead,label);

    if(ptrEnt) /*doesn't do anything if already exists*/
        state = EXIT;

    else
        state = handleEntNode(label,entryHead,line);

    if(state == ERROR){
        ERROR_MEMORY_MAXED_OUT(line);
        state = 0;}
    else
        state = 1;

    return state;
}

void setEntryAddress(entryTablePtr* ptr ,long address){
    (*ptr)->address = address;
}

/*returns 1 if valid allocation of new node, -1 otherwise*/
int handleEntNode(char* label,entryTablePtr* entryHead,int line){
    int state = VALID;
    entryTablePtr tempEnt,currEnt;

    tempEnt = (entryTablePtr) calloc(1,sizeof(entryTable));
    if(tempEnt){
        strcpy(tempEnt->label,label);
        tempEnt->line = line;

        if(getEntryLine(*entryHead)){
            for(currEnt = (*entryHead);currEnt->next;currEnt = currEnt->next);
            currEnt->next = tempEnt;}

        else
            (*entryHead) = tempEnt;
    }
    else
        state = ERROR;
    return state;
}

entryTablePtr findEntryLabel(entryTablePtr* entryTableHead, char* word){
    entryTablePtr node = NULL;
    entryTablePtr curr = *entryTableHead;

    for(;curr->next;curr = curr->next){
        if(!strcmp(curr->label, word))
            node = curr;
    }
    return node;

}
/*used to hold the ext file we going to print, returns 1 if no error,and -1 if memory allocation error found*/
int addToExtTable(externTablePtr *tableHead, char* word, long address,int line){
    externTablePtr node;
    externTablePtr curr = *tableHead;
    int state = VALID;

    node = (externTablePtr) calloc(1,sizeof(externTable));
    if(node){
        strcpy(node->label,word);
        node->address = address;
        node->line = line;

        if(!(curr->line))
            (*tableHead) = node;

        else{
            for(;curr->next;curr = curr->next);
            curr->next = node;
        }
    }

    else
        state = ERROR;

    return state;
}

char* getEntryLabel(entryTablePtr ptr){
    return ptr->label;
}

char* getExternLabel(externTablePtr ptr){
    return ptr->label;
}

long getExternSize(){
    return sizeof(externTable);
}
long getExtAddress(externTablePtr ptr){
    return ptr->address;
}

long getEntAddress(entryTablePtr ptr){
    return ptr->address;
}

int getEntryLine(entryTablePtr ptr){
    return ptr->line;
}

externTablePtr getNextExtNode(externTablePtr ptr){
    return ptr->next;
}
entryTablePtr getNextEntNode(entryTablePtr ptr){
    return ptr->next;
}

int getExternLine(externTablePtr ptr){
    return ptr->line;
}


