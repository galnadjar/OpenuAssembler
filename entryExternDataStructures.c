#include "entryExternDataStructures.h"

/*data structures for entry and extern symbols*/

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



/* adds an entry symbol,
 * if already exist / doesn't exist yet and handleEntNode returns 1 , the func returns 1
 * otherwise, if memory is full returns -1*/
int addEntry(entryTablePtr* entryHead,char* label,int line){

    entryTablePtr ptrEnt;
    int state = VALID;

    ptrEnt = findEntryLabel(entryHead,label);

    if(!ptrEnt) /*if doesn't exist*/
        state = handleEntNode(label,entryHead,line);

    if(state == ERROR)
        ERROR_MEMORY_MAXED_OUT(line);

    return state;
}


/*sets a new entry address to a given node*/
void setEntryAddress(entryTablePtr* ptr ,long address){
    (*ptr)->address = address;
}


/*handles the actual adding of the entry node to the table returns 1 if the allocation of new node is possible,
 * otherwise, returns -1 */
int handleEntNode(char* label,entryTablePtr* entryHead,int line){
    int state = VALID;
    entryTablePtr tempEnt,currEnt;

    tempEnt = (entryTablePtr) calloc(1,sizeof(entryTable));
    if(tempEnt){
        strcpy(tempEnt->label,label);
        tempEnt->line = line;

        if((*entryHead)){ /*check if the head is not empty*/
            for(currEnt = (*entryHead);currEnt->next;currEnt = currEnt->next);
            currEnt->next = tempEnt;}

        else
            (*entryHead) = tempEnt;
    }
    else
        state = ERROR;

    return state;
}


/*search in the entry table for an entry label with a matching name to the given label,if found return its address,otherwise null*/
entryTablePtr findEntryLabel(entryTablePtr* entryTableHead, char* word){
    entryTablePtr node = NULL;
    entryTablePtr curr = *entryTableHead;

    for(;curr;curr = curr->next){
        if(!strcmp(curr->label, word))
            node = curr;
    }
    return node;

}


/*if the allocation for the new node is valid,adds a new ext node to the extern data table and returns 1,otherwise,-1*/
int addToExtTable(externTablePtr *tableHead, char* word, long address,int line){

    externTablePtr node;
    externTablePtr curr = *tableHead;
    int state = VALID;

    node = (externTablePtr) calloc(1,sizeof(externTable));
    if(node){
        strcpy(node->label,word);
        node->address = address;
        node->line = line;

        if(!(curr)) /*means that the table is empty*/
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


/*updates an entry symbol address if found and then returns 1 ,otherwise if not found returns -1*/
int analyzeEntrySymbol(char* label,entryTablePtr* entryHead,long address){

    int state = ERROR; /*meaning entry wasn't found yet*/

    entryTablePtr entryPtr = (*entryHead);
    for(;entryPtr;entryPtr = getNextEntNode(entryPtr)){ /*iterating through the entry table*/
        if(!strcmp(getEntryLabel(entryPtr), label)){
            state = VALID;
            setEntryAddress(&entryPtr,address);
        }
    }
    return state;
}


/*returns the label of the current entry node*/
char* getEntryLabel(entryTablePtr ptr){
    return ptr->label;
}


/*returns the label of the current extern node*/
char* getExternLabel(externTablePtr ptr){
    return ptr->label;
}


/*returns the address of the current extern node*/
long getExtAddress(externTablePtr ptr){
    return ptr->address;
}


/*returns the address of the current entry node*/
long getEntAddress(entryTablePtr ptr){
    return ptr->address;
}


/*returns the next node to the current extern node*/
externTablePtr getNextExtNode(externTablePtr ptr){
    return ptr->next;
}


/*returns the next node to the current entry node*/
entryTablePtr getNextEntNode(entryTablePtr ptr){
    return ptr->next;
}