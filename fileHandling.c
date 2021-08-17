#include "fileHandling.h"

/*opens the file ,and check if empty
 * if the file was opened properly and is not empty, calls readFile function to read it*/

void openFile(char* p){

    int state = 1;
    FILE *fp = fopen(p,"r");
    int ch;

    if(!fp) {
        ERROR_FILE_DONT_EXIST(p);
        state = 0;
    }

    else {
        if((ch = fgetc(fp)) == EOF) { /*file is empty*/
            ERROR_FILE_IS_EMPTY(p);
            state = 0;
        }
        ungetc(ch,fp);
    }

    if(state){
        printf("----------------------------START OF FILE------------------------------\n");
        readFile(fp,p);
        printf("----------------------------END OF FILE------------------------------\n");
    }
}


/*after validating the input from the first iteration on the file,
 * this function responsible for the second iteration, including more validating.
 * if the given assembly file was validated in the second iteration as well, prints it in the required format*/
void secondIteration(symbolPtr* symbolTableHead,entryTablePtr* entryTableHead,labelTablePtr* labelTablehead,
                    codeImgPtr* codeImgHead,dataImgPtr* dataImgHead,externTablePtr* externHead, const long DCF,const long ICF,char* name){

    int state = VALID;

    addICF(dataImgHead,symbolTableHead,ICF); /*adds the ICF value to the current data addresses*/
    state = analyzeLabelTable(labelTablehead, symbolTableHead,externHead,codeImgHead,entryTableHead);
    if(state != ERROR){
        name[strlen(name)-EXT_LENGTH] = '\0';

        writeOB(name, *dataImgHead,*codeImgHead,DCF,ICF);
        if(*entryTableHead) /*check if head is not empty*/
            writeEntry(name,*entryTableHead);

        if(*externHead)/*check if head is not empty*/
            writeExtern(name,*externHead);
    }
}


/*this function creates a file for the entry data structure,
 * if the memory is available for the name of the file and the file itself to be created, prints it*/
void writeEntry(char* name,entryTablePtr entryPtr){

    FILE* fp;
    char *extension;
    extension = ".ent";
    fp = createWriteFile(name, extension);
    if(fp)
        printEnt(fp,entryPtr);
}


/*this function creates a file for the extern data structure,
 * if the memory is available for the name of the file and the file itself to be created, prints it*/
void writeExtern(char* name, externTablePtr externPtr){

    FILE* fp;
    char *extension;
    extension = ".ext";
    fp = createWriteFile(name, extension);
        if(fp)
            printExt(fp,externPtr);
}


/*this function prints the extern data structure to the given file*/
void printExt(FILE* fp,externTablePtr externPtr){

    externTablePtr curr = externPtr;
    while(curr){
        fprintf(fp,"%04ld\t%s\n",getExtAddress(curr), getExternLabel(curr));
        curr = getNextExtNode(curr);
    }
}


/*this function prints the entry data structure to the given file*/
void printEnt(FILE* fp,entryTablePtr entryPtr){

    entryTablePtr curr = entryPtr;
    while(curr){
        fprintf(fp,"%s\t%04ld\n", getEntryLabel(curr),getEntAddress(curr));
        curr = getNextEntNode(curr);
    }
}


/*if the external and entry were exist update them and return 1,otherwise -1
 * the function analyzes each label that was passed as an argument and calls analyzeTypeSymbol to:
 * validate and update the data structures based on their traits that were given from first iteration
 * returns 1 everything was updated and found successfully
 * returns -1 if a label wasn't declared or doesn't correspond to the rules*/
