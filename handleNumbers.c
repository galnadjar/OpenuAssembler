
#include "handleNumbers.h"

/*this function receives a num and a digit and calls for a check if the add is valid ,
 *if valid , applies the add and turn the firstdigit flag on and return 1,
 * otherwise return -1*/
int handleDigit(long* num, int ch, int line, int* firstDigit, const long minVal, const long maxVal){
    int state = VALID;

    state = checkValidNumRange(ch, *num,minVal,maxVal);

    if(!state){
        state = ERROR;
        ERROR_NUMBER_OUT_OF_BOUNDS(line);}

    else{
        (*num) = ((*num)*10) + (ch - '0');
        (*firstDigit) = 1;}

    return state;
}


/*check if the given number is 2 byte sized*/
int checkTwoByteSize(long num){
    return (num >= MIN_TWO_BYTES_SIZE && num <= MAX_TWO_BYTES_SIZE);
}


/*check if the number will stay in the num range after the add , returns 1 if so,otherwise returns 0*/
int checkValidNumRange(int ch, long num,const long minRange,const long maxRange){
    return (num*10) + (ch - '0') >= minRange && (num*10) + (ch - '0') <= maxRange;
}


/*this function handle a given space while analyzing a number,
 * returns 1 if proper space and -1 if improper*/
int handleNumSpace(long* num, int operator, int firstDigit, int line){
    int state = VALID;

    if(!firstDigit){ /*there was an operator sign,and then space without any digit*/
        ERROR_OP_WITHOUT_DIGIT(line);
        state = ERROR;}

    else{/*theres been a digit*/
        if(operator == MINUS_OP)
            (*num) *= -1;
    }

    return state;
}


/*this function handles the case of which a comma was analyzed while reading a number,
 * if the comma is requested and is after a given num return 0,otherwise returns -1*/
int handleNumComma(int operator, long* num, int commaReq, int line, int ch,int firstDigit){
    int state = EXIT;

    if(commaReq){
        if(!firstDigit){ /*a comma before argument*/
            COMMA_WITHOUT_DATA(line);
            state = ERROR;
        }

        else if(operator == MINUS_OP)
            (*num) *= -1;
    }

    else{
        ERROR_INVALID_CHAR(line,ch);
        state = ERROR;}

    return state;
}


/*this function handles arithmetic operation signs while reading a number,
 * if properly placed sets them accordingly and returns 1,
 * otherwise returns -1*/
int handleNumOps(int firstDigit, int line, int ch, int* operator){
    int state = VALID;

    if(firstDigit){
        ERROR_INVALID_CHAR(line,ch);
        state = ERROR;
    }

    else if(*operator){ /* an operator was already given */
        ERROR_IMPROPER_ARIT_OPS(line);
        state = ERROR;
    }

    else{
        if(ch == '-')
            (*operator) = MINUS_OP;
        else
            (*operator) = 1;
    }
    return state;
}


/*adjust min and max values variables based on the given dir*/
void adjustValues(long* minVal, long* maxVal,int dir){

    if(dir == DB_DIR){
        (*minVal) = MIN_ONE_BYTE_SIZE;
        (*maxVal) = MAX_ONE_BYTE_SIZE;}

    else if(dir == DH_DIR){
        (*minVal) = MIN_TWO_BYTES_SIZE;
        (*maxVal) = MAX_TWO_BYTES_SIZE;}

    /*case its word*/
    else{
        (*minVal) = MIN_FOUR_BYTES_SIZE;
        (*maxVal) = MAX_FOUR_BYTES_SIZE;}
}

/*this function responsible for parsing the num given as argument,
 *it sets the num variable and returns the index of the comma/after space if the number was set properly,
 *otherwise, returns -1*/
int checkAndSetNum(char* lineInput, int i, int line, long* num, int commaReq,const long minVal,const long maxVal,int* comma){
    int state = VALID,ch;
    long sum = 0;
    int operator = 0,firstDigit = 0;

    for(;i < strlen(lineInput) -1 && state == VALID;i++){
        ch = (int)lineInput[i];

        if(isspace(ch)){

            if(!firstDigit && !operator) /*no data was given besides white spaces*/
                i = locAfterSpace(lineInput,i)-1;

            else{
                state = handleNumSpace(&sum, operator, firstDigit, line);
                if(state != ERROR){
                    (*num) = sum;
                    firstDigit = 0;
                    (*comma) = 0;}
            }
        }

        else if(ch == ','){
            state = handleNumComma(operator, &sum, commaReq, line, ch,firstDigit);
            if(state != ERROR){
                firstDigit = 0;
                (*num) = sum;
                i--;
                (*comma) = 1;}
        }


        else if(ch == '-' || ch == '+')
            state = handleNumOps(firstDigit, line, ch, &operator);


        else if(isdigit(ch))
            state = handleDigit(&sum, ch, line, &firstDigit, minVal, maxVal);

        else{
            state = ERROR;
            ERROR_INVALID_CHAR(line,ch);}
    }

    if(firstDigit && i == strlen(lineInput) -1){/*case a number was read and the '\n' was following it*/
        state = handleNumSpace(&sum, operator, firstDigit, line);
        if(state != ERROR){
            (*num) = sum;
            (*comma) = 0;}
    }

    if(state == ERROR)
        i = ERROR;

    return i;
}