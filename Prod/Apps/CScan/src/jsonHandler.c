
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <system.h>

#include <fileHandler.h>
#include <jsonHandler.h>
#include <errno.h>
#include <wchar.h>


double *dblToPtr(double dblVar){
    double *dblP;
    return dblP=&dblVar;
}
short isNumber(char charVar){
    return charVar<48||charVar>57?0:1;
}

JsonElement *initJsonElement(unsigned long sizeArrChildElement, unsigned long elementPosition, char *name, Data *data, JsonType dataType, JsonElement *parentElement){

    JsonElement *jsonElement;
    jsonElement=malloc(sizeof(JsonElement));
    jsonElement->name=name;
    jsonElement->sizeArrChildElement=sizeArrChildElement;
    jsonElement->elementPosition=elementPosition;
    jsonElement->jsonType=dataType;
    jsonElement->parentElement=parentElement;
    jsonElement->data=data;
    if(jsonElement->sizeArrChildElement>0){
        jsonElement->arrChildElement=malloc(sizeof(JsonElement*)*jsonElement->sizeArrChildElement);
    }
    return jsonElement;
}

short formatJsonString(char **jsonString){

    char *tmpString;
    unsigned long i;
    unsigned long j=0;
    tmpString=malloc(sizeof(char*)*(strlen(*jsonString)+1));
    for(i=0;i<strlen(*jsonString);i++){
        if(((*jsonString)[i]!=' '&&(*jsonString)[i]!='\n')&&controlCharInString(*jsonString,i)==0){
            tmpString[j]=(*jsonString)[i];
            j++;
        }
    }
    tmpString[j]='\0';
    free(*jsonString);
    *jsonString=tmpString;
    return 0;

}

// Data defineJsonData(void *inputData, JsonType dataType){

//     Data outputData;
//     switch(dataType){
//         case _LONG_:
//             outputData.lngData=(long)inputData;
//         break;
//         case _DOUBLE_:
//             outputData.dblData=*(double*)inputData;
//         break;
//         case _STRING_:      
//             outputData.strData=(char*)inputData;
//         break;
//         case _BOOLEAN_:
//             printf("BOOL ptr : %p\n",inputData);
//             printf("BOOLFINAL : %c\n",(char*)(inputData));

//             outputData.bolData=*(char*)(inputData);
//         break;
//         case _OBJECT_:
//             outputData.nulData=NULL;
//         break;
//         case _ARRAY_:
//             outputData.nulData=NULL;
//         break;
//         default:
//         break;
//     }
//     return outputData;

// }

JsonElement *readJsonFile(FILE *jsonFile){

    char *jsonContent;
    unsigned long cursor=0;
    jsonContent=getFileContent(jsonFile);
    printf("JSON STRING START : %s",jsonContent);
    formatJsonString(&jsonContent);

    return readJsonString(jsonContent,&cursor,0,NULL);
}


JsonElement *readJsonString(char *jsonString,unsigned long *cursor,unsigned long elementPosition, JsonElement *parentElement){

    JsonElement *jsonElement;
    JsonType dataType;
    unsigned long lastCursorPosition=*cursor;
    Data *data;
    char *name;

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
        lastCursorPosition=*cursor;
        jsonElement->sizeArrChildElement=countChildElements(jsonString,*cursor,dataType);
        jsonElement->arrChildElement=malloc(sizeof(JsonElement*)*jsonElement->sizeArrChildElement);
        for(unsigned long i=0; i<jsonElement->sizeArrChildElement; i++){
            jsonElement->arrChildElement[i]=readJsonString(jsonString,cursor,i,jsonElement);
        }
        *cursor+=2;
    }
    return jsonElement;
}

