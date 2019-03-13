#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <wCharHandler.h>



short wCharToChar(wchar_t **wValue,char **cValue, ConvertParameter wConvertParameter,ConvertParameter cConvertParameter){

    if(cConvertParameter==_FREE_){
        printf("CVALUE : %s",*cValue);
        free(*cValue);
    }
    if(wcslen(*wValue)!=0){
        *cValue=malloc(sizeof(char)*(wcslen(*wValue)+1));
        wcstombs(*cValue,*wValue,wcslen(*wValue));
        if(wConvertParameter==_FREE_){
            free(*wValue);
        }
    }
    return 0;
}

short charToWchar(char **cValue,wchar_t **wValue,ConvertParameter cConvertParameter,ConvertParameter wConvertParameter){

    if(wConvertParameter==_FREE_){
        free(*wValue);
    }
    if(strlen(*cValue)!=0){
        *wValue=malloc(sizeof(wchar_t)*(strlen(*cValue)+1));
        mbstowcs(*wValue,*cValue,strlen(*cValue));
        if(cConvertParameter==_FREE_){
            free(*cValue);
        }
    }
    return 0;
}

short allocCopyString(wchar_t **dst, wchar_t *src){

    if((*dst=malloc(sizeof(wchar_t)*(wcslen(src)+1)))==NULL){
        return 0;
    }
    wcscpy(*dst,src);
    return 1;
}
