#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <system.h>

#include <jsonHandler.h>
#include <jsonReader.h>
#include <fileHandler.h>


JsonElement *readJsonFile(FILE *jsonFile,int deepness){

    wchar_t *jsonContent;
    JsonElement *json;
    unsigned long cursor=0;
    jsonContent=getFileContent(jsonFile);
    formatJsonString(&jsonContent);
    wprintf(L"JSON FORMATED : %ls",jsonContent);
    json=readJsonString(jsonContent,&cursor,0,NULL,deepness);
    return json;
}

short formatJsonString(wchar_t **jsonString){

    wchar_t *tmpString;
    unsigned long i;
    unsigned long j=0;
    tmpString=malloc(sizeof(wchar_t*)*(wcslen(*jsonString)+1));
    for(i=0;i<wcslen(*jsonString);i++){
        if((*jsonString)[i]!=L' '&&(*jsonString)[i]!=L'\n'&&controlCharInString((*jsonString),i)==0){
            tmpString[j]=(*jsonString)[i];
            j++;
        }
        else if(controlCharInString((*jsonString),i)){
            tmpString[j]=(*jsonString)[i];
            j++;
        }
    }
    tmpString[j]='\0';
    free(*jsonString);
    *jsonString=tmpString;
    return 0;

}

JsonElement *readJsonString(wchar_t *jsonString,unsigned long *cursor,unsigned long elementPosition, JsonElement *parentElement, int deepness){

    JsonElement *jsonElement;
    JsonType dataType;
    unsigned long lastCursorPosition=*cursor;
    Data *data;
    wchar_t *name;

    data=malloc(sizeof(Data));
    if((name=readName(jsonString,cursor))==NULL){
        *cursor=lastCursorPosition;
        readData(jsonString,cursor,&dataType,data);
        *cursor+=1;
    }
    else{
        readData(jsonString,cursor,&dataType,data);
    }
    jsonElement=initJsonElement(0,elementPosition,name,data,dataType,parentElement);

    if(dataType==_OBJECT_||dataType==_ARRAY_){
        unsigned long endArray=0;
        if(countDeepness(jsonElement,0)<deepness){
            jsonElement->sizeArrChildElement=countChildElements(jsonString,*cursor,dataType,&endArray);
            jsonElement->arrChildElement=malloc(sizeof(JsonElement*)*jsonElement->sizeArrChildElement);
            for(unsigned long i=0; i<jsonElement->sizeArrChildElement; i++){
                jsonElement->arrChildElement[i]=readJsonString(jsonString,cursor,i,jsonElement,deepness);
            }
            if(jsonString[*cursor]!='{'&&jsonString[*cursor]!='['&&controlCharInString(jsonString,*cursor)==0){
                *cursor+=1;
            }
        }
        else{
            *cursor=endArray;
        }
    }
    return jsonElement;
}

wchar_t *readName(wchar_t *jsonString, unsigned long *cursor){

    wchar_t *name;
    for(unsigned long i=*cursor; i<wcslen(jsonString);i++){
        if((jsonString[i]==L'['||jsonString[i]==L']'||jsonString[i]==L'{'||jsonString[i]==L'}')&&controlCharInString(jsonString,i)==0){
            *cursor=i;
            return NULL;
        }
        else if(jsonString[i]==':'&&controlCharInString(jsonString,i)==0){
            name=readStringType(jsonString,i);
            *cursor=i;
            return name;
        }
    }
    return NULL;
}


