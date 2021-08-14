//
// Created by xubuntu on 7/21/21.
//

#include "instructionFuncs.h"
int getOpCode(char* instruction){

    opcode code;

    if(!strcmp(instruction,"add") || !strcmp(instruction,"sub") || !strcmp(instruction,"and") ||
    !strcmp(instruction,"or") || !strcmp(instruction,"nor"))
        code = logic_arit;

    else if(!strcmp(instruction,"move") || !strcmp(instruction,"mvhi") || !strcmp(instruction,"mvlo"))
        code = copyOC;

    else {
        if (!strcmp(instruction, "addi"))
            code = addi;

        else if(!strcmp(instruction,"subi"))
            code = subi;

        else if(!strcmp(instruction,"andi"))
            code = andi;

        else if(!strcmp(instruction,"ori"))
            code = ori;

        else if(!strcmp(instruction,"nori"))
            code = nori;

        else if(!strcmp(instruction,"bne"))
            code = bne;

        else if(!strcmp(instruction,"beq"))
            code = beq;

        else if(!strcmp(instruction,"blt"))
            code = blt;

        else if(!strcmp(instruction,"bgt"))
            code = bgt;

        else if(!strcmp(instruction,"lb"))
            code = lb;

        else if(!strcmp(instruction,"sb"))
            code = sb;

        else if(!strcmp(instruction,"lw"))
            code = lw;

        else if(!strcmp(instruction,"sw"))
            code = sw;

        else if(!strcmp(instruction,"lh"))
            code = lh;

        else if(!strcmp(instruction,"sh"))
            code = sh;

        else if(!strcmp(instruction,"jmp"))
            code = jmp;

        else if(!strcmp(instruction,"la"))
            code = la;

        else if(!strcmp(instruction,"call"))
            code = call;

        else /*if(!strcmp(instruction,"stop"))*/
        code = stop;
    }
    return code;
}

/*returns 1 if args were valid, and 0 if invalid args were found*/
int checkInsArgs(char* lineInput,char* instruction,int i,int line,long* IC,codeImgPtr* imgHead,labelTablePtr* labelTableHead){

    int state = VALID,funct;
    int opcode = getOpCode(instruction);

    if(opcode < I_CMD_MIN_OPCODE){/*all R type instructs*/
        funct = getFunct(instruction,opcode);
        state = handleRargs(lineInput,i,line,IC,opcode,funct,imgHead);}

    else if(opcode > I_CMD_MAX_OPCODE) /*all J type instructs*/
        state = handleJargs(lineInput, i, line,IC,opcode,imgHead,labelTableHead);

    else /*all I type instructs*/
        state = handleIargs(lineInput, i, line,IC,opcode,imgHead,labelTableHead);

    return state;
}

int getFunct(char* instruction,int opcode){

    int funct;
    if(opcode == 0){

        if(!strcmp("add",instruction))
            funct = ADD_FUNCT;

        else if(!strcmp("sub",instruction))
            funct = SUB_FUNCT;

        else if(!strcmp("and",instruction))
            funct = AND_FUNCT;

        else if(!strcmp("or",instruction))
            funct = OR_FUNCT;

        else /*case its nor*/
        funct = NOR_FUNCT;
    }

    else {/*opcode is 1*/

        if (!strcmp("move", instruction))
            funct = MOVE_FUNCT;

        else if (!strcmp("mvhi", instruction))
            funct = MVHI_FUNCT;

        else /*case its mvlo*/
        funct = MVLO_FUNCT;
    }
    return funct;
}

/*returns 1 if no errors were found , otherwise returns 0*/
int handleRargs(char* lineInput, int i, int line,long* IC,int opcode,int funct,codeImgPtr* imgHead){
    int state = VALID;
    int rs,rt,rd;

    state = setRcmdReg(lineInput,i,line,&rs,&rt,&rd,opcode);

    if(state != ERROR){
        state = addRCodeNode(rs,rt,rd,*IC,opcode,funct,imgHead,line);
        if(!state)
            ERROR_MEMORY_MAXED_OUT(line);}

    else
        state = 0;

    return state;
}


