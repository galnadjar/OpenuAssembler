#include "printFile.h"

/*creates and return a valid file pointer if memory allow file allocation,otherwise returns null*/
FILE* createWriteFile(char* name,char* extension){

    FILE* fp = NULL;
    char fileName[strlen(name) + strlen(extension)+1];
    strcpy(fileName, name);
    strcat(fileName, extension);

    fp = fopen(fileName,"w");
    if(!fp)
        ERROR_MEMORY_ALLOCATION(fileName);

    return fp;
}

/*writes the ob file*/
void writeOB(char* name,dataImgPtr dataPtr,codeImgPtr codePtr,const long DCF,const long ICF){

    FILE* fp = NULL;
    char* ob = ".ob";

    fp = createWriteFile(name,ob);
    if(fp){
        printCounters(fp,ICF,DCF);
        if(codePtr)
            printCodeImg(fp,codePtr);
        if(dataPtr)
            printDataImg(fp,dataPtr,ICF,DCF);
    }
}


/*prints the data img to the file*/
void printDataImg(FILE* fp, dataImgPtr dataPtr, long ICF,const long DCF){

    dataImgPtr curr = dataPtr;
    long DC = ICF;
    int bytesCounter = 0;
    fprintf(fp,"%04ld\t",DC);
    while(curr){
        printDataDisplay(fp,curr,&bytesCounter,&DC,DCF);
        curr = getNextDataNode(curr);}
}


/*prints the code img to the file*/
void printCodeImg(FILE* fp,codeImgPtr codePtr){

    while(codePtr){
        fprintf(fp,"%04ld\t%02X\t%02X\t%02X\t%02X\n",getCodeAddress(codePtr),getCodeDisplay(codePtr,A_BYTE),
                getCodeDisplay(codePtr,B_BYTE),getCodeDisplay(codePtr,C_BYTE),getCodeDisplay(codePtr,D_BYTE));
        codePtr = getNextCodeNode(codePtr);
    }
}


/*prints the IC and DC counters to the file*/
void printCounters(FILE* fp ,long IC, long DC){
    fprintf(fp,"\t\t%ld\t%ld\n",IC-IC_INITIAL_ADDRESS,DC-IC);
}


/*this function prints the entry data structure to the given file*/
void printEnt(FILE* fp,entryTablePtr entryPtr){

    entryTablePtr curr = entryPtr;
    while(curr){
        fprintf(fp,"%s\t%04ld\n", getEntryLabel(curr),getEntAddress(curr));
        curr = getNextEntNode(curr);
    }
}


/*this function creates a file for the entry data structure,
 * if the memory is available for the name of the file and the file itself to be created, prints it*/
void writeEntry(char* name,entryTablePtr entryPtr){

    FILE* fp;
    char *extension;
    extension = ".ent";
    fp = createWriteFile(name, extension);
    if(fp)
        printEnt(fp,entryPtr);
}


/*this function creates a file for the extern data structure,
 * if the memory is available for the name of the file and the file itself to be created, prints it*/
void writeExtern(char* name, externTablePtr externPtr){

    FILE* fp;
    char *extension;
    extension = ".ext";
    fp = createWriteFile(name, extension);
    if(fp)
        printExt(fp,externPtr);
}


/*this function prints the extern data structure to the given file*/
void printExt(FILE* fp,externTablePtr externPtr){

    externTablePtr curr = externPtr;
    while(curr){
        fprintf(fp,"%04ld\t%s\n",getExtAddress(curr), getExternLabel(curr));
        curr = getNextExtNode(curr);
    }
}