short readData(wchar_t *jsonString, unsigned long *cursor,JsonType *dataType,Data *data){

    for(unsigned long i=*cursor; i<wcslen(jsonString);i++){
        if(controlCharInString(jsonString,i)){
        }
        else{
        }
        if(jsonString[i]==L'['&&controlCharInString(jsonString,i)==0){
            *cursor=(i+1);
            *dataType=_ARRAY_;
            data->nulData=NULL;
            return 0;
        }
        else if(jsonString[i]==L'{'&&controlCharInString(jsonString,i)==0){
            *cursor=(i+1);
            *dataType=_OBJECT_;
            data->nulData=NULL;
            return 0;
        }
        if((jsonString[i]==L','||jsonString[i]==L']'||jsonString[i]==L'}')&&controlCharInString(jsonString,i)==0){
            if((data->strData=readStringType(jsonString,i))!=0){
                *dataType=_STRING_;
                *cursor=i+1;
                return 1;
            }
            if((readDoubleType(jsonString,i,data))!=0){
                *dataType=_DOUBLE_;
                *cursor=i+1;
                return 1;
            }
            if((readLongType(jsonString,i,data))!=0){
                *dataType=_LONG_;
                *cursor=i+1;
                return 1;
            }
            if((readBolType(jsonString,i,data))!=0){
                *dataType=_BOOLEAN_;
                *cursor=i+1;
                return 1;
            }
            if((readNullType(jsonString,i,data))!=0){
                *dataType=_NULL_;
                *cursor=i+1;
                return 1;
            }
        }
    }
    return 0;
}

unsigned long countChildElements(wchar_t *jsonString,unsigned long cursor,JsonType dataType,unsigned long *cursorEndArray){

    unsigned long commaCounter=0;
    long tokenCounter=0;
    wchar_t searchedToken;
    wchar_t nonSearchedToken;

    if(dataType==_OBJECT_){
        searchedToken=L']';
        nonSearchedToken=L'}';
    }
    else if(dataType==_ARRAY_){
        searchedToken=L'}';
        nonSearchedToken=L']';
    }
    while(tokenCounter<1&&cursor<wcslen(jsonString)){
        if((jsonString[cursor]==searchedToken-2||jsonString[cursor]==nonSearchedToken-2)&&controlCharInString(jsonString,cursor)==0){
            tokenCounter--;
        }
        else if((jsonString[cursor]==searchedToken||jsonString[cursor]==nonSearchedToken)&&controlCharInString(jsonString,cursor)==0){
            tokenCounter++;
        }
        if(tokenCounter>=0){
            if(jsonString[cursor]==L','){
                if(controlCharInString(jsonString,cursor)==0){
                    commaCounter++;
                }
            }
            if(commaCounter==0&&jsonString[cursor]!=L'{'&&jsonString[cursor]!=L'}'&&jsonString[cursor]!=L'['&&jsonString[cursor]!=L']'){
                commaCounter++;
            }
        }
        if(tokenCounter<0&&commaCounter==0){
            commaCounter++;
        }
        cursor++;
    }
    *cursorEndArray=cursor;
    return commaCounter;
}