/*returns 1 if no errors were found , otherwise returns 0*/
int handleJargs(char* lineInput, int i, int line,long* IC,int opcode,codeImgPtr* imgHead,labelTablePtr* labelTableHead){

    int state = VALID;
    int reg;
    long address = 0;
    char* label = NULL;
    state = setJcmdReg(lineInput,i,line,&reg,&address,opcode,&label);

    if(state != ERROR){
        if(label)
            addToLabelTable(labelTableHead,label,*IC,J_BRANCHING,line);

        state = addJCodeNode(reg,*IC,address,opcode,imgHead,label,line);
        if(!state)
            ERROR_MEMORY_MAXED_OUT(line);}
    else
        state = 0;

    return state;
}

/*returns 1 if no errors were found , otherwise returns 0*/
int handleIargs(char* lineInput, int i, int line,long* IC,int opcode,codeImgPtr* imgHead,labelTablePtr* labelTableHead){

    int rs,rt,state = VALID;
    long immed;
    char* label;

    if(opcode == bne || opcode == beq || opcode == bgt || opcode == blt){
        label = calloc(MAX_LABEL_LENGTH+1,sizeof(char));
        if(label){
            state = setIcmdWithLabel(lineInput,i,line,&rs,&rt,label);
            addToLabelTable(labelTableHead,label,*IC,I_BRANCHING,line);
        }
        else{
            state = ERROR;
            ERROR_MEMORY_MAXED_OUT(line);}
    }

    else
        state = setIcmdWithoutLabel(lineInput,i,line,&rs,&immed,&rt);

    if(state != ERROR){
        state = addICodeNode(rs,rt,immed,*IC,opcode,imgHead,label,line);
        if(!state)
            ERROR_MEMORY_MAXED_OUT(line);}
    else
        state = 0;

    return state;
}


/*handles cmd with labels,returns 1 if the properties was set properly because of valid input,otherwise returns -1*/
int setIcmdWithLabel(char* lineInput,int i,int line,int* rs,int* rt,char* label) {

    int state = VALID;

    i = checkAndSetReg(lineInput, i, line, rs, 1);
    if (i != ERROR) {
        i = commaAfterSpace(lineInput, i);
        if (i > 0) {
            i = checkAndSetReg(lineInput, i, line, rt, 1);
            if (i != ERROR) {
                i = commaAfterSpace(lineInput, i);
                if (i > 0) {
                    i = analyzeLabel(lineInput,i,line,&label);
                    if (i) {
                        i = locAfterSpace(lineInput, i);
                        if (i != strlen(lineInput) - 1){
                            ERROR_EXTRANEOUS_END_OF_CMD(line);
                            state = ERROR;}
                    }
                }
                else {
                    ERROR_MISSING_COMMA(line);
                    state = ERROR;}
            }
        }
        else {
            ERROR_MISSING_COMMA(line);
            state = ERROR;
        }
    }

    if(i == ERROR)
        state = ERROR;

    return state;
}

/*handles cmd without labels,returns 1 if the properties were set properly because of valid input,otherwise returns -1*/
int setIcmdWithoutLabel(char* lineInput,int i,int line,int* rs,long* immed,int* rt) {

    int state = VALID;

    i = checkAndSetReg(lineInput, i, line, rs, 1);
    if (i != ERROR) {
        i = commaAfterSpace(lineInput, i);
        if (i > 0) {
            i = checkAndSetNum(lineInput, i, line, immed, 1,MIN_TWO_BYTES_SIZE,MAX_TWO_BYTES_SIZE);
            if (i != ERROR) {
                state = checkTwoByteSize(*immed);
                if(state > 0){
                    i = commaAfterSpace(lineInput, i);
                    if (i > 0) {
                        i = checkAndSetReg(lineInput,i,line,rt,0);
                        if (i != ERROR) {
                            i = locAfterSpace(lineInput, i);

                            if (i != strlen(lineInput) - 1){
                                ERROR_EXTRANEOUS_END_OF_CMD(line);
                                state = ERROR;}
                        }
                    }
                    else {
                        ERROR_MISSING_COMMA(line);
                        state = ERROR;}
                }
                else
                    ERROR_NUMBER_OUT_OF_BOUNDS(line);
            }
        }
        else {
            ERROR_MISSING_COMMA(line);
            state = ERROR;}
    }

    if(i < 1)
        state = ERROR;

    return state;
}

