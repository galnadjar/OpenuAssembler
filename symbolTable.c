#include "symbolTable.h"

typedef struct symbol{

    char name[MAX_LABEL_LENGTH+1];
    long line;
    long address;
    attribute type;
    symbolPtr next;

}symbol;


/*handles the adding of a label to the symbol table,
 * if the symbol was previously declared extern and now it's the second declaration of extern, does nothing and returns 1.
 * otherwise,was previously declared as anything else / the creation of symbol wasn't possible returns -1,*/
int addSymbol(symbolPtr* symbolHead, char* label, long address, int category, int line){ /*first node is not assigned*/

    int state = VALID;
    symbolPtr temp,ptr;
    temp = findSymbol(symbolHead, label); /*check if a symbol is already exist in the table*/
    if(temp){
        if(temp->type == EXTERN_AT && category == EXTERN_AT)
            state = VALID;

        else{
            state = ERROR;
            ERROR_ATTRIBUTE_ASSIGN(line);}
    }
    else{
        state = createSymbol(&ptr, label, address, category, line);
        if(state != ERROR){
            if(!(*symbolHead)) /*is first node*/
                (*symbolHead) = ptr;

            else
                addNodeToSymbolTable(symbolHead, &ptr);
        }
    }
    return state;
}


/*sets a given symbol values*/
void setSymbolValues(symbolPtr* ptr,char* label,long address,int category,int line){
    strcpy((*ptr)->name,label);
    (*ptr)->address = address;
    (*ptr)->type = category;
    (*ptr)->line = line;
}


/*adds a given node to the end of the symbol table*/
void addNodeToSymbolTable(symbolPtr* head,symbolPtr* desired){
    symbolPtr p = *head;
    for(;p->next;p = p->next);
    p->next = (*desired);
}


/*searches in the symbol data structure after a label,
* return the node if found otherwise returns null*/
symbolPtr findSymbol(symbolPtr* symbolTableHead, char* word){

    symbolPtr match = NULL;
    symbolPtr ptr = *symbolTableHead;
    for(;ptr && getSymbolType(ptr);ptr = ptr->next){
        if(!strcmp(getSymbolLabel(ptr),word))
            match = ptr;}

    return match;
}


/*creates a new symbol if memory allocated properly update its values and return 1,
 * otherwise if not enough memory returns -1*/
int createSymbol(symbolPtr* symbolName,char* label,long address,int category,int line){

    int state = VALID;

    (*symbolName) = (symbolPtr) calloc(1,sizeof(symbol));
    if(!(*symbolName)){
        ERROR_MEMORY_MAXED_OUT(line);
        state = ERROR;}

    else
        setSymbolValues(symbolName,label,address,category,line);

    return state;
}


/*gets a given symbol name*/
char* getSymbolLabel(symbolPtr ptr){
    return ptr->name;
}


/*gets a given symbol address*/
long getSymbolAddress(symbolPtr ptr){
    return ptr->address;
}


/*gets a given symbol type*/
int getSymbolType(symbolPtr ptr){
    return ptr->type;
}


/*gets the following node to the given node*/
symbolPtr getSymbolNextNode(symbolPtr ptr){
    return ptr->next;
}


/*updates a given symbol node to its current address + a given num*/
void updateSymbolAddress(symbolPtr* ptr,long num){
    (*ptr)->address = getSymbolAddress(*ptr) + num;
}




