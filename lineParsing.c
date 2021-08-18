#include "lineParsing.h"

/*parseFirstWord returns 0 if theres been an error ,and 1 if the word was recieved successfuly*/
int parseCategory(int* i, char* lineInput, char** wordSaved, int* category, int line) {

    int ch,state = VALID;
    char* word= (char*) calloc(strlen(lineInput), sizeof(char));
    int wordloc = 0,firstLetter = 0,dot = 0;

    if(word){

        for (; (*i) < strlen(lineInput) -1 && state == VALID;(*i)++) {
            ch = (int)lineInput[(*i)];

            /*check if the current char is ':' and that we look for a label*/
            if (ch == ':')
                state = handleColon(word,category,firstLetter,line,ch);/*no word was previously saved*/

            else if (isspace(ch)) {

                if (!firstLetter && !dot) /*means its space before anything so we jump to the first letter*/
                    (*i) = locAfterSpace(lineInput, *i) - 1;

                else if (dot > 0)  /*a dot was passed in already,so we check if it's a start of directive name*/
                    state = handleDirCase(category, word, line);

                else  /*a word without a dot, should be a label or instruction*/
                state = handleLabelOrInstruction(category,word,line);

            } /*end of isspace*/

            else {
                state = analyzeChar(&dot,ch,&firstLetter,line);

                if(state != ERROR)
                    word[wordloc++] = (char) ch;
            }/*end of else*/
        } /*end of for loop*/


        strcpy(*wordSaved,word);
        free(word);
    }
    if(state == ERROR)
        (*i) = EXIT;

    return *i;
} /*end of parseFirstWord func*/



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
    else
        state = ERROR;
    return state;
}


int validDirectiveName(char* word){

    char dw[] = ".dw" ,db[] = ".db" ,dh[] = ".dh", asciz[] = ".asciz", ext[] = ".extern",ent[] = ".entry";

    return (!strcmp(word, dw) || !strcmp(word, db) || !strcmp(word, dh) ||
           !strcmp(word, asciz) || !strcmp(word, ext) || !strcmp(word, ent));
}



int validDotStart(int dot,int firstLetter,int line){

    int ans;
    if (!dot && !firstLetter)
        ans = VALID;

    else { /*a misplacement of a dot - error*/

        if (!dot) /*meaning a letter was already passed*/
            ERROR_IMPROPER_DOT(line);

        else if (!firstLetter) /* meaning a dot was already passed yet no letter,meaning they are consecutive*/
            ERROR_CONSECUTIVE_DOT(line);

        ans = 0;
    }
    return ans;
}



void invalidCategoryArgs(int line, int category){

    if((category) == DIRECTIVE_FLAG)
        ERROR_INVALID_DIRECTIVE_ARG(line);

    else if((category) == INSTRUCTION_FLAG)
        ERROR_INVALID_INSTRUCTION_ARG(line);

    else if(category == ENTRY_FLAG || category == EXTERN_FLAG)
        ERROR_LABEL_NAME(line);
}



void labelNameError(int line,char* word){
    if (strlen(word) > MAX_LABEL_LENGTH)/*the label length is too long*/
        ERROR_LABEL_LENGTH(line);

    else /*meaning its not a valid label name*/
        ERROR_LABEL_NAME(line);

}



void directiveCategorySelector(int* category,char* word){

    if(!strcmp(word,".entry"))
        (*category) = ENTRY_FLAG;

    else if(!strcmp(word,".extern"))
        (*category) = EXTERN_FLAG;

    else{
        (*category) = DIRECTIVE_FLAG;
    }
}



int handleColon(char* word,int* category,int firstLetter,int line ,int ch){
    int ans,state = VALID;

    if ((*category) == EMPTY_CATEGORY_FLAG && firstLetter) {
        ans = validLabelORInstruction(word, LABEL_SEARCH);/*check if the given label is valid*/

        if (ans && strlen(word) <= MAX_LABEL_LENGTH) {
            (*category) = LABEL_FLAG;
            state = EXIT;

        } else {
            labelNameError(line,word);
            state = ERROR;
        }
    }
    /*its an invalid positioning of the char ':' */
    else if(!firstLetter){
        ERROR_INVALID_CHAR(line,ch);
        state = ERROR;}

    return state;
}



int analyzeChar(int* dot,int ch,int* firstLetter,int line){

    int state = VALID;

    if (ch == '.') {

        if (validDotStart((*dot), (*firstLetter), line))
            (*dot) = 1;
        else
            state = ERROR;
    }

    else if (isalpha(ch))
        (*firstLetter) = 1;

    else if (isdigit(ch)) {

        if (!firstLetter) { /*a digit is given before a letter which is invalid*/
            ERROR_INVALID_PLACEMENT_OF_DIGIT(line);
            state = ERROR;}
    }

    else { /*an invalid character was given*/
        ERROR_INVALID_CHAR(line,ch);
        state = ERROR;
    }
    return state;
}


/*case we look for directive*/
int handleDirCase(int* category, char* word, int line){

    int ans,state = VALID;

    if ((*category) == LABEL_FLAG || (*category) == EMPTY_CATEGORY_FLAG) {
        ans = validDirectiveName(word);
        if (ans) {
            directiveCategorySelector(category,word);
            state = EXIT;
        }
        else {
            ERROR_IMPROPER_DIRECTIVE_NAME(line);
            state = ERROR;
        }
    }
    else{/*was directive/instruction already*/
        invalidCategoryArgs(line,*category);
        state = ERROR;
    }
    return state;
}



int handleLabelOrInstruction(int* category,char* word,int line){
    int ans,state = VALID;

    /*if the former was entry or extern*/
    if ((*category) == ENTRY_FLAG || (*category) == EXTERN_FLAG) {
        ans = validLabelORInstruction(word, LABEL_SEARCH);/*check if valid label*/

        if (ans)
            state = EXIT;

        else {
            ERROR_LABEL_NAME(line);
            state = ERROR;
        }

    } else if ((*category) == LABEL_FLAG || (*category) == EMPTY_CATEGORY_FLAG) {
        ans = validLabelORInstruction(word, INSTRUCTION_SEARCH); /*because dot counter is 0*/
        if (!ans) {
            (*category) = INSTRUCTION_FLAG;
            state = EXIT;}

        else {
            state = ERROR;
            ERROR_IMPROPER_INSTRUCTION_NAME(line);
        }
    } else { /*handles cases that it was directive or instruction*/
        state = ERROR;
        invalidCategoryArgs(line, *category);
    }

    return state;
}


/*if the external and entry were exists update them and return 1,otherwise -1
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