int regNumCheck(int reg){
    return reg >= MIN_REG_NUM && reg <= MAX_REG_NUM;
}

int setRcmdReg(char* lineInput,int i,int line,int* rs,int* rt,int* rd,int opcode){

    int state = VALID;

    i = checkAndSetReg(lineInput, i, line, rs,1);
    if(i != ERROR) { /*currently with 1 reg*/
        i = commaAfterSpace(lineInput, i);
        if (i > 0) { /*currently with 1 reg and a comma*/

            if(opcode == logic_arit){
                i = checkAndSetReg(lineInput, i, line, rt,1);
                if (i != ERROR) /*check if a proper reg*/
                    i = commaAfterSpace(lineInput, i);
            }

            else /*case it copy instructions*/
                (*rt) = 0;

            if (i > 0){/*check if comma was found*/
                i = checkAndSetReg(lineInput, i, line, rd,0);
                if (i != ERROR) {
                    i = locAfterSpace(lineInput, i);

                    if (i != strlen(lineInput) - 1) { /*check if no more args were passed in the line*/
                        ERROR_EXTRANEOUS_END_OF_CMD(line);
                        state = ERROR;}
                }
            }
        }
        else{
            ERROR_MISSING_COMMA(line);
            state = ERROR;}
    }

    if(i == ERROR)
        state = ERROR;

    return state;
} /*end of setRcmdReg*/

/*return 1 if no erros, and -1 if errors were found*/
int setJcmdReg(char* lineInput,int i,int line,int* reg,long* address,int opcode,char** label){
    int state = VALID;

    if(opcode != stop && locAfterSpace(lineInput,i) == strlen(lineInput)-1){/*empty instruction*/
        state = ERROR;
        ERROR_EMPTY_INSTRUCTION(line);
    }

    else if(opcode == jmp){
        i = analJmp(lineInput,i,line,reg,address,label);
        if(i != ERROR){
            i = locAfterSpace(lineInput,i);
            if(i != strlen(lineInput)-1)
                ERROR_EXTRANEOUS_END_OF_CMD(line);
        }
    }

    else if(opcode == stop) {
        state = analStop(lineInput, i, line);
        if (state)
            (*reg) = 0;
    }

    else{ /*case its la or call*/
        (*label) = (char*) calloc(MAX_LABEL_LENGTH+1,sizeof(char));
        i = analyzeLabel(lineInput,i,line,label);
        if(i != ERROR){
            i = locAfterSpace(lineInput,i);
            if(i != strlen(lineInput)-1){
                ERROR_EXTRANEOUS_END_OF_CMD(line);
                state = ERROR;}
            else
                (*reg) = 0;
        }
    }
    return state;
}


int analStop(char* lineInput,int i,int line){

    int state = VALID;
    i = locAfterSpace(lineInput,i);
    if(i != strlen(lineInput)-1){
        ERROR_EXTRANEOUS_END_OF_CMD(line);
        state = ERROR;
    }
    return state;
}