int analyzeLabelTable(labelTablePtr* labelTablehead, symbolPtr* symbolTableHead,externTablePtr* externTableHead,codeImgPtr* codeHead,entryTablePtr* entryHead){

    int state = VALID,ans;

    labelTablePtr curr = (*labelTablehead);
    for(;curr;curr = getNextLabelNode(curr)){/*iter through label as arg dataStructure*/
        ans = analyzeTypeSymbol(curr, symbolTableHead, externTableHead, codeHead,entryHead);
        if(ans == EXIT)
            if(findEntryLabel(entryHead, getLabelTableName(curr)))
                analyzeEntrySymbol(getLabelTableName(curr),entryHead, getLabelTableAddress(curr));
        if(ans == ERROR)
            state = ERROR;
    }

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


/*the function analyzes a given label from labelTable data structure
 * returns 1 if the label exists in the symbolTable and corresponds to the rules.
 * if exists but does not correspond to the rules returns -1.
 * otherwise, if doesn't exist returns 0*/
int analyzeTypeSymbol(labelTablePtr labelPtr, symbolPtr* symbolHead, externTablePtr* externHead, codeImgPtr* codeHead,entryTablePtr* entryHead){

    int state = VALID,found = 0;
    symbolPtr symPtr = (*symbolHead);

    for(;!found && symPtr;symPtr = getSymbolNextNode(symPtr)){ /*iterating through the symbol table*/

        if(!(strcmp(getSymbolLabel(symPtr),getLabelTableName(labelPtr)))){ /*matching name between symbol table and label table*/
            found = 1;
            if(getSymbolType(symPtr) == EXTERN_AT){
                if(getLabelBranch(labelPtr) == J_BRANCHING){ /*la/call/jmp*/
                    state = addToExtTable(externHead, getLabelTableName(labelPtr), getLabelTableAddress(labelPtr),
                                          getLabelTableLine(labelPtr));
                    if(state == ERROR)
                        ERROR_MEMORY_MAXED_OUT(getLabelTableLine(labelPtr));
                }

                else{  /*case its I BRANCH*/
                    state = ERROR;
                    ERROR_EXTERN_I_BRANCH(getLabelTableLine(labelPtr),getLabelTableName(labelPtr));
                }
            }

            else{ /*not external*/

                if(findEntryLabel(entryHead, getSymbolLabel(symPtr)))
                    analyzeEntrySymbol(getLabelTableName(labelPtr),entryHead, getSymbolAddress(symPtr));

                if(getLabelBranch(labelPtr) == J_BRANCHING)
                    state = updateJbranching(codeHead,getLabelTableName(labelPtr), getSymbolAddress(symPtr));


                else if(getLabelBranch(labelPtr) == I_BRANCHING)/*the argument immed field is updated*/
                    state = updateImmed(codeHead, getLabelTableName(labelPtr), getSymbolAddress(symPtr));
            }
        }
    }

    if(!found){
        state = EXIT;
    }

    return state;
}


/*adds ICF value to all data addresses*/
void addICF(dataImgPtr* dataImgHead,symbolPtr* symbolHead,long ICF){

    symbolPtr  currSymbol = (*symbolHead);
    dataImgPtr currData = (*dataImgHead);
    while(currData){
        setDataAddress(&currData, getDataAddress(currData) + ICF);
        currData = getNextDataNode(currData);
    }
    while(currSymbol){
        if(getSymbolType(currSymbol) == DATA_AT)
            updateSymbolAddress(&currSymbol,ICF);
        currSymbol = getSymbolNextNode(currSymbol);
    }
}


/*this function reads the file and analyzes it for first iteration,if no error was found during read,
 * calls the second iteration function,otherwise closes file*/
void readFile(FILE* fp,char* fileName) {

    int i,line = 1, state = VALID,wasError = 0,wasLabel = 0,category = EMPTY_CATEGORY_FLAG,exceedLine;
    long IC = IC_INITIAL_ADDRESS, DC = DC_INITIAL_ADDRESS;
    char *wordSaved = NULL,*labelName = NULL,*lineInput = (char *) calloc(MAX_ROW_LENGTH, sizeof(char));

    externTablePtr  externHead = NULL;
    symbolPtr symbolTableHead = NULL;
    entryTablePtr entryHead = NULL;
    codeImgPtr codeImgHead = NULL;
    dataImgPtr dataImgHead = NULL;
    labelTablePtr labelTableHead = NULL;

    while (1) {

        fgets(lineInput, MAX_ROW_LENGTH-1, fp);
        if (feof(fp))
            break;

        state = !isEmptyLine(lineInput);

        if (state) {/*meaning its not comment and neither empty line*/

            labelName =  (char*) calloc(MAX_LABEL_LENGTH+1,sizeof(char));
            wordSaved = (char*) calloc(MAX_LABEL_LENGTH+1,sizeof(char));
            i = parseCategory(&i, lineInput, &wordSaved, &category, line);/*parses the category of the first word*/

            if(i){
                if (category == LABEL_FLAG){
                    wasLabel = 1;
                    state = handleLabelCategory(&i,lineInput,&labelName,&wordSaved,&category,line);}

                if(i) {
                    if (category == EXTERN_FLAG || category == ENTRY_FLAG)
                        state = handleEntOrExtCategory(&i, lineInput, labelName, line, category, &entryHead,
                                                       &symbolTableHead);

                    else if (category == DIRECTIVE_FLAG) {
                        if(wasLabel)
                            state = addSymbol(&symbolTableHead, labelName, DC, DATA_AT, line);
                        if(state)
                            state = checkDirArgs(lineInput, wordSaved, i, line,&DC,&dataImgHead);}

                    else { /*case category == INSTRUCTION_FLAG*/
                        if(wasLabel)
                            state = addSymbol(&symbolTableHead, labelName, IC, CODE_AT, line);
                        if (state)
                            state = checkInsArgs(lineInput, wordSaved, i, line, &IC, &codeImgHead,&labelTableHead);
                        IC += 4;
                    }
                }
            }
        }
        if(DC + IC == MAX_PROG_SIZE)
            exceedLine = line;

        resetIterVars(&wasLabel, &wordSaved, &labelName, &i, &category);
        line++;
        if(state == ERROR)
            wasError = 1;
    }
    free(lineInput);

    if(DC + IC < MAX_PROG_SIZE){
        if(!wasError)/*no reason for second iteration if errors were found at the first iteration*/
            secondIteration(&symbolTableHead, &entryHead, &labelTableHead, &codeImgHead, &dataImgHead,&externHead, IC + DC, IC, fileName);
    }
    else
        ERROR_MAX_PROG(exceedLine);

    releaseDataTables(&symbolTableHead, &entryHead, &labelTableHead, &codeImgHead, &dataImgHead, &externHead);
    fclose(fp);
}


/*this function handles the case when the category given is a label*/
int handleLabelCategory(int* i,char* lineInput,char** labelName,char** wordSaved,int* category,int line){

    int state = VALID;

    if(isspace(lineInput[(*i)])) { /*check that after the ':' theres a spacing*/
        strcpy(*labelName,*wordSaved);
        (*i) = parseCategory(i, lineInput, wordSaved, category, line);}

    else{
        (*i) = 0;
        state = ERROR;
        ERROR_LABEL_WITHOUT_SPACE(line);}
    return state;
}


/*handle the case when the category given is entry/extern*/
int handleEntOrExtCategory(int* i, char* lineInput, char* labelName, int line, int category, entryTablePtr* entryHead, symbolPtr* symHead){

    int state = VALID;
    (*i) = analyzeLabel(lineInput,*i,line,&labelName);

    if ((*i) != ERROR) {
        (*i) = locAfterSpace(lineInput, *i);

        if ((*i) == strlen(lineInput)-1) {
            if (category == ENTRY_FLAG)
                state = addEntry(entryHead, labelName, line);

            else
                state = addSymbol(symHead,labelName,0,EXTERN_AT,line);
        }

        else {
            ERROR_EXTRANEOUS_END_OF_CMD(line);
            state = ERROR;}
    }
    return state;
}


/*resets the iteration variables so they will be empty for the next iteration*/
void resetIterVars(int* wasLabel, char** wordSaved, char** labelName, int* i, int* category){
    (*i) = 0;
    (*wasLabel) = 0;
    (*category) = EMPTY_CATEGORY_FLAG;

    if(*labelName)
        free(*labelName);
    if(*wordSaved)
        free(*wordSaved);
}


/*creates and return a valid file pointer if memory allow file allocation,otherwise returns null*/
FILE* createWriteFile(char* name,char* extension){

    FILE* fp = NULL;
    char fileName[strlen(name) + strlen(extension)+1];
    strcpy(fileName, name);
    strcat(fileName, extension);

    fp = fopen(fileName,"w");
    if(!fp)
        ERROR_MEMORY_ALLOCATION(fileName);

    return fp;
}


/*writes the ob file*/
void writeOB(char* name,dataImgPtr dataPtr,codeImgPtr codePtr,const long DCF,const long ICF){

    FILE* fp = NULL;
    char* ob = ".ob";

    fp = createWriteFile(name,ob);
    if(fp){
        printCounters(fp,ICF,DCF);
        printCodeImg(fp,codePtr);
        printDataImg(fp,dataPtr,ICF,DCF);
    }
}


/*prints the data img to the file*/
void printDataImg(FILE* fp, dataImgPtr dataPtr, long ICF,const long DCF){

    dataImgPtr curr = dataPtr;
    long DC = ICF;
    int bytesCounter = 0;
    fprintf(fp,"%04ld\t",DC);
    while(curr){
        printDataDisplay(fp,curr,&bytesCounter,&DC,DCF);
        curr = getNextDataNode(curr);}
}


/*prints the code img to the file*/
void printCodeImg(FILE* fp,codeImgPtr codePtr){

    while(codePtr){
        fprintf(fp,"%04ld\t%02X\t%02X\t%02X\t%02X\n",getCodeAddress(codePtr),getCodeDisplay(codePtr,A_BYTE),
                getCodeDisplay(codePtr,B_BYTE),getCodeDisplay(codePtr,C_BYTE),getCodeDisplay(codePtr,D_BYTE));
        codePtr = getNextCodeNode(codePtr);
    }
}


/*prints the IC AND DC COUNTERS*/
void printCounters(FILE* fp ,long IC, long DC){
        fprintf(fp,"\t\t%ld\t%ld\n",IC-IC_INITIAL_ADDRESS,DC-IC);
}

/*releases all the data structures in the program*/
void releaseDataTables(symbolPtr* symbolTableHead,entryTablePtr* entryTableHead,labelTablePtr* labelTableHead,
                  codeImgPtr* codeImgHead,dataImgPtr* dataImgHead,externTablePtr* externTableHead){

    freeSymSt(symbolTableHead);
    freeEntSt(entryTableHead);
    freeExtSt(externTableHead);
    freeLabelSt(labelTableHead);
    freeCodeSt(codeImgHead);
    freeDataSt(dataImgHead);

}


/*those function releases the specific table given from the memory*/
void freeSymSt(symbolPtr* ptr){
    symbolPtr currSym = (*ptr),tailSym;
    while(currSym){
        tailSym = currSym;
        currSym = getSymbolNextNode(currSym);
        free(tailSym);}
}

void freeEntSt(entryTablePtr* ptr){
    entryTablePtr currEnt = (*ptr),tailEnt;
    while(currEnt){
        tailEnt = currEnt;
        currEnt = getNextEntNode(currEnt);
        free(tailEnt);}
}

void freeExtSt(externTablePtr* ptr){
    externTablePtr currExt = (*ptr),tailExt;
    while(currExt){
        tailExt = currExt;
        currExt = getNextExtNode(currExt);
        free(tailExt);
    }
}

void freeLabelSt(labelTablePtr* ptr){

    labelTablePtr currLab = (*ptr),tailLab;
    while(currLab){
        tailLab = currLab;
        currLab = getNextLabelNode(currLab);
        free(tailLab);
    }
}

void freeCodeSt(codeImgPtr* ptr){
    codeImgPtr currCode = (*ptr),tailCode;
    while(currCode){
        tailCode = currCode;
        currCode = getNextCodeNode(currCode);
        free(tailCode);
    }
}

void freeDataSt(dataImgPtr* ptr){
    dataImgPtr currData = (*ptr),tailData;
    while(currData){
        tailData = currData;
        currData = getNextDataNode(currData);
        free(tailData);
    }
}