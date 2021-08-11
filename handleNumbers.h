//
// Created by xubuntu on 8/3/21.
//

#ifndef MAMAN14_HANDLENUMBERS_H
#define MAMAN14_HANDLENUMBERS_H

#include "errorsHeader.h"
#include "spaceParse.h"

#define MAX_ROW_LENGTH 81

#define VALID 1
#define ERROR (-1)
#define EXIT 0

#define MINUS_OP 2
typedef enum{
    ASCIZ_DIR,
    DH_DIR,
    DW_DIR,
    DB_DIR
}dataType;

#define MIN_ONE_BYTE_SIZE (-128)
#define MAX_ONE_BYTE_SIZE (127)

#define MIN_TWO_BYTES_SIZE (-32768)
#define MAX_TWO_BYTES_SIZE 32767

#define MIN_FOUR_BYTES_SIZE (-2147483647)
#define MAX_FOUR_BYTES_SIZE 2147483647

int handleNum(long* num, int ch, int line, int* firstDigit, long minVal, long maxVal);

/*the function returns the index after the comma/space if the number was set properly,otherwise returns -1*/
int checkAndSetNum(char* lineInput, int i, int line, long* num, int commaReq, long minVal, long maxVal);
int checkTwoByteSize(long num);

/*check if the number will stay in the num range, returns 1 if so,otherwise returns 0*/
int checkValidNumRange(int ch, long num, long minRange, long maxRange);

/*returns 0 if proper space and -1 if improper*/
int handleNumSpace(long* num, int operator, int firstDigit, int line);
int handleNumComma(int operator, long* num, int commaReq, int line, int ch);
int handleNumOps(int firstDigit, int line, int ch, int* operator);
void adjustValues(long* minVal, long* maxVal,int dir);
#endif //MAMAN14_HANDLENUMBERS_H