int analJmp(char* lineInput,int i,int line,int* reg,long* address,char** labelName) {

    int option, state = VALID;
    option = checkFirstLetter(lineInput, i); /*based on the first letter ,selects which arg will be received*/

    switch (option) {

        case REG_OPTION:
            state = checkAndSetReg(lineInput,i,line,(int*)address,0);
            if(state != ERROR)
                (*reg) = 1;
            break;

        case LABEL_OPTION:
            (*labelName) = (char*) calloc(MAX_LABEL_LENGTH+1,sizeof(char));
            state = analyzeLabel(lineInput,i,line,labelName);
            if(state != ERROR)
                (*reg) = 0;
            break;


        default:/*first letter that was passed as an instruction arg was invalid*/
            state = ERROR;
            ERROR_INVALID_CHAR(line, lineInput[locAfterSpace(lineInput, i)]);
            break;

    }
    return state;
}

/*check if the first letter given is directed to a register or labelname*/
int checkFirstLetter(char* lineInput,int i){
    int option;
    int state = VALID,ch;
    for(;i < strlen(lineInput)-1 && state == VALID;i++){
        ch = (int)lineInput[i];
        if(isspace(ch))
            i = locAfterSpace(lineInput,i)-1;

        else{
            if(ch == '$')
                option = REG_OPTION;

            else if(isalpha(ch))
                option = LABEL_OPTION;

            else{
                option = ERROR;
            }
            state = EXIT;
        }
    }
    return option;
}


/*the function returns the index if the registry was set properly,otherwise returns -1*/
int checkAndSetReg(char* lineInput, int i, int line,int* reg,int commaReq){
    int digit = 0,regSign = 0,regNum = 0,ch,state = VALID;

    for(;i < strlen(lineInput)-1 && state == VALID;i++){

        ch = (int)lineInput[i];
        if(isspace(ch)) {
            if (!regSign)
                i = locAfterSpace(lineInput, i) - 1;

            else/*handles a space that came after a '$'*/
                state = handleRegSpace(digit,reg,regNum,line);
        }
        else if(isdigit(ch))
            state = handleRegDigit(regSign,&digit,&regNum,ch,line);

        else if(ch == ','){
            state = handleRegComma(reg,regNum,line, ch, digit, regSign, commaReq);
            i--;}

        else if(ch == '$'){
            if(!regSign)
                regSign = 1;

            else{ /*there was a reg sign already*/
                ERROR_INVALID_REG_NAME(line);
                state = ERROR;}}
        else{
            ERROR_INVALID_CHAR(line,ch);
            state = ERROR;
        }
    }

    if(state == ERROR)
        i = ERROR;

    return i;
}


/*handles the case of spacing after '$'*/
int handleRegSpace(int digit,int* reg,int regNum,int line){

    int state = VALID;

    if (digit) {
        (*reg) = regNum;
        state = EXIT;
    }

    else {
        ERROR_INVALID_REG_NAME(line);
        state = ERROR;
    }
    return state;
}

/*handles the case of digit encounter in reg name*/

int handleRegDigit(int regSign,int* digit,int* regNum,int ch,int line){

    int state = VALID;
    if (regSign) {
        (*digit) = 1;
        (*regNum) = ((*regNum)*10) +(ch - '0');

        state = regNumCheck(*regNum);
        if(!state){
            ERROR_REG_OUT_OF_BOUNDS(line);
            state = ERROR;}
    }

    else{
        ERROR_FIRST_LETTER_DIGIT(line);
        state = ERROR;
    }
    return state;
}

/*handles the case of comma in a reg name*/
int handleRegComma(int* reg,int regNum,int line,int ch,int digit,int regSign, int commaReq){
    int state = VALID;

    if (!regSign) {/*comma before everything*/
        ERROR_INVALID_CHAR(line, ch);
        state = ERROR;}

    else if (!digit) { /*its a regSign without a digit afterwards*/
        ERROR_INVALID_REG_NAME(line);
        state = ERROR;
    } else { /* a comma after the reg name approved only if requested*/
        if (commaReq) {
            (*reg) = regNum;
            state = EXIT;

        } else {
            ERROR_INVALID_CHAR(line, ch);
            state = ERROR;
        }
    }
    return state;
}