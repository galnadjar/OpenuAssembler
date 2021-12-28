#include "lineParsing.h"

/*parseCategory parses and analyzes the category of a given first/second arg in the line of input
 * returns 0 if there's been an error throughout the analysis
 * returns the end index if the analyzed arg corresponds to the given rules,and update "category", "wordSaved","i" vars*/
int parseCategory(int i, char* lineInput, char** wordSaved, int* category, int line) {

    int ch,state = VALID;
    char* word= (char*) calloc(strlen(lineInput), sizeof(char));
    int wordloc = 0,firstLetter = 0,dot = 0;

    if(word){

        for (; i < strlen(lineInput) -1 && state == VALID;i++) {
            ch = (int)lineInput[i];

            if (isspace(ch)) {

                if (!firstLetter && !dot) /*means its space before anything so we jump to the first letter*/
                    i = locAfterSpace(lineInput, i) - 1;

                else if (dot > 0)  /*a dot was passed in already,so we check if it's a start of directive name*/
                    state = handleDirCase(category, word, line);

                else  /*a word without a dot, should be a label or instruction*/
                    state = handleLabelOrInstruction(category,word,line);

            } /*end of isspace*/

            /*check if the current char is ':' and that we look for a label*/
            else if (ch == ':'){
                state = handleColon(word,category,firstLetter,line,ch,dot,&i);}

            else {
                state = analyzeChar(&dot,ch,&firstLetter,line);

                if(state == ERROR){ /*problematic letter was found*/
                    if(dot > 0)
                        ERROR_IMPROPER_DIRECTIVE_NAME(line);


                    else if(firstLetter)
                        ERROR_IMPROPER_INSTRUCTION_NAME(line);


                    else
                        ERROR_INVALID_CHAR(line,ch);
                }

                else
                    word[wordloc++] = (char) ch;
            }/*end of else*/
        } /*end of for loop*/

        if(i == strlen(lineInput)-1 && state != ERROR){

            if (dot > 0)  /*a dot was passed in already,so we check if it's a start of directive name*/
                state = handleDirCase(category, word, line);


            else if((lineInput[i] == '\n' || lineInput[i] == 0) && !firstLetter){
                state = ERROR;
                ERROR_EMPTY_LABEL(line);
            }

            else  /*a word without a dot, should be a label or instruction*/
                state = handleLabelOrInstruction(category,word,line);
        }
        if(state != ERROR)
            strcpy(*wordSaved,word);
        free(word);
    }
    if(state == ERROR)
        i = 0;

    return i;
} /*end of parseFirstWord func*/


/*this function handles the case when the category given is a label*/
int handleLabelCategory(int* i,char* lineInput,char** labelName,char** wordSaved,int* category,int line){

    int state = VALID;

    if(isspace(lineInput[(*i)])) { /*check that after the ':' theres a spacing*/
        strcpy(*labelName,*wordSaved);
        (*i) = parseCategory(*i, lineInput, wordSaved, category, line);
    }

    else{
        (*i) = 0;
        state = ERROR;
        ERROR_LABEL_WITHOUT_SPACE(line);}

    return state;
}


/*handle the case when the category given is entry/extern*/
int handleEntOrExtCategory(int i, char* lineInput, char* label, int line, int category, entryTablePtr* entryHead, symbolPtr* symHead){

    symbolPtr symNode = NULL;
    entryTablePtr entNode = NULL;
    int state = VALID;
    i = analyzeLabel(lineInput,i,line,&label);

    if (i != ERROR) {
        i = locAfterSpace(lineInput, i);

        if (i == strlen(lineInput)-1) {

            if (category == ENTRY_FLAG){
                symNode = findSymbol(symHead,label);
                if(symNode && getSymbolType(symNode) == EXTERN_AT){
                    state = ERROR;
                    ERROR_EXTERN_ENTRY_MIX(line,label);}
                else
                    state = addEntry(entryHead, label, line);}


            else{ /*category == EXTERN_FLAG*/
                entNode = findEntryLabel(entryHead, label);
                if(entNode){
                    state = ERROR;
                    ERROR_EXTERN_ENTRY_MIX(line,label);
                }

                else
                    state = addSymbol(symHead, label, 0, EXTERN_AT, line);}
        }
        else {
            ERROR_EXTRANEOUS_END_OF_CMD(line);
            state = ERROR;}
    }
    else
        state = ERROR;

    return state;
}

