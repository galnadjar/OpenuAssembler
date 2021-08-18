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
        printf("%s%s file%s\n",UNDERLINE,p,CLOSEUNDERLINE);
        readFile(fp,p);
        printf("\n\n");
    }
}


/*this function reads the file and analyzes it for first iteration,if no error was found during read,
 * calls the second iteration function,otherwise closes file*/
void readFile(FILE* fp,char* fileName) {

    int i = 0,line = 1, state = VALID,wasError = 0,wasLabel = 0,category = EMPTY_CATEGORY_FLAG,exceedLine;
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
                        if(state != ERROR)
                            state = checkDirArgs(lineInput, wordSaved, i, line,&DC,&dataImgHead);}

                    else { /*case category == INSTRUCTION_FLAG*/
                        if(wasLabel)
                            state = addSymbol(&symbolTableHead, labelName, IC, CODE_AT, line);
                        if (state != ERROR)
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
            secondIteration(&symbolTableHead, &entryHead, &labelTableHead, &codeImgHead, &dataImgHead,&externHead, IC + DC, IC, fileName);}
    else
        ERROR_MAX_PROG(exceedLine);

    releaseDataTables(&symbolTableHead, &entryHead, &labelTableHead, &codeImgHead, &dataImgHead, &externHead);
    fclose(fp);
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

        printf("file given was valid, output files were created accordingly.\n");
    }
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