char *readName(char *jsonString, unsigned long *cursor){

    char *name;
    for(unsigned long i=*cursor; i<strlen(jsonString);i++){

            
        if((jsonString[i]=='['||jsonString[i]==']'||jsonString[i]=='{'||jsonString[i]=='}')&&controlCharInString(jsonString,i)==0){
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

short readData(char *jsonString, unsigned long *cursor,JsonType *dataType,Data *data){

    for(unsigned long i=*cursor; i<strlen(jsonString);i++){

        if(jsonString[i]=='['&&controlCharInString(jsonString,i)==0){
            *cursor=(i+1);
            *dataType=_ARRAY_;
            data->nulData=NULL;
            return 0;
        }
        else if(jsonString[i]=='{'&&controlCharInString(jsonString,i)==0){
            *cursor=(i+1);
            *dataType=_OBJECT_;
            data->nulData=NULL;
            return 0;
        }
        if((jsonString[i]==','||jsonString[i]==']'||jsonString[i]=='}')&&controlCharInString(jsonString,i)==0){
            if((data->strData=readStringType(jsonString,i))!=0){
                *dataType=_STRING_;
                *cursor=i;
                return 1;
            }
            if((readDoubleType(jsonString,i,data))!=0){
                *dataType=_DOUBLE_;
                *cursor=i;
                return 1;
            }
            if((readLongType(jsonString,i,data))!=0){
                *dataType=_LONG_;
                *cursor=i;
                return 1;
            }
            if((readBolType(jsonString,i,data))!=0){
                *dataType=_BOOLEAN_;
                *cursor=i;
                return 1;
            }
        }
    }
    return 0;
}

unsigned long countChildElements(char *jsonString,unsigned long cursor,JsonType dataType){

    unsigned long commaCounter=1;
    long tokenCounter=0;
    char searchedToken;
    char nonSearchedToken;

    if(dataType==_OBJECT_){
        searchedToken=']';
        nonSearchedToken='}';
    }
    else if(dataType==_ARRAY_){
        searchedToken='}';
        nonSearchedToken=']';
    }
    while(tokenCounter<1&&cursor<strlen(jsonString)){
        if((jsonString[cursor]==searchedToken-2||jsonString[cursor]==nonSearchedToken-2)&&controlCharInString(jsonString,cursor)==0){
            tokenCounter--;
        }
        else if((jsonString[cursor]==searchedToken||jsonString[cursor]==nonSearchedToken)&&controlCharInString(jsonString,cursor)==0){
            tokenCounter++;
        }
        if(tokenCounter>=0){
            if(jsonString[cursor]==','&&controlCharInString(jsonString,cursor)==0){
                commaCounter++;
            }
        }
        cursor++;
    }
    return commaCounter;
}

short controlCharInString(char *jsonString,unsigned long cursor){

    for(unsigned long i = cursor ; i>0; i--){

        if(jsonString[i]=='\"'&&jsonString[i-1]!='\\'){
            if(jsonString[i-1]==','||jsonString[i-1]==':'||jsonString[i-1]=='{'||jsonString[i-1]=='['){
                for(unsigned long j = i ; j<strlen(jsonString); j++){
                    if(jsonString[j]=='\"'&&jsonString[j-1]!='\\'){                      
                        if(jsonString[j+1]==','||jsonString[j+1]==':'||jsonString[j+1]=='}'||jsonString[j+1]==']'){
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
    return 0;
}

short readDoubleType(char *jsonString,unsigned long cursor,Data *data){

    char *strVar;
    short doteCounter=0;
    unsigned long strSize=0;

    if(jsonString[cursor-1]=='\"'&&jsonString[cursor-2]!='\\'){
        return 0;
    }

    for(unsigned long i=cursor-1;i!=0;i--){
        if(isNumber(jsonString[i])==0&&jsonString[i]!='.'&&jsonString[i]!=':'){
            return 0;
        }
        if(jsonString[i]=='.'){
            doteCounter++;
        }
        if(doteCounter==1&&jsonString[i]==':'){
            strVar=malloc(sizeof(char)*(strSize+1));
            strncpy(strVar,jsonString+i+1,strSize);
            strVar[strSize]='\0';
            data->dblData=atof(strVar);
            free(strVar);
            return 1;
        }
        strSize++;  
    }
    return 0;
}

short readLongType(char *jsonString,unsigned long cursor,Data *data){

    char *strVar;
    unsigned long strSize=0;

    if(jsonString[cursor-1]=='\"'&&jsonString[cursor-2]!='\\'){
        return 0;
    }

    for(unsigned long i=cursor-1;i!=0;i--){

        if(isNumber(jsonString[i])==0&&jsonString[i]!=':'){
            return 0;
        }
        if(jsonString[i]=='.'){
            return 0;
        }
        if(jsonString[i]==':'){
            strVar=malloc(sizeof(char)*(strSize));
            strncpy(strVar,jsonString+i+1,strSize);
            strVar[strSize]='\0';
            data->lngData=atol(strVar);
            free(strVar);
            return 1;
        }
        strSize++;  
    }
    return 0;
}

short readBolType(char *jsonString,unsigned long cursor,Data *data){

    if(jsonString[cursor-1]=='\"'&&jsonString[cursor-2]!='\\'){
        return 0;
    }
    for(unsigned long i=cursor-1;i!=0;i--){

        if(jsonString[i]=='.'||isNumber(jsonString[i])){
            return 0;
        }
        if(jsonString[i]==':'||jsonString[i]==','){
            if(strncmp(jsonString+i+1,"true",strlen("true"))==0){
                data->bolData=1;
                return 1;
            }
            else if(strncmp(jsonString+i+1,"false",strlen("false"))==0){
                data->bolData=0;
                return 1;
            }
        } 
    }
    return 0;
}

char *readStringType(char *jsonString, unsigned long cursor){

    char *strVar;
    short quoteCounter=0;
    unsigned long strSize=0;

    if(jsonString[cursor-1]!='\"'){
        return NULL;
    }
    
    for(unsigned long i=cursor;i!=0;i--){
        if(jsonString[i]=='\"'){
            quoteCounter++;
        }
        if(quoteCounter==2){
            strVar=malloc(sizeof(char)*(strSize-1));
            strncpy(strVar,jsonString+i+1,strSize-2);
            strVar[strSize-2]='\0';
            return strVar;
        }
        strSize++;  
    }
    return NULL;
}

short writeJsonElement(JsonElement *jsonElement, char **jsonString, unsigned long bufferSize){

    writeJsonName(jsonElement,jsonString,bufferSize);
    writeJsonData(jsonElement,jsonString,bufferSize);

    if(jsonElement->parentElement!=NULL){
        if(jsonElement->parentElement->sizeArrChildElement-1!=jsonElement->elementPosition){
            *jsonString=realloc(*jsonString,sizeof(char)*(strlen(*jsonString)+2));
            strcat(*jsonString,",");
        }
    }
    return 1;
}

short writeJsonData(JsonElement *jsonElement, char **jsonString,unsigned long bufferSize){

    char *dataBuffer;
    dataBuffer=malloc(sizeof(char)*bufferSize);

    switch(jsonElement->jsonType){
        case _LONG_:
            sprintf(dataBuffer,"%ld",jsonElement->data->lngData);
        break;

        case _DOUBLE_:
            sprintf(dataBuffer,"%lf",jsonElement->data->dblData);
        break;

        case _STRING_:      
            sprintf(dataBuffer,"\"%s\"",jsonElement->data->strData);
        break;

        case _BOOLEAN_:
            if(jsonElement->data->bolData==0){
                sprintf(dataBuffer,"false");
            }
            else if(jsonElement->data->bolData){
                sprintf(dataBuffer,"true");
            }
        break;
        case _OBJECT_:
            writeAgreggatedValue(jsonElement,jsonString,bufferSize);
            return 1;
        break;
        case _ARRAY_:
            writeAgreggatedValue(jsonElement,jsonString,bufferSize);
            return 1;
        break;
    }

    *jsonString=realloc(*jsonString, (strlen(*jsonString) + strlen(dataBuffer)+1) * sizeof(char));
    strcat(*jsonString,dataBuffer);
    free(dataBuffer);
    
    return 1;
}


short writeJsonName(JsonElement *jsonElement, char **jsonString, unsigned long bufferSize){

    char *nameBuffer;

    nameBuffer=malloc(sizeof(char)*bufferSize);

    if(jsonElement->name!=NULL){

        sprintf(nameBuffer,"\"%s\":",jsonElement->name);
        *jsonString=realloc(*jsonString, (strlen(*jsonString) + strlen(nameBuffer)+1) * sizeof(char));
        strcat(*jsonString,nameBuffer);
    }

    free(nameBuffer);
    return 1;
}

short writeAgreggatedValue(JsonElement *jsonElement, char **jsonString, unsigned long bufferSize){
    char token[2];

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
    for(unsigned long i=0;i<jsonElement->sizeArrChildElement;i++){
        writeJsonElement(jsonElement->arrChildElement[i],jsonString,bufferSize);
        
    }
    *jsonString=realloc(*jsonString, (strlen(*jsonString) + strlen(token)+2) * sizeof(char));
    token[0]+=2;  
    strcat(*jsonString,token);

    return 1;

}

short freeJsonElement(JsonElement *jsonElement){

    if(jsonElement->sizeArrChildElement>0){
        for(unsigned long i=0;i<jsonElement->sizeArrChildElement;i++){
            freeJsonElement(jsonElement->arrChildElement[i]);
            free(jsonElement->arrChildElement[i]);
        }
    }
    if(jsonElement->jsonType==_STRING_){
        free(jsonElement->data->strData);
    }
    free(jsonElement->data);
    return 1;
}


