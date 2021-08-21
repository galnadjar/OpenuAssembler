#ifndef MAMAN14_HANDLENUMBERS_H
#define MAMAN14_HANDLENUMBERS_H

/*---------------------------- includes ----------------------------*/

#include "errorsHeader.h"
#include "spaceParse.h"


/*---------------------------- defines ----------------------------*/
#define MINUS_OP 2
#define MIN_ONE_BYTE_SIZE (-128)
#define MAX_ONE_BYTE_SIZE (127)
#define MIN_TWO_BYTES_SIZE (-32768)
#define MAX_TWO_BYTES_SIZE 32767
#define MIN_FOUR_BYTES_SIZE (-2147483647)
#define MAX_FOUR_BYTES_SIZE 2147483647

typedef enum{
    ASCIZ_DIR = 1,
    DH_DIR,
    DW_DIR,
    DB_DIR
}dataType;



/*-------------------- functions declarations ---------------------*/

int handleDigit(long* num, int ch, int line, int* firstDigit, long minVal, long maxVal);

int checkTwoByteSize(long num);

int checkValidNumRange(int ch, long num, long minRange, long maxRange);

int handleNumSpace(long* num, int operator, int firstDigit, int line);

int handleNumComma(int operator, long* num, int commaReq, int line, int ch,int firstDigit);

int handleNumOps(int firstDigit, int line, int ch, int* operator);

void adjustValues(long* minVal, long* maxVal,int dir);

int checkAndSetNum(char* lineInput, int i, int line, long* num, int commaReq, long minVal, long maxVal,int* comma);

#endif //MAMAN14_HANDLENUMBERS_H