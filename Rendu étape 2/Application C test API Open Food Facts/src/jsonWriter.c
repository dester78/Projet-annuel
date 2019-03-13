#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <wctype.h>
#include <system.h>

#include <jsonWriter.h>
#include <jsonHandler.h>



short writeJsonElement(JsonElement *jsonElement, wchar_t **jsonString, unsigned long bufferSize){

    writeJsonName(jsonElement,jsonString,bufferSize);
    writeJsonData(jsonElement,jsonString,bufferSize);

    if(jsonElement->parentElement!=NULL){
        if(jsonElement->parentElement->sizeArrChildElement-1!=jsonElement->elementPosition){
            *jsonString=realloc(*jsonString,sizeof(wchar_t)*(wcslen(*jsonString)+2));
            wcscat(*jsonString,L",");
        }
    }
    return 1;
}

short writeJsonData(JsonElement *jsonElement, wchar_t **jsonString,unsigned long bufferSize){

    wchar_t *dataBuffer;
    dataBuffer=malloc(sizeof(wchar_t)*bufferSize);

    switch(jsonElement->jsonType){
        case _LONG_:
            swprintf(dataBuffer,L"%ld",jsonElement->data->lngData);
            break;
        case _DOUBLE_:
            swprintf(dataBuffer,L"%lf",jsonElement->data->dblData);
            wcschr(dataBuffer,',')[0]='.';
            break;

        case _STRING_:
            swprintf(dataBuffer,L"\"%ls\"",jsonElement->data->strData);
            break;

        case _BOOLEAN_:
            if(jsonElement->data->bolData==0){
                swprintf(dataBuffer,L"%ls",L"false");
            }
            else if(jsonElement->data->bolData){
                swprintf(dataBuffer,L"%ls",L"true");
            }
            break;
        case _NULL_:
            swprintf(dataBuffer,L"%ls",jsonElement->data->strData);
            break;
        case _OBJECT_:
            writeAgreggatedValue(jsonElement,jsonString,bufferSize);
            return 1;
        case _ARRAY_:
            writeAgreggatedValue(jsonElement,jsonString,bufferSize);
            return 1;
        default:
            break;
    }

    *jsonString=realloc(*jsonString, (wcslen(*jsonString) + wcslen(dataBuffer)+1) * sizeof(wchar_t));
    wcscat(*jsonString,dataBuffer);
    free(dataBuffer);

    return 1;
}


short writeJsonName(JsonElement *jsonElement, wchar_t **jsonString, unsigned long bufferSize){

    wchar_t *nameBuffer;

    nameBuffer=malloc(sizeof(wchar_t)*bufferSize);

    if(jsonElement->name!=NULL){

        swprintf(nameBuffer,L"\"%ls\":",jsonElement->name);
        *jsonString=realloc(*jsonString, (wcslen(*jsonString) + wcslen(nameBuffer)+1) * sizeof(wchar_t));
        wcscat(*jsonString,nameBuffer);
    }

    free(nameBuffer);
    return 1;
}

short writeAgreggatedValue(JsonElement *jsonElement, wchar_t **jsonString, unsigned long bufferSize){
    wchar_t token[2];

    switch(jsonElement->jsonType){
        case _OBJECT_:
            wcscpy(token,L"{");
            break;
        case _ARRAY_:
            wcscpy(token,L"[");
            break;

        default:
            break;
    }
    *jsonString=realloc(*jsonString, (wcslen(*jsonString) + wcslen(token)+2) * sizeof(wchar_t));

    wcscat(*jsonString,token);
    for(unsigned long i=0;i<jsonElement->sizeArrChildElement;i++){
        writeJsonElement(jsonElement->arrChildElement[i],jsonString,bufferSize);
    }
    *jsonString=realloc(*jsonString, (wcslen(*jsonString) + wcslen(token)+2) * sizeof(wchar_t));
    token[0]+=2;
    wcscat(*jsonString,token);

    return 1;

}