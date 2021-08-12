//
// Created by xubuntu on 8/4/21.
//

#include "labelHandling.h"

/*if proper label returns its ending index+1, otherwise return -1*/
int analyzeLabel(char* lineInput, int i, int line, char** label){

    int state = VALID,ch,wordStarted = 0,ind = 0;
    char* word = (char*) calloc(MAX_LABEL_LENGTH+1,sizeof(char));

    for(;i < strlen(lineInput) -1 && state == VALID;i++){
        ch = (int)lineInput[i];

        if(isspace(ch)){

            if(!wordStarted)
                continue;

            else{
                state = validLabelORInstruction(lineInput, LABEL_SEARCH);
                if(state == ERROR)
                    ERROR_LABEL_NAME(line);
                else
                    state = EXIT;
            }
        }

        else if(isalpha(ch)){
            if(ind == MAX_LABEL_LENGTH){
                ERROR_LABEL_LENGTH(line);
                state = ERROR;}

            else{
                wordStarted = 1;
                word[ind++] = (char)ch;}
        }

        else if(wordStarted && isdigit(ch))
            word[ind++] = (char)ch;


        else{
            ERROR_LABEL_NAME(line);
            state = ERROR;
        }
    }

    if(i == strlen(lineInput)){
        ERROR_LABEL_NAME(line);
        state = ERROR;}

    if(state == ERROR)
        i = ERROR;

    else{
        strcpy((*label),word);
        free(word);}

    return i;
}

/*returns 1 if not found in the database otherwise if found returns 0*/
int validLabelORInstruction(char* input, int request) {/*returns 1 if not found in the list, and 0 if found*/
    int isFound = 0;
    int bot = 0,top,mid;
    int loc;

    char* savedWords[] = {"add", "and", "andi", "asciz", "beq", "bgt", "blt", "bne", "call", "db", "dh", "dw", "entry",
                          "extern", "jmp", "la", "lb", "lh", "lw", "move", "mvhi", "mvlo", "nor", "nori",
                          "or", "ori", "sb", "sh", "stop", "sub", "subi", "sw"};

    char* instructionWords[] = {"add", "and", "andi", "beq", "bgt", "blt", "bne", "call", "jmp", "la", "lb", "lh", "lw", "move",
                                "mvhi", "mvlo", "nor", "nori", "or", "ori", "sb", "sh", "stop", "sub", "subi", "sw"};

    if(!request)
        top = SAVED_WORDS_LENGTH;

    else /*means its an order*/
        top = INSTRUCTION_WORDS_LENGTH;

    while(top >= bot && !isFound){

        mid = (top + bot) / 2;
        loc = strcmp(input, (request ? instructionWords[mid] : savedWords[mid]));

        if(loc == 0){
            isFound = 1;
        }

        else if(loc < 0){
            top = mid-1;
        }

        else
            bot = mid+1;
    }
    return !isFound;
}