short controlCharInString(wchar_t *jsonString,unsigned long cursor){

    unsigned long quoteCounter=0;
    unsigned long i,j;
    for(i = cursor ; i>0; i--){
        if(jsonString[i]==L'\"'&&jsonString[i-1]!=L'\\'){
            quoteCounter++;
            if(jsonString[i-1]==L','||jsonString[i-1]==L':'||jsonString[i-1]==L'{'||jsonString[i-1]==L'['||jsonString[i-1]==L'\n'||jsonString[i-1]==L' '){
                for(j = i+1 ; j<wcslen(jsonString); j++){
                    if(jsonString[j]==L'\"'&&jsonString[j-1]!=L'\\'){
                        quoteCounter++;
                        if(j!=cursor){
                            if((jsonString[j+1]==L','||jsonString[j+1]==L':'||jsonString[j+1]==L'}'||jsonString[j+1]==L']'||jsonString[i-1]==L'\n'||jsonString[i-1]==L' ')&&quoteCounter%2==0){
                                return 1;
                            }
                            else{
                                return 0;
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

short readDoubleType(wchar_t *jsonString,unsigned long cursor,Data *data){

    wchar_t *strVar;
    short doteCounter=0;
    unsigned long strSize=0;

    if(jsonString[cursor-1]==L'\"'&&jsonString[cursor-2]!=L'\\'){
        return 0;
    }

    for(unsigned long i=cursor-1;i!=0;i--){
        strSize++;
        if(iswdigit(jsonString[i])==0&&jsonString[i]!=L'.'&&jsonString[i]!=L':'){
            return 0;
        }
        if(jsonString[i]==L'.'){
            doteCounter++;
        }
        if(doteCounter==1&&(jsonString[i-1]==L':'||jsonString[i-1]==L'['||jsonString[i-1]==L'{'||jsonString[i-1]==L' '||jsonString[i-1]==L'\n'||jsonString[i-1]==L',')){

            wcschr(jsonString+i,'.')[0]=',';
            strVar=malloc(sizeof(wchar_t)*(strSize+1));
            wcsncpy(strVar,jsonString+i,strSize);
            strVar[strSize+1]=L'\0';
            data->dblData=wcstod(strVar,NULL);
            free(strVar);
            return 1;
        }

    }
    return 0;
}

short readLongType(wchar_t *jsonString,unsigned long cursor,Data *data){

    wchar_t *strVar;
    unsigned long strSize=0;
    if(jsonString[cursor-1]==L'\"'&&jsonString[cursor-2]!=L'\\'){
        return 0;
    }

    for(unsigned long i=cursor-1;i!=0;i--){

        strSize++;
        if(iswdigit(jsonString[i])==0&&jsonString[i]!=L':'&&jsonString[i]!=L' '&&jsonString[i]!=L'\n'){
            return 0;
        }
        if(jsonString[i]==L'.'){
            return 0;
        }
        if(jsonString[i-1]==L':'||jsonString[i-1]==L'['||jsonString[i-1]==L'{'||jsonString[i-1]==L' '||jsonString[i-1]==L'\n'||jsonString[i-1]==L','){

            strVar=malloc(sizeof(wchar_t)*(strSize));

            wcsncpy(strVar,jsonString+i,strSize);
            strVar[strSize]='\0';
            data->lngData=wcstol(strVar,NULL,10);
            free(strVar);
            return 1;
        }


    }
    return 0;
}

short readBolType(wchar_t *jsonString,unsigned long cursor,Data *data){

    if(jsonString[cursor-1]==L'\"'&&jsonString[cursor-2]!=L'\\'){
        return 0;
    }
    for(unsigned long i=cursor-1;i!=0;i--){

        if(jsonString[i]=='.'||iswdigit(jsonString[i])){
            return 0;
        }
        if(jsonString[i-1]==L':'||jsonString[i-1]==L'['||jsonString[i-1]==L'{'||jsonString[i-1]==L' '||jsonString[i-1]==L'\n'||jsonString[i-1]==L','){

            if(wcsncmp(jsonString+i,L"true",wcslen(L"true"))==0){
                data->bolData=1;
                return 1;
            }
            else if(wcsncmp(jsonString+i,L"false",wcslen(L"false"))==0){
                data->bolData=0;
                return 1;
            }
        }
    }
    return 0;
}

short readNullType(wchar_t *jsonString,unsigned long cursor,Data *data){

    if(jsonString[cursor-1]==L'\"'&&jsonString[cursor-2]!=L'\\'){
        return 0;
    }
    for(unsigned long i=cursor-1;i!=0;i--){

        if(jsonString[i]=='.'||iswdigit(jsonString[i])){
            return 0;
        }
        if(jsonString[i-1]==L':'||jsonString[i-1]==L'['||jsonString[i-1]==L'{'||jsonString[i-1]==L' '||jsonString[i-1]==L'\n'||jsonString[i-1]==L','){
            if(wcsncmp(jsonString+i,L"null",wcslen(L"null"))==0){
                data->strData=malloc(sizeof(wchar_t)*(wcslen(L"null")+1));
                wcscpy(data->strData,L"null");
                return 1;
            }
        }
    }
    return 0;
}

wchar_t *readStringType(wchar_t *jsonString, unsigned long cursor){

    wchar_t *strVar;
    short quoteCounter=0;
    unsigned long strSize=0;

    if(jsonString[cursor-1]!=L'\"'){
        return NULL;
    }

    for(unsigned long i=cursor;i!=0;i--){
        if(jsonString[i]==L'\"'&&jsonString[i-1]!='\\'){
            quoteCounter++;
        }
        if(quoteCounter==2){
            strVar=malloc(sizeof(wchar_t)*(strSize-1));
            wcsncpy(strVar,jsonString+i+1,strSize-2);
            strVar[strSize-2]=L'\0';
            return strVar;
        }
        strSize++;
    }
    return NULL;
}
