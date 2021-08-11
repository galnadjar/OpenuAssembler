#include "fileHandling.h"

/*opens the file and if not empty and valid, reads it as well*/
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
        readFile(fp,p);
    }
}


void secondIteration(symbolPtr* symbolTableHead,entryTablePtr* entryTableHead,labelTablePtr* labelTablehead,
                    codeImgPtr* codeImgHead,dataImgPtr* dataImgHead,const long DCF,const long ICF,char* name){

    int state = VALID;
    externTablePtr externTableHead = (externTablePtr) calloc(1,getExternSize());

    addICF(dataImgHead,ICF); /*adds the ICF value to the current data addresses*/
    state = analyzeLabelTable(labelTablehead, symbolTableHead,&externTableHead,codeImgHead,entryTableHead);
    if(state != ERROR){

        writeOB(name, *dataImgHead,*codeImgHead,DCF,ICF);
        if(getEntryLabel(*entryTableHead))
            writeEntry(name,*entryTableHead);

         if(getExternLabel(externTableHead))
            writeExtern(name,externTableHead);
    }
} /*end of readFile*/


void writeEntry(char* name,entryTablePtr entryPtr){

    FILE* fp;
    char *externLabel = NULL,*extension;
    externLabel = getEntryLabel(entryPtr);
    if(externLabel){
        extension = ".ent";
        fp = createWriteFile(name, extension);
        if(fp)
            printEnt(fp,entryPtr);
    }
}


void writeExtern(char* name, externTablePtr externPtr){

    FILE* fp;
    char *externLabel = NULL,*extension;
    externLabel = getExternLabel(externPtr);
    if(externLabel){
        extension = ".extension";
        fp = createWriteFile(name, extension);
        if(fp)
            printExt(fp,externPtr);
    }
}

void printExt(FILE* fp,externTablePtr externPtr){

    externTablePtr curr = externPtr;
    while(curr){
        fprintf(fp,"%04ld\t%s\n",getExtAddress(curr), getExternLabel(curr));
        curr = getNextExtNode(curr);
    }
}

void printEnt(FILE* fp,entryTablePtr entryPtr){

    entryTablePtr curr = entryPtr;
    while(curr){
        fprintf(fp,"%04ld\t%s\n",getEntAddress(curr), getEntryLabel(curr));
        curr = getNextEntNode(curr);
    }
}


/*if the external and entry were exist update them and return 1,otherwise -1*/
int analyzeLabelTable(labelTablePtr* labelTablehead, symbolPtr* symbolTableHead,externTablePtr* externTableHead,codeImgPtr* codeHead,entryTablePtr* entryHead){

    int state = VALID,ans;

    labelTablePtr curr = (*labelTablehead);
    for(;curr;curr = getNextLabelNode(curr)){
        ans = analyzeTypeSymbol(curr, symbolTableHead, externTableHead, codeHead);
        if(ans != ERROR)
            ans = analyzeEntrySymbol(curr,entryHead);

        if(ans == ERROR)
            state = ERROR;
    }

    return state;
}

/*returns 1 if entry was used,otherwise -1*/
int analyzeEntrySymbol(labelTablePtr labelPtr,entryTablePtr* entryHead){

    int state = ERROR; /*meaning entry wasn't found yet*/

    entryTablePtr entryPtr = (*entryHead);
    for(;entryPtr;entryPtr = getNextEntNode(entryPtr)){ /*iterating through the symbol table*/
        if(!strcmp(getEntryLabel(entryPtr), getLabelTableName(labelPtr))){
            state = VALID;
            setEntryAddress(&entryPtr,getLabelTableAddress(labelPtr));
        }
    }
    return state;
}

