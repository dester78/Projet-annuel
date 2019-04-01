#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fileHandler.h>

#include <convertCharHandler.h>



short wCharToChar(char **wValue,char **cValue, ConvertParameter wConvertParameter,ConvertParameter cConvertParameter){

    if(cConvertParameter==_FREE_){
        free(*cValue);
    }
    if(strlen(*wValue)!=0){
        do {
            if((*cValue=malloc(sizeof(char)*(strlen(*wValue)+1)))==NULL){
                createErrorReport("Memory allocation error : ",__FILE__,__LINE__,__DATE__,__TIME__);
                return 0;
            }
            wcstombs(*cValue,*wValue,strlen(*wValue));
            (*cValue)[strlen(*wValue)]='\0';
            if(strlen(*wValue)!=strlen(*cValue)){
                free(*cValue);
            }
        }while(strlen(*wValue)!=strlen(*cValue));


        if(wConvertParameter==_FREE_){
            free(*wValue);
        }
    }
    return 1;
}

short charToWchar(char **cValue,char **wValue,ConvertParameter cConvertParameter,ConvertParameter wConvertParameter){

    if(wConvertParameter==_FREE_){
        free(*wValue);
    }
    if(strlen(*cValue)!=0){

        do {
            if((*wValue=malloc(sizeof(char)*(strlen(*cValue)+1)))==NULL){
                createErrorReport("Memory allocation error : ",__FILE__,__LINE__,__DATE__,__TIME__);
                return 0;
            }
            mbstowcs(*wValue,*cValue,strlen(*cValue));
            (*wValue)[strlen(*cValue)]='\0';

            if(strlen(*wValue)!=strlen(*cValue)){
                free(*wValue);
            }
        }while(strlen(*wValue)!=strlen(*cValue));

        if(cConvertParameter==_FREE_){
            free(*cValue);
        }
    }
    return 1;
}

short allocCopyString(char **dst, char *src){

    if((*dst=malloc(sizeof(char)*(strlen(src)+1)))==NULL){
        return 0;
    }
    strcpy(*dst,src);
    return 1;
}


