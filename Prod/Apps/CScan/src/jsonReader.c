#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>
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
    //Pour chaques caractères de la chaine json vérifie si c'est un retour à la ligne ou un espace et s(il est délimité par des guillemets s'il n'est pas délimité par des guillemets il est supprimé
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
        //Si un élément json est dans un niveau de recherche inférieur à celui symbolisé par deepness alors on parcours ses éléments. Sinon on passe à l'élément suivant.
        if(countDeepness(jsonElement,0)<deepness){
            jsonElement->sizeArrChildElement=countChildElements(jsonString,*cursor,dataType,&endArray);
            jsonElement->arrChildElement=malloc(sizeof(JsonElement*)*jsonElement->sizeArrChildElement);
            for(unsigned long i=0; i<jsonElement->sizeArrChildElement; i++){
                jsonElement->arrChildElement[i]=readJsonString(jsonString,cursor,i,jsonElement,deepness);
            }
            // Si le caractère ciblé par le curseur est un délimiteur d'ouverture on n'avance pas le curseur, avancer le curseur permet de passer à la lecture d'un nouvelle élément
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
        //On estime qu'un nom ne peut pas être suivi de caractère d'ouverture ou de fermeture d'ensemble json.
        if((jsonString[i]==L'['||jsonString[i]==L']'||jsonString[i]==L'{'||jsonString[i]==L'}')&&controlCharInString(jsonString,i)==0){
            *cursor=i;
            return NULL;
        }
        //Cependant un symbole ':' permet de détecter un nom
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
        //Si un caractère d'ouverture d'ensemble json est présent et qu'il ne se trouve pas dans une chaine de caractère délimitée par des guillemets alors on type en _ARRAY_ ou _OBJECT_ en fonction du cractère
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
        //On applique succesivement les fonctions suivantes pour extraire les différents types présent dans la chaine, la fonction de lecture est en première car la majorité des données lues actuellements sont de ce type.
        if((jsonString[i]==L','||jsonString[i]==L']'||jsonString[i]==L'}')&&controlCharInString(jsonString,i)==0){
            if((data->strData=readStringType(jsonString,i))!=0){
                *dataType=_STRING_;
                *cursor=i+1;
                return 1;
            }
            if((readLongType(jsonString,i,data))!=0){
                *dataType=_LONG_;
                *cursor=i+1;
                return 1;
            }
            if((readDoubleType(jsonString,i,data))!=0){
                *dataType=_DOUBLE_;
                *cursor=i+1;
                return 1;
            }
            if((readBolType(jsonString,i,data))!=0){
                *dataType=__BOOLEAN__;
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
        //A chaque caractère de fermeture rencontré, décrémente le compteur de délimiteur de 1 et inversement lors de caractère d'ouverture. L'objectif étant de ne pas compte le nombre d'éléments présent dans les sous ensembles de l'élément ciblé par la fonction
        if((jsonString[cursor]==searchedToken-2||jsonString[cursor]==nonSearchedToken-2)&&controlCharInString(jsonString,cursor)==0){
            tokenCounter--;
        }
        else if((jsonString[cursor]==searchedToken||jsonString[cursor]==nonSearchedToken)&&controlCharInString(jsonString,cursor)==0){
            tokenCounter++;
        }
        if(tokenCounter>=0){
            //Le caractère ',' enous permet de savoir combien d'éléments sont présents dans un ensemble
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

    //Parcours la chaine de caractère en sens inverse jusqu'à une guillement non échappée
    for(i = cursor ; i>0; i--){
        if(jsonString[i]==L'\"'&&jsonString[i-1]!=L'\\'){
            quoteCounter++;
            //Vérifie la présence d'un caractère délimiteur délément json derrière une guillement
            if(jsonString[i-1]==L','||jsonString[i-1]==L':'||jsonString[i-1]==L'{'||jsonString[i-1]==L'['||jsonString[i-1]==L'\n'||jsonString[i-1]==L' '){
                //Parcours la chaine dans l'autre sens avec un décallage de 1 caractère et effectue le meme traitement dans le sens inverse en ne prenant pas en compte le caractère ciblé par le curseur d'origine
                for(j = i+1 ; j<wcslen(jsonString); j++){
                    if(jsonString[j]==L'\"'&&jsonString[j-1]!=L'\\'){
                        quoteCounter++;
                        if(j!=cursor){
                            if((jsonString[j+1]==L','||jsonString[j+1]==L':'||jsonString[j+1]==L'}'||jsonString[j+1]==L']'||jsonString[i-1]==L'\n'||jsonString[i-1]==L' ')&&quoteCounter%2==0){
                                //Est délimité par des guillemets
                                return 1;
                            }
                            else{
                                //N'est pas délimité par des guillemets
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
