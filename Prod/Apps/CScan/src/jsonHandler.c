
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <fileHandler.h>
#include <jsonHandler.h>
#include <errno.h>


// short fileFileIndex(FILE *srcFile, File ***fileIndex){

//     int fileCounter=0;
//     char fileName[BUFFER_SIZE];
//     char path[BUFFER_SIZE];
//     char openMode[BUFFER_SIZE];

//     while(feof(srcFile)!=EOF){
//         if(fscanf(srcFile,"\"name\":\"%s\",\"path\":\"%s\",\"openMode\":\"%s\"",&fileName,&path,&openMode)){
//             (*fileIndex)[fileCounter]=malloc(sizeof(File));
//             (*fileIndex)[fileCounter]=initFileElement(fileName,path,openMode);
//             fileCounter++;
//         }
//     }
// }



double *dblToPtr(double dblVar){
    
    double *dblP;
    return dblP=&dblVar;
}



short isNumber(char charVar){

    return charVar<48||charVar>57?0:1;
}

JsonElement *initJsonElement(unsigned long sizeArrChildElement, unsigned long elementPosition, char *name, void *data, JsonType dataType, JsonElement *parentElement){

    JsonElement *jsonElement;
    jsonElement=malloc(sizeof(JsonElement));
    jsonElement->name=malloc(sizeof(char)*(strlen(name)+1));
    strcpy(jsonElement->name,name);
    jsonElement->sizeArrChildElement=sizeArrChildElement;
    jsonElement->elementPosition=elementPosition;
    jsonElement->jsonType=dataType;
    jsonElement->parentElement=parentElement;
    jsonElement->data=defineJsonData(data,jsonElement->jsonType);

    if(jsonElement->sizeArrChildElement>0){
        jsonElement->arrChildElement=malloc(sizeof(JsonElement*)*jsonElement->sizeArrChildElement);
    }

    return jsonElement;
}

Data defineJsonData(void *inputData, JsonType dataType){

    Data outputData;
    switch(dataType){
        case _LONG_:
            outputData.lngData=(long)inputData;
        break;
        case _DOUBLE_:
            outputData.dblData=*(double*)inputData;
        break;
        case _STRING_:      
            outputData.strData=(char*)inputData;
        break;
        case _BOOLEAN_:
            outputData.bolData=(short)inputData;
        break;
        case _OBJECT_:
            outputData.nulData=NULL;
        break;
        case _ARRAY_:
            outputData.nulData=NULL;
        break;
    }
    return outputData;

}

JsonElement *readJsonFile(FILE *jsonFile){

    char *jsonContent;
    unsigned long cursor=0;
    jsonContent=getFileContent(jsonFile);
    // printf("%s",jsonContent);
    
    return readJsonString(jsonContent,&cursor,0,NULL);
}


JsonElement *readJsonString(char *jsonString,unsigned long *cursor,unsigned long elementPosition, JsonElement *parentElement){

    JsonElement *jsonElement;
    JsonType dataType;
    unsigned long lastCursorPosition=*cursor;
    char *name=readName(jsonString,cursor);
    printf("%s\n",name);
    void *data=readData(jsonString,cursor,&dataType);

    jsonElement=initJsonElement(0,elementPosition,name,data,dataType,parentElement);

    if(dataType==_OBJECT_||dataType==_ARRAY_){
        
        // printf("%s\n",jsonString+cursor);
        jsonElement->sizeArrChildElement=countChildElements(jsonString,cursor,dataType);
        for(unsigned long i=0; i<jsonElement->sizeArrChildElement; i++){
            printf("i : %d",i);
            jsonElement->arrChildElement[i]=readJsonString(jsonString,&cursor,i,jsonElement);
            printf("APRES");
        }
        *cursor=lastCursorPosition;
    }
    
    
    return jsonElement;

}

char *readName(char *jsonString, unsigned long *cursor){

    char *name;
    for(unsigned long i=*cursor; i<strlen(jsonString);i++){
        if(jsonString[i]==':'&&controlCharInString(jsonString,i)==0){
            name=readStringType(jsonString,i);
            *cursor=i;
            return name;
        }
    }
    return NULL;
}

