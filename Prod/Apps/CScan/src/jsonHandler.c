#define BUFFER_SIZE 1000


#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <fileHandler.h>
#include <jsonHandler.h>


short fileFileIndex(FILE *srcFile, File ***fileIndex){

    int fileCounter=0;
    char fileName[BUFFER_SIZE];
    char path[BUFFER_SIZE];
    char openMode[BUFFER_SIZE];

    while(feof(srcFile)!=EOF){
        if(fscanf(srcFile,"\"name\":\"%s\",\"path\":\"%s\",\"openMode\":\"%s\"",&fileName,&path,&openMode)){
            (*fileIndex)[fileCounter]=malloc(sizeof(File));
            (*fileIndex)[fileCounter]=initFileElement(fileName,path,openMode);
            fileCounter++;
        }
    }
}

char** writeJson(JsonData **jsonData){

    char **jsonString;

    switch((*jsonData)->jsonValueType){
        case INTEGER:

        break;

        case REAL:
        break;

        case STRING:
        break;

        case OBJECT:
        break;

        case BOOLEAN:
        break;

        case ARRAY:
        break;
    }

    return jsonString;
}