/*todo add a line number to every struct in the program so the messages would correspond to the line */
/*returns 1 if all the label exists and corresponds to the matching rules,otherwise -1*/
int analyzeTypeSymbol(labelTablePtr labelPtr, symbolPtr* symbolHead, externTablePtr* externHead, codeImgPtr* codeHead){

    int state = VALID;
    symbolPtr symPtr = (*symbolHead);

    for(;symPtr;symPtr = getSymbolNextNode(symPtr)){ /*iterating through the symbol table*/

        if(!(strcmp(getSymbolLabel(symPtr),getLabelTableName(labelPtr)))){ /*case they have same name*/
            if(getSymbolType(symPtr) == EXTERN_AT){ /*todo fix this symbol line*/
                if(getLabelBranch(labelPtr) == J_BRANCHING){
                    state = addToExtTable(externHead, getLabelTableName(labelPtr), getLabelTableAddress(labelPtr));
                    if(state == ERROR)
                        ERROR_MEMORY_MAXED_OUT(getLabelTableLine(labelPtr));
                }

                else{  /*case its I BRANCH*/
                    state = ERROR;
                    ERROR_EXTERN_I_BRANCH(getLabelTableLine(labelPtr),getLabelTableName(labelPtr));
                }
            }

            else{ /*not external*/
                if(getLabelBranch(labelPtr) == I_BRANCHING)/*the argument is ibranching type so we update its field*/
                    updateImmed(codeHead, getLabelTableName(labelPtr), getSymbolAddress(symPtr));
            }
        }

        else{
            state = ERROR;
            ERROR_LABEL_EXISTENCE(getLabelTableLine(labelPtr),getLabelTableName(labelPtr));}
        }

    return state;
}


/*adds ICF to all data addresses*/
void addICF(dataImgPtr* dataImgHead,long ICF){

    dataImgPtr curr = (*dataImgHead);
    while(curr){
        setDataAddress(&curr, getDataAddress(curr) + ICF);
        curr = getNextDataNode(curr);
    }
}

void allocVars(char** labelName,char** wordSaved){

    (*labelName) =  (char*) calloc(MAX_LABEL_LENGTH+1,sizeof(char));
    (*wordSaved) = (char*) calloc(MAX_LABEL_LENGTH+1,sizeof(char));
}

void readFile(FILE* fp,char* fileName) {

    int i,line = 1, state = VALID,wasError = 0,wasLabel = 0;
    int category = EMPTY_CATEGORY_FLAG;
    long IC = IC_INITIAL_ADDRESS, DC = DC_INITIAL_ADDRESS;
    char *lineInput = (char *) calloc(MAX_ROW_LENGTH, sizeof(char));
    char *wordSaved = NULL,*labelName = NULL;

    symbolPtr symbolTableHead = (symbolPtr) calloc(1,sizeof(getSymbolSize()));
    entryTablePtr entryTableHead = (entryTablePtr) calloc(1,sizeof(getExternSize()));
    codeImgPtr codeImgHead = NULL;
    dataImgPtr dataImgHead = NULL;
    labelTablePtr labelTableHead = (labelTablePtr) calloc(1,sizeof(getLabelTableSize()));

    while (1) {

        fgets(lineInput, MAX_ROW_LENGTH-1, fp);
        if (feof(fp))
            break;

        state = !isEmptyLine(lineInput);

        if (state) {/*meaning its not comment and neither empty line*/

            allocVars(&labelName,&wordSaved);
            i = parseCategory(&i, lineInput, &wordSaved, &category, line);/*parses the category of the first word*/

            if(i){
                /*case label*/
                if (category == LABEL_FLAG){
                    wasLabel = 1;
                    if(isspace(lineInput[i])) { /*check that after the ':' theres a spacing*/ /*todo check if thats needed*/
                        strcpy(labelName,wordSaved);
                        i = parseCategory(&i, lineInput, &wordSaved, &category, line);}

                    else{
                        i = 0;
                        state = ERROR;
                        ERROR_LABEL_WITHOUT_SPACE(line);}
                }

                if (i) {

                    /* case extern / entry*/
                    if (category == EXTERN_FLAG || category == ENTRY_FLAG) {
                        i = analyzeLabel(lineInput,i,line,(char**)&labelName);

                        if (i != ERROR) {
                            i = locAfterSpace(lineInput, i);

                            if (i == strlen(lineInput)-1) {
                                if (category == ENTRY_FLAG)
                                    state = addEntry(&entryTableHead, labelName,UNKNOWN_ADDRESS, line);

                                else
                                    state = addSymbol(&symbolTableHead,labelName,0,EXTERN_AT,line);
                            }

                            else {
                                ERROR_EXTRANEOUS_END_OF_CMD(line);
                                state = ERROR;}
                        }
                    }

                    else if (category == DIRECTIVE_FLAG) {
                        state = checkDirArgs(lineInput, wordSaved, i, line,&DC,&dataImgHead);
                        if(state && wasLabel)
                            addSymbol(&symbolTableHead, labelName, DC, DATA_AT, line);}


                    else { /*category == INSTRUCTION_FLAG*/
                        state = checkInsArgs(lineInput, wordSaved, i, line, &IC, &codeImgHead,&labelTableHead);
                        if (state && wasLabel) {
                            state = addSymbol(&symbolTableHead, labelName, IC, CODE_AT, line);
                            if(state != ERROR)
                                IC += 4;}
                    }
                }
            }
        }

        resetFirstIterVars(&wasLabel, &wordSaved, &labelName,&i,&category);
        line++;
        if(state == ERROR)
            wasError = 1;
    }
    if(!wasError) /*no reason for second iteration if errors were found at the first 1*/
        secondIteration(&symbolTableHead,&entryTableHead,&labelTableHead,&codeImgHead,&dataImgHead,IC,IC+DC,fileName);

} /*end of readfile*/