/*a function that matches a given directive name to the actual directive names.
 * returns 1 if the label corresponds to one of the options,otherwise returns -1*/
int validDirName(char* word){

    char dw[] = ".dw" ,db[] = ".db" ,dh[] = ".dh", asciz[] = ".asciz", ext[] = ".extern",ent[] = ".entry";

    return (!strcmp(word, dw) || !strcmp(word, db) || !strcmp(word, dh) ||
           !strcmp(word, asciz) || !strcmp(word, ext) || !strcmp(word, ent));
}


/*a function that checks if the dot received is valid,
 * if it is, returns 1,
 * otherwise, return 0*/
int dotValidation(int dot, int firstLetter, int line){

    int ans;
    if (!dot && !firstLetter) /*a dot before letter,which is also not consecutive*/
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


/*a function that matches an error to the given state*/
void invalidCategoryArgs(int line, int category){

    if((category) == DIRECTIVE_FLAG)
        ERROR_INVALID_DIRECTIVE_ARG(line);

    else if((category) == INSTRUCTION_FLAG)
        ERROR_INVALID_INSTRUCTION_ARG(line);

    else if(category == ENTRY_FLAG || category == EXTERN_FLAG)
        ERROR_LABEL_NAME(line);
}


/*a function that handles 2 cases of label errors based on the state*/
void labelNameError(int line,char* word){
    if (strlen(word) > MAX_LABEL_LENGTH)/*the label length is too long*/
        ERROR_LABEL_LENGTH(line);

    else /*meaning its not a valid label name*/
        ERROR_LABEL_NAME(line);
}


/*sets the directive category based on the match with those given strings*/
void directiveCategorySelector(int* category,char* word){

    if(!strcmp(word,".entry"))
        (*category) = ENTRY_FLAG;

    else if(!strcmp(word,".extern"))
        (*category) = EXTERN_FLAG;

    else{
        (*category) = DIRECTIVE_FLAG;
    }
}


/*used by parseCategory, this function analyzes a given label(since ended with a colon).
 * returns 0 if a valid label and matches in its placement
 * returns -1 if either the label is invalid or the colon position is */
int handleColon(char* word,int* category,int firstLetter,int line ,int ch,int dot,int* i){
    int ans,state = VALID;

    if (dot > 0){  /*a dot was passed in already,so we check if it's a start of directive name*/
        state = handleDirCase(category, word, line); /*retuns 0 or error*/
        if(!state)
            (*i)--;}

    else if ((*category) == EMPTY_CATEGORY_FLAG && firstLetter) { /*it's the first word, and a letter was given*/
        ans = validLabelORInstruction(word, LABEL_SEARCH);/*check if the given label is valid*/

        if (ans && strlen(word) <= MAX_LABEL_LENGTH) {
            (*category) = LABEL_FLAG;
            state = EXIT;

        } else {
            labelNameError(line,word);
            state = ERROR;
        }
    }
    /*an invalid positioning of the char ':' */
    else{
        ERROR_INVALID_CHAR(line,ch);
        state = ERROR;}

    return state;
}


/*used by parseCategory, this function analyzes a given char ,
 * returns 1 if the type of char and its placement corresponds to the rules,
 * otherwise returns -1*/
int analyzeChar(int* dot,int ch,int* firstLetter,int line){

    int state = VALID;

    if (ch == '.') {

        if (dotValidation((*dot), (*firstLetter), line))
            (*dot) = 1;
        else
            state = ERROR;
    }
    else if (isalpha(ch))
        (*firstLetter) = 1;

    else if (isdigit(ch)) {

        if (!(*firstLetter))  /*a digit is given before a letter which is invalid*/
            state = ERROR;
    }

    else  /*an invalid character was given*/
        state = ERROR;

    return state;
}


/*a dot was found in the beginning of the word, during parseCategory func analyze,
 * so an exam if a proper directive name is being done in this function
 * if valid returns 0
 * otherwise,if invalid returns -1*/
int handleDirCase(int* category, char* word, int line){

    int ans,state = VALID;

    if ((*category) == LABEL_FLAG || (*category) == EMPTY_CATEGORY_FLAG) {
        ans = validDirName(word);
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


/*a function used by parseCategory function, that analyzes if the given arg is a valid instruction / label
 * based on its position in the line,and based on what came beforehand.
 * if the arg is valid,returns 1.
 * otherwise returns -1*/
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


/* the function analyzes each label that was passed as an argument and calls analyzeTypeSymbol to:
 * validate and update the data structures based on their traits that were given from first iteration
 * returns 1 everything was updated and found successfully
 * returns -1 if a label wasn't declared or doesn't correspond to the rules*/
int analyzeLabelTable(labelTablePtr* labelTablehead, symbolPtr* symHead, externTablePtr* extHead, codeImgPtr* codeHead, entryTablePtr* entHead){

    int state = VALID,ans;

    labelTablePtr curr = (*labelTablehead);
    for(;curr;curr = getNextLabelNode(curr)){/*iter through label as arg dataStructure*/
        ans = analyzeTypeSymbol(curr, symHead, extHead, codeHead, entHead);
        if(ans == EXIT){
            if(findEntryLabel(entHead, getLabelTableName(curr)))
                analyzeEntrySymbol(getLabelTableName(curr), entHead, getLabelTableAddress(curr));
            else{
                ERROR_UNUSED_LABEL(getLabelTableLine(curr));
                ans = ERROR;}
        }
        if(ans == ERROR)
            state = ERROR;
    }

    return state;
}


/*the function analyzes a given label from labelTable data structure and matches it to a symbol from the symbolTable
 * returns 1 if the label exists in the symbolTable and corresponds to the rules.
 * if exists but does not correspond to the rules returns -1.
 * otherwise, if doesn't exist at all returns 0*/
int analyzeTypeSymbol(labelTablePtr labelPtr, symbolPtr* symbolHead, externTablePtr* extHead, codeImgPtr* codeHead, entryTablePtr* entHead){

    int state = VALID,found = 0;
    symbolPtr symPtr = (*symbolHead);

    for(;!found && symPtr;symPtr = getSymbolNextNode(symPtr)){ /*iterating through the symbol table*/

        if(!(strcmp(getSymbolLabel(symPtr),getLabelTableName(labelPtr)))){ /*matching name between symbol table and label table*/
            found = 1;
            if(getSymbolType(symPtr) == EXTERN_AT){
                if(getLabelBranch(labelPtr) == J_BRANCHING){ /*la/call/jmp*/
                    state = addToExtTable(extHead, getLabelTableName(labelPtr), getLabelTableAddress(labelPtr),
                                          getLabelTableLine(labelPtr));
                    if(state == ERROR)
                        ERROR_MEMORY_MAXED_OUT(getLabelTableLine(labelPtr));
                }

                else{  /*case its I BRANCH*/
                    state = ERROR;
                    ERROR_EXTERN_I_BRANCH(getLabelTableLine(labelPtr),getLabelTableName(labelPtr));
                }
            }

            else{ /*the labels are matched,and the symbol wasn't declared externally */

                if(findEntryLabel(entHead, getSymbolLabel(symPtr)))
                    analyzeEntrySymbol(getLabelTableName(labelPtr), entHead, getSymbolAddress(symPtr));

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

