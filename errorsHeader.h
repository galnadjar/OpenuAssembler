#ifndef MAMAN14_ERRORSHEADER_H
#define MAMAN14_ERRORSHEADER_H

/*---------------------------- includes ----------------------------*/

#include <stdio.h>

/*---------------------------- defines ----------------------------*/
#define VALID 1
#define ERROR (-1)
#define EXIT 0
#define MAX_ROW_LENGTH 81
#define MAX_LABEL_LENGTH 31


/*-------------------- functions declarations ---------------------*/
#define ERROR_MEMORY_MAXED_OUT(X) printf("ERROR: Memory ran out, couldn't save data from line %d in the file\n",X)

#define ERROR_NO_FILES printf("ERROR: no file was entered\n")

#define ERROR_FILE_IS_EMPTY(X) printf("ERROR: the file \"%s\" is empty.\n",X)

#define ERROR_WRONG_EXTENSION(X) printf("ERROR: the file \"%s\" has incompatible extension , please provide a valid assembly file.\n",X)

#define ERROR_FILE_DONT_EXIST(X) printf("ERROR: the file \"%s\" doesn't exist in the directory, please make sure the name is valid.\n",X)

#define ERROR_IMPROPER_DOT(X) printf("ERROR: In line %d, an improper dot was found\n",X)

#define ERROR_CONSECUTIVE_DOT(X) printf("ERROR: In line %d, multiple consecutive dot were found\n",X)

#define ERROR_MAX_PROG(X) printf("ERROR: In line %d, the file Exceeded the max memory size\n",X)

#define ERROR_LABEL_NAME(X) printf("ERROR: In line %d, a label with an invalid name was found\n",X)

#define ERROR_IMPROPER_INSTRUCTION_NAME(X) printf("ERROR: In line %d, an improper instruction name was found\n",X)

#define ERROR_IMPROPER_DIRECTIVE_NAME(X) printf("ERROR: In line %d, an improper directive word was found\n",X)

#define ERROR_INVALID_PLACEMENT_OF_DIGIT(X) printf("ERROR: In line %d, an invalid placement of digit was found\n",X)

#define ERROR_INVALID_CHAR(X,Y) printf("ERROR: In line %d, extraneous text starts with the char \"%c\" was found\n",X,Y)

#define ERROR_INVALID_ASCIZ(X) printf("ERROR: In line %d, an invalid asciz char was given\n",X)

#define ERROR_LABEL_LENGTH(X) printf("ERROR: In line %d, the label given exceeded the maximum length of a valid label\n",X)

#define ERROR_EXTERN_ENTRY_MIX(X,Y) printf("ERROR: In line %d, the label \"%s\" was assigned both extern and entry instructions\n",X,Y)

#define ERROR_INVALID_DIRECTIVE_ARG(X) printf("ERROR: In line %d,a an invalid directive arg was inserted\n",X)

#define ERROR_INVALID_INSTRUCTION_ARG(X) printf("ERROR: In line %d,a an invalid instruction arg was inserted\n",X)

#define ERROR_MISSING_COMMA(X) printf("ERROR: In line %d, a comma that was required, was missing\n",X)

#define ERROR_FIRST_LETTER_DIGIT(X) printf("ERROR: In line %d, an argument was started invalidly with a digit\n",X)

#define ERROR_REG_OUT_OF_BOUNDS(X) printf("ERROR: In line %d,registry given as an arg is out of the reg bounds\n",X)

#define ERROR_INVALID_REG_NAME(X) printf("ERROR: In line %d,an invalid registry name was given\n",X)

#define ERROR_EXTRANEOUS_END_OF_CMD(X) printf("ERROR: In line %d,extraneous text after end of command was found\n",X)

#define ERROR_LABEL_WITHOUT_SPACE(X) printf("ERROR: In line %d,a line that started with label didn't have space after\n",X)

#define ERROR_EMPTY_INSTRUCTION(X) printf("ERROR: In line %d,instruction received no arguments while needed\n",X)

#define ERROR_OP_WITHOUT_DIGIT(X) printf("ERROR: In line %d,a arithmetic operator was given without a following digit\n",X)

#define ERROR_IMPROPER_ARIT_OPS(X) printf("ERROR: In line %d, arithmetic signs were improperly placed\n",X)

#define ERROR_NUMBER_OUT_OF_BOUNDS(X) printf("ERROR: In line %d,the number that was given was out of bounds\n",X)

#define ERROR_MISSING_QUOTATION(X) printf("ERROR: In line %d,quotation were missing from the argument\n",X)

#define ERROR_ATTRIBUTE_ASSIGN(X) printf("ERROR: In line %d,an invalid attribute assign to an existing label was found\n",X)

#define ERROR_EXTERN_I_BRANCH(X,Y) printf("ERROR: In line %d the label \"%s\" was defined as external yet tried to be jumped to locally\n",X,Y)

#define ERROR_JUMP_OUT_OF_BOUNDS(X) printf("ERROR: In line %d the jumping destination was out of bounds",X)

#define ERROR_MEMORY_ALLOCATION(X) printf("ERROR: system couldn't allocate space for \"%s\" file on the disk\n",X)

#endif //MAMAN14_ERRORSHEADER_H