void resetFirstIterVars(int* wasLabel, char** wordSaved, char** labelName,int* i,int* catergory){
    (*i) = 0;
    (*wasLabel) = 0;
    (*catergory) = EMPTY_CATEGORY_FLAG;

    if(*labelName)
        free(*labelName);
    if(*wordSaved)
        free(*wordSaved);
}



FILE* createWriteFile(char* name,char* extension){
    FILE* fp = NULL;
    char* fileName = NULL;
    fileName = (char*) calloc(strlen(name) + strlen(extension), sizeof(char));

    if(fileName){
        strcpy(fileName, name);
        strcat(fileName, extension);
        fp = fopen(fileName,"w");
    }

    else
        ERROR_MEMORY_ALLOCATION(name);
    return fp;
}

void writeOB(char* name,dataImgPtr dataPtr,codeImgPtr codePtr,const long DCF,const long ICF){

    FILE* fp = NULL;
    char* ob = ".ob";

    fp = createWriteFile(name,ob);
    if(fp){
        printCounters(fp,ICF,DCF);
        printCodeImg(fp,codePtr);
        printDataImg(fp,dataPtr);
    }
}


void printDataImg(FILE* fp, dataImgPtr dataPtr){

    dataImgPtr curr = dataPtr;
    long DC = getDataAddress(dataPtr);
    int bytesCounter = 0;
    while(curr){
        newLineOrTab(fp,&bytesCounter,&DC);

        if(!bytesCounter) /*meaning a new line that needs new address*/
            newLineOrTab(fp,&bytesCounter,&DC);

        printDataDisplay(fp,curr,&bytesCounter,&DC);
        curr = getNextDataNode(curr);}
}

void printCodeImg(FILE* fp,codeImgPtr codePtr){

    while(codePtr){
        fprintf(fp,"%04ld\t%02x\t%02x\t%02x\t%02x\n",getCodeAddress(codePtr),getCodeDisplay(codePtr,A_BYTE),
                getCodeDisplay(codePtr,B_BYTE),getCodeDisplay(codePtr,C_BYTE),getCodeDisplay(codePtr,D_BYTE));
        codePtr = getNextCodeNode(codePtr);
    }
}



void printCounters(FILE* fp ,long IC, long DC){
        fprintf(fp,"\t\t%ld\t%ld\n",IC-IC_INITIAL_ADDRESS,DC);
}