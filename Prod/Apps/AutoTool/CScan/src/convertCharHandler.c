#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fileHandler.h>

#include <convertCharHandler.h>



short wCharToChar(wchar_t **wValue,char **cValue, ConvertParameter wConvertParameter,ConvertParameter cConvertParameter){

    if(cConvertParameter==_FREE_){
        free(*cValue);
    }
    if(wcslen(*wValue)!=0){
        do {
            if((*cValue=malloc(sizeof(char)*(wcslen(*wValue)+1)))==NULL){
                createErrorReport("Memory allocation error : ",__FILE__,__LINE__,__DATE__,__TIME__);
                return 0;
            }
            wcstombs(*cValue,*wValue,wcslen(*wValue));
            (*cValue)[wcslen(*wValue)]='\0';
            if(wcslen(*wValue)!=strlen(*cValue)){
                free(*cValue);
            }
        }while(wcslen(*wValue)!=strlen(*cValue));


        if(wConvertParameter==_FREE_){
            free(*wValue);
        }
    }
    return 1;
}

short charToWchar(char **cValue,wchar_t **wValue,ConvertParameter cConvertParameter,ConvertParameter wConvertParameter){

    if(wConvertParameter==_FREE_){
        free(*wValue);
    }
    if(strlen(*cValue)!=0){

        do {
            if((*wValue=malloc(sizeof(wchar_t)*(strlen(*cValue)+1)))==NULL){
                createErrorReport("Memory allocation error : ",__FILE__,__LINE__,__DATE__,__TIME__);
                return 0;
            }
            mbstowcs(*wValue,*cValue,strlen(*cValue));
            (*wValue)[strlen(*cValue)]='\0';

            if(wcslen(*wValue)!=strlen(*cValue)){
                free(*wValue);
            }
        }while(wcslen(*wValue)!=strlen(*cValue));

        if(cConvertParameter==_FREE_){
            free(*cValue);
        }
    }
    return 1;
}

short allocCopyString(wchar_t **dst, wchar_t *src){

    if((*dst=malloc(sizeof(wchar_t)*(wcslen(src)+1)))==NULL){
        return 0;
    }
    wcscpy(*dst,src);
    return 1;
}


