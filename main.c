#include <stdio.h>
#include "errorsHeader.h"
#include "fileHandling.h"

int main(int argc,char* argv[]) {

    char extension[] = ".as";
    int state; /*1 means its ok , 0 means an error was found*/
    int i;

    if (argc < 2) {
        ERROR_NO_FILES;
    }

    else { /* theres a file*/
        for (i = 1; i < argc; i++) {

            /*check if the extension is .as*/
            state = !strcmp(strrchr(argv[i], '.'), extension);

            if (!state)
                ERROR_WRONG_EXTENSION(argv[i]);

            else{
                openFile(argv[i]);
                if(i < argc-1)
                    printf("\n\n");}
        }
    }
    return 0;
}