void *readData(char *jsonString, unsigned long *cursor,JsonType *dataType){

    void *data;

    for(unsigned long i=*cursor; i<strlen(jsonString);i++){

        if(jsonString[i]=='['){
            *cursor=(i+1);
            *dataType=_ARRAY_;
            return NULL;
        }
        else if(jsonString[i]=='{'){
            *cursor=(i+1);
            *dataType=_OBJECT_;
            return NULL;
        }

        else if((jsonString[i]==','||jsonString[i]==']'||jsonString[i]=='}')&&controlCharInString(jsonString,*cursor)==0){
            if((data=readStringType(jsonString,i))!=NULL){
                *dataType=_STRING_;
                *cursor=i;
                return data;
            }
            if((data=readDoubleType(jsonString,i))!=NULL){
                *dataType=_DOUBLE_;
                *cursor=i;
                return data;
            }
            if((data=readLongType(jsonString,i))!=NULL){
                *dataType=_LONG_;
                *cursor=i;
                return data;
            }
            if((data=readBolType(jsonString,i))!=NULL){
                *dataType=_BOOLEAN_;
                *cursor=i;
                return data;
            }
        }
        
    }
    return NULL;
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
        
        
            printf("cursor :%d\n",cursor);
            // printf("%s\n",jsonString+cursor);
            if((jsonString[cursor]==searchedToken-2||jsonString[cursor]==nonSearchedToken-2)&&controlCharInString(jsonString,cursor)==0){
                
                printf("%s\n",jsonString+cursor);
                tokenCounter--;
                printf("tokenCounter : %d\n",tokenCounter);

            }
            else if((jsonString[cursor]==searchedToken||jsonString[cursor]==nonSearchedToken)&&controlCharInString(jsonString,cursor)==0){
                tokenCounter++;
                printf("tokenCounter : %d\n",tokenCounter);

            }
            if(tokenCounter>=0){
                if(jsonString[cursor]==','&&controlCharInString(jsonString,cursor)==0){
                    printf("%s\n",jsonString+cursor);
                    printf("commaCounter : %d\n",commaCounter);
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

                for(unsigned long j = cursor ; j<strlen(jsonString); j++){
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

double *readDoubleType(char *jsonString,unsigned long cursor){

    char *strVar;
    double dblVar;
    short doteCounter=0;
    unsigned long strSize=0;

    if(jsonString[cursor-1]=='\"'&&jsonString[cursor-2]!='\\'){
        return NULL;
    }

    for(unsigned long i=cursor-1;i!=0;i--){
        if(isNumber(jsonString[i])==0&&jsonString[i]!='.'&&jsonString[i]!=':'){
            return NULL;
        }
        if(jsonString[i]=='.'){
            doteCounter++;
        }
        if(doteCounter==1&&jsonString[i]==':'){
            strVar=malloc(sizeof(char)*(strSize-1));
            strncpy(strVar,jsonString+i+1,strSize);
            strVar[strSize]='\0';
            dblVar=atof(strVar);
            free(strVar);
            return dblToPtr(dblVar);
        }
        strSize++;  
    }
    return NULL;
}

long *readLongType(char *jsonString,unsigned long cursor){

    char *strVar;
    long lngVar;
    long *lngPtr;
    unsigned long strSize=0;

    if(jsonString[cursor-1]=='\"'&&jsonString[cursor-2]!='\\'){
        return NULL;
    }

    for(unsigned long i=cursor-1;i!=0;i--){

        if(isNumber(jsonString[i])==0&&jsonString[i]!=':'){
            return NULL;
        }
        if(jsonString[i]=='.'){
            return NULL;
        }
        if(jsonString[i]==':'){
            strVar=malloc(sizeof(char)*(strSize-1));
            strncpy(strVar,jsonString+i+1,strSize-1);
            strVar[strSize-1]='\0';
            lngVar=atol(strVar);
            lngPtr=&lngVar;
            free(strVar);
            return lngPtr;
        }
        strSize++;  
    }
    return NULL;
}

short *readBolType(char *jsonString,unsigned long cursor){

    short shtVar;
    short *shtPtr;
    if(jsonString[cursor-1]=='\"'&&jsonString[cursor-2]!='\\'){
        return NULL;
    }
    for(unsigned long i=cursor-1;i!=0;i--){

        if(jsonString[i]=='.'||isNumber(jsonString[i])){
            return NULL;
        }
        if(jsonString[i]==':'){

            if(strcmp(jsonString+i,"true")==0){
                shtVar=1;
            }
            else if(strcmp(jsonString+i,"false")==0){
                shtVar=0;
            }
            shtPtr=&shtVar;
            return shtPtr;
        } 
    }
    return NULL;
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
            sprintf(dataBuffer,"%d",jsonElement->data.lngData);
        break;

        case _DOUBLE_:
            sprintf(dataBuffer,"%f",jsonElement->data.dblData);
        break;

        case _STRING_:      
            sprintf(dataBuffer,"\"%s\"",jsonElement->data.strData);
        break;

        case _BOOLEAN_:
            if(jsonElement->data.bolData==0){
                sprintf(dataBuffer,"false");
            }
            else if(jsonElement->data.bolData){
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
// short writeJsonInteger(JsonElement **JsonElement, )

short freeJsonElement(JsonElement *jsonElement){

    if(jsonElement->sizeArrChildElement>0){
        for(unsigned long i=0;i<jsonElement->sizeArrChildElement;i++){
            freeJsonElement(jsonElement->arrChildElement[i]);
            free(jsonElement->arrChildElement[i]);
        }
    }
    if(jsonElement->jsonType==_STRING_){
        free(jsonElement->data.strData);
    }
    return 1;
}
