#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <wctype.h>
#include <wchar.h>

#include <jsonWriter.h>
#include <jsonHandler.h>



short writeJsonElement(JsonElement *jsonElement, char **jsonString, unsigned long bufferSize){

    
    writeJsonName(jsonElement,jsonString,bufferSize);
    writeJsonData(jsonElement,jsonString,bufferSize);
    
wprintf(L"VOOL\n");
wprintf(L"%ls\n",jsonString);
fflush(stdout);
    //Si un élément fait partie d'un ensemble d'élément et qu(il n'est pas le dernier de son ensemble alors on ajoute en fin de chaine json le caractère ','
    if(jsonElement->parentElement!=NULL){
        wprintf(L"ARG\n");
fflush(stdout);

        if(jsonElement->parentElement->sizeArrChildElement-1!=jsonElement->elementPosition){
            if((*jsonString=realloc(*jsonString,sizeof(char)*(strlen(*jsonString)+2)))==NULL){
                wprintf(L"ARG\n");
                fflush(stdout);
                return 0;
            }
            strcat(*jsonString,",");
        }
    }
    return 1;
}

short writeJsonData(JsonElement *jsonElement, char **jsonString,unsigned long bufferSize){

    char *dataBuffer;
    dataBuffer=malloc(sizeof(char)*bufferSize);

    //Applique des conversions de types quelconques en chaine de caractere
    switch(jsonElement->jsonType){
        case _LONG_:
            sprintf(dataBuffer,"%ld",jsonElement->data->lngData);
            break;
        case _DOUBLE_:
            sprintf(dataBuffer,"%lf",jsonElement->data->dblData);
            strchr(dataBuffer,',')[0]='.';
            break;

        case _STRING_:
            sprintf(dataBuffer,"\"%s\"",jsonElement->data->strData);
            break;

        case __BOOLEAN__:
            if(jsonElement->data->bolData==0){
                sprintf(dataBuffer,"%s","false");
            }
            else if(jsonElement->data->bolData){
                sprintf(dataBuffer,"%s","true");
            }
            break;
        case _NULL_:
            sprintf(dataBuffer,"%s",jsonElement->data->strData);
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
    *jsonString=realloc(*jsonString, (strlen(*jsonString) + strlen(dataBuffer)+1) * sizeof(char));
    strcat(*jsonString,dataBuffer);
    free(dataBuffer);

    return 1;
}


short writeJsonName(JsonElement *jsonElement, char **jsonString, unsigned long bufferSize){

    char *nameBuffer;

    nameBuffer=malloc(sizeof(char)*bufferSize);

    if(jsonElement->name!=NULL){

        //Ajoute le caractère ':' qui caractèrise les noms dans le format json
        sprintf(nameBuffer,"\"%s\":",jsonElement->name);
        *jsonString=realloc(*jsonString, (strlen(*jsonString) + strlen(nameBuffer)+1) * sizeof(char));
        strcat(*jsonString,nameBuffer);
    }

    free(nameBuffer);
    return 1;
}

short writeAgreggatedValue(JsonElement *jsonElement, char **jsonString, unsigned long bufferSize){
    char token[2];

    //Sélectionne un délimiteur particulier en fonction du type d'objet json envoyé à la fonction
    switch(jsonElement->jsonType){
        case _OBJECT_:
            strcpy(token,"{");
            break;
        case _ARRAY_:
            strcpy(token,"[");
            break;

        default:
            break;
    }
    *jsonString=realloc(*jsonString, (strlen(*jsonString) + strlen(token)+2) * sizeof(char));

    strcat(*jsonString,token);
    //Rappelle la fonction writeJsonElement pour chaque élements de l'ensemble
    for(unsigned long i=0;i<jsonElement->sizeArrChildElement;i++){
        writeJsonElement(jsonElement->arrChildElement[i],jsonString,bufferSize);
    }

    //Utilise le code ASCII du délimiteur d'ouverture +2 pour ajouter un délimiteur de fermeture
    *jsonString=realloc(*jsonString, (strlen(*jsonString) + strlen(token)+2) * sizeof(char));
    token[0]+=2;
    strcat(*jsonString,token);

    return 1;

}