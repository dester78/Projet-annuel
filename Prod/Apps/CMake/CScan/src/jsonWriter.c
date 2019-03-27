#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <wctype.h>
#include <curl/system.h>
#include <wchar.h>

#include <jsonWriter.h>
#include <jsonHandler.h>



short writeJsonElement(JsonElement *jsonElement, wchar_t **jsonString, unsigned long bufferSize){

    writeJsonName(jsonElement,jsonString,bufferSize);
    writeJsonData(jsonElement,jsonString,bufferSize);

    //Si un élément fait partie d'un ensemble d'élément et qu(il n'est pas le dernier de son ensemble alors on ajoute en fin de chaine json le caractère ','
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

    //Applique des conversions de types quelconques en chaine de caractere
    switch(jsonElement->jsonType){
        case _LONG_:
            swprintf(dataBuffer,bufferSize,L"%ld",jsonElement->data->lngData);
            break;
        case _DOUBLE_:
            swprintf(dataBuffer,bufferSize,L"%lf",jsonElement->data->dblData);
            wcschr(dataBuffer,',')[0]='.';
            break;

        case _STRING_:
            swprintf(dataBuffer,bufferSize,L"\"%ls\"",jsonElement->data->strData);
            break;

        case __BOOLEAN__:
            if(jsonElement->data->bolData==0){
                swprintf(dataBuffer,bufferSize,L"%ls",L"false");
            }
            else if(jsonElement->data->bolData){
                swprintf(dataBuffer,bufferSize,L"%ls",L"true");
            }
            break;
        case _NULL_:
            swprintf(dataBuffer,bufferSize,L"%ls",jsonElement->data->strData);
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

    //Concatene la chaine buffer avec la chaine de caractère json
    *jsonString=realloc(*jsonString, (wcslen(*jsonString) + wcslen(dataBuffer)+1) * sizeof(wchar_t));
    wcscat(*jsonString,dataBuffer);
    free(dataBuffer);

    return 1;
}


short writeJsonName(JsonElement *jsonElement, wchar_t **jsonString, unsigned long bufferSize){

    wchar_t *nameBuffer;

    nameBuffer=malloc(sizeof(wchar_t)*bufferSize);

    if(jsonElement->name!=NULL){

        //Ajoute le caractère ':' qui caractèrise les noms dans le format json
        swprintf(nameBuffer,bufferSize,L"\"%ls\":",jsonElement->name);
        *jsonString=realloc(*jsonString, (wcslen(*jsonString) + wcslen(nameBuffer)+1) * sizeof(wchar_t));
        wcscat(*jsonString,nameBuffer);
    }

    free(nameBuffer);
    return 1;
}

short writeAgreggatedValue(JsonElement *jsonElement, wchar_t **jsonString, unsigned long bufferSize){
    wchar_t token[2];

    //Sélectionne un délimiteur particulier en fonction du type d'objet json envoyé à la fonction
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
    //Rappelle la fonction writeJsonElement pour chaque élements de l'ensemble
    for(unsigned long i=0;i<jsonElement->sizeArrChildElement;i++){
        writeJsonElement(jsonElement->arrChildElement[i],jsonString,bufferSize);
    }

    //Utilise le code ASCII du délimiteur d'ouverture +2 pour ajouter un délimiteur de fermeture
    *jsonString=realloc(*jsonString, (wcslen(*jsonString) + wcslen(token)+2) * sizeof(wchar_t));
    token[0]+=2;
    wcscat(*jsonString,token);

    return 1;

}