#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>
#include <errno.h>
#include <locale.h>

#include <jsonHandler.h>
#include <jsonReader.h>
#include <fileHandler.h>


JsonElement *readJsonFile(FILE *jsonFile,int deepness){

    char *jsonContent;
    JsonElement *json;
    unsigned long cursor=0;
    jsonContent=getFileContent(jsonFile);
    
    printf("COUCU");
    if((formatJsonString(&jsonContent))==0){
        createErrorReport("Json string formating error : ",__FILE__,__LINE__,__DATE__,__TIME__);
    }
    // puts("TTI");
    // printf("toto");
    // puts("TTI");

    
    // printf("toto");
    fflush(stdout);

    if((json=readJsonString(jsonContent,&cursor,0,NULL,deepness))==NULL){
        createErrorReport("Json string formating error : ",__FILE__,__LINE__,__DATE__,__TIME__);
        free(jsonContent);
        return NULL;
    }

    // printf("json->name : %ld",json->sizeArrChildElement);
    // puts("TTI");

    
    fflush(stdout);

    return json;
}

short formatJsonString(char **jsonString){

    char *tmpString;
    unsigned long i;
    unsigned long j=0;
    if((tmpString=malloc(sizeof(char*)*(strlen(*jsonString)+1)))==NULL){
        createErrorReport("Memory allocation error :  ",__FILE__,__LINE__,__DATE__,__TIME__);
        return 0;
    }
    //Pour chaques caractères de la chaine json vérifie si c'est un retour à la ligne ou un espace
    //et s'il est délimité par des guillemets s'il n'est pas délimité par des guillemets il est supprimé
    for(i=0;i<strlen(*jsonString);i++){
        if((*jsonString)[i]!=' '&&(*jsonString)[i]!='\n'&&controlCharInString((*jsonString),i)==0){
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
    return 1;

}

JsonElement *readJsonString(char *jsonString,unsigned long *cursor,unsigned long elementPosition, JsonElement *parentElement, int deepness){

    JsonElement *jsonElement;
    JsonType dataType;
    unsigned long lastCursorPosition=*cursor;
    Data *data;
    char *name;
    errno=0;

    if((data=malloc(sizeof(Data)))==NULL){
        createErrorReport("Memory allocation error : ",__FILE__,__LINE__,__DATE__,__TIME__);
        return NULL;
    }
    if((name=readName(jsonString,cursor))==NULL){
        *cursor=lastCursorPosition;
        readData(jsonString,cursor,&dataType,data);
        *cursor+=1;
    }else{
        readData(jsonString,cursor,&dataType,data);
    }

    //Control code d'erreur, l'inscrit dans le journal d'erreur s'il est différent de 0 et renvoie null pour terminer les traitements.
    if(errno!=0){
        createErrorReport(strerror(errno),__FILE__,__LINE__,__DATE__,__TIME__);
        return NULL;
    }
    if((jsonElement=initJsonElement(0,elementPosition,name,data,dataType,parentElement))==NULL){
        return NULL;
    }

    if(dataType==_OBJECT_||dataType==_ARRAY_){
        unsigned long endArray=0;
        //Si un élément json est dans un niveau de recherche inférieur à celui symbolisé par l'entier deepness alors on parcours ses éléments. Sinon on passe à l'élément suivant.
        if(countDeepness(jsonElement,0)<deepness){
            jsonElement->sizeArrChildElement=countChildElements(jsonString,*cursor,dataType,&endArray);
            if((jsonElement->arrChildElement=malloc(sizeof(JsonElement*)*jsonElement->sizeArrChildElement))==NULL){
                jsonElement->sizeArrChildElement=0;
                createErrorReport("Memory allocation error : ",__FILE__,__LINE__,__DATE__,__TIME__);
                return NULL;
            }
            for(unsigned long i=0; i<jsonElement->sizeArrChildElement; i++){
                jsonElement->arrChildElement[i]=readJsonString(jsonString,cursor,i,jsonElement,deepness);
            }
            // Si le caractère ciblé par le curseur est un délimiteur d'ouverture d'object json on n'avance pas le curseur,
            // avancer le curseur permet de passer à la lecture d'un nouvelle élément
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

char *readName(char *jsonString, unsigned long *cursor){

    char *name;
    for(unsigned long i=*cursor; i<strlen(jsonString);i++){
        //On estime qu'un nom ne peut pas être suivi de caractère d'ouverture ou de fermeture d'ensemble json.
        if((jsonString[i]=='['||jsonString[i]==']'||jsonString[i]=='{'||jsonString[i]=='}')&&controlCharInString(jsonString,i)==0){
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


short readData(char *jsonString, unsigned long *cursor,JsonType *dataType,Data *data){

    for(unsigned long i=*cursor; i<strlen(jsonString);i++){
        //Si un caractère d'ouverture d'ensemble json est présent et qu'il ne se trouve pas dans une chaine de caractère délimitée par des guillemets
        //alors on le type en _ARRAY_ ou _OBJECT_ en fonction du caractère
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
        //On applique succesivement les fonctions suivantes pour extraire les différents types présent dans la chaine,
        //la fonction de lecture est en première car la majorité des données lues actuellements sont de ce type.
        if((jsonString[i]==','||jsonString[i]==']'||jsonString[i]=='}')&&controlCharInString(jsonString,i)==0){
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


unsigned long countChildElements(char *jsonString,unsigned long cursor,JsonType dataType,unsigned long *cursorEndArray){

    unsigned long commaCounter=0;
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
        //A chaque caractère de fermeture rencontré, décrémente le compteur de délimiteur de 1 et inversement lors de caractère d'ouverture. L'objectif étant de ne pas compte le nombre d'éléments présent dans les sous ensembles de l'élément ciblé par la fonction
        if((jsonString[cursor]==searchedToken-2||jsonString[cursor]==nonSearchedToken-2)&&controlCharInString(jsonString,cursor)==0){
            tokenCounter--;
        }
        else if((jsonString[cursor]==searchedToken||jsonString[cursor]==nonSearchedToken)&&controlCharInString(jsonString,cursor)==0){
            tokenCounter++;
        }
        if(tokenCounter>=0){
            //Le caractère ',' enous permet de savoir combien d'éléments sont présents dans un ensemble
            if(jsonString[cursor]==','){
                if(controlCharInString(jsonString,cursor)==0){
                    commaCounter++;
                }
            }
            if(commaCounter==0&&jsonString[cursor]!='{'&&jsonString[cursor]!='}'&&jsonString[cursor]!='['&&jsonString[cursor]!=']'){
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

short controlCharInString(char *jsonString,unsigned long cursor){

    unsigned long quoteCounter=0;
    unsigned long i,j;

    //Parcours la chaine de caractère en sens inverse jusqu'à une guillement non échappée
    for(i = cursor ; i>0; i--){
        if(jsonString[i]=='\"'&&jsonString[i-1]!='\\'){
            quoteCounter++;
            //Vérifie la présence d'un caractère délimiteur délément json derrière une guillement
            if(jsonString[i-1]==','||jsonString[i-1]==':'||jsonString[i-1]=='{'||jsonString[i-1]=='['||jsonString[i-1]=='\n'||jsonString[i-1]==' '){
                //Parcours la chaine dans l'autre sens avec un décallage de 1 caractère et effectue le meme traitement dans le sens inverse en ne prenant pas en compte le caractère ciblé par le curseur d'origine
                for(j = i+1 ; j<strlen(jsonString); j++){
                    if(jsonString[j]=='\"'&&jsonString[j-1]!='\\'){
                        quoteCounter++;
                        if(j!=cursor){
                            if((jsonString[j+1]==','||jsonString[j+1]==':'||jsonString[j+1]=='}'||jsonString[j+1]==']'||jsonString[i-1]=='\n'||jsonString[i-1]==' ')&&quoteCounter%2==0){
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

short readDoubleType(char *jsonString,unsigned long cursor,Data *data){

    char *strVar;
    short doteCounter=0;
    unsigned long strSize=0;

    if(jsonString[cursor-1]=='\"'&&jsonString[cursor-2]!='\\'){
        return 0;
    }

    for(unsigned long i=cursor-1;i!=0;i--){
        strSize++;

        if(iswdigit(jsonString[i])==0&&jsonString[i]!='.'&&jsonString[i]!=':'){
            return 0;
        }
        if(jsonString[i]=='.'){
            doteCounter++;
        }
        if(doteCounter==1&&(jsonString[i-1]==':'||jsonString[i-1]=='['||jsonString[i-1]=='{'||jsonString[i-1]==' '||jsonString[i-1]=='\n'||jsonString[i-1]==',')){

            strchr(jsonString+i,'.')[0]=',';
            if((strVar=malloc(sizeof(char)*(strSize+1)))!=NULL){
                strncpy(strVar,jsonString+i,strSize);
                strVar[strSize+1]='\0';
                data->dblData=atof(strVar);
                free(strVar);
            }
            return 1;
        }

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

        strSize++;
        if(iswdigit(jsonString[i])==0&&jsonString[i]!=':'&&jsonString[i]!=' '&&jsonString[i]!='\n'){
            return 0;
        }
        if(jsonString[i]=='.'){
            return 0;
        }
        if(jsonString[i-1]==':'||jsonString[i-1]=='['||jsonString[i-1]=='{'||jsonString[i-1]==' '||jsonString[i-1]=='\n'||jsonString[i-1]==','){

            if((strVar=malloc(sizeof(char)*(strSize)))!=NULL){
                strncpy(strVar,jsonString+i,strSize);
                strVar[strSize]='\0';
                data->lngData=atol(strVar);
                free(strVar);
            }
            return 1;
        }
    }
    return 0;
}

short readBolType(char *jsonString,unsigned long cursor,Data *data){

    if(jsonString[cursor-1]=='\"'&&jsonString[cursor-2]!='\\'){
        return 0;
    }
    for(unsigned long i=cursor-1;i!=0;i--){

        if(jsonString[i]=='.'||iswdigit(jsonString[i])){
            return 0;
        }
        if(jsonString[i-1]==':'||jsonString[i-1]=='['||jsonString[i-1]=='{'||jsonString[i-1]==' '||jsonString[i-1]=='\n'||jsonString[i-1]==','){

            if(strncmp(jsonString+i,"true",strlen("true"))==0){
                data->bolData=1;
                return 1;
            }
            else if(strncmp(jsonString+i,"false",strlen("false"))==0){
                data->bolData=0;
                return 1;
            }
        }
    }
    return 0;
}

short readNullType(char *jsonString,unsigned long cursor,Data *data){

    if(jsonString[cursor-1]=='\"'&&jsonString[cursor-2]!='\\'){
        return 0;
    }
    for(unsigned long i=cursor-1;i!=0;i--){

        if(jsonString[i]=='.'||iswdigit(jsonString[i])){
            return 0;
        }
        if(jsonString[i-1]==':'||jsonString[i-1]=='['||jsonString[i-1]=='{'||jsonString[i-1]==' '||jsonString[i-1]=='\n'||jsonString[i-1]==','){
            if(strncmp(jsonString+i,"null",strlen("null"))==0){
                if((data->strData=malloc(sizeof(char)*(strlen("null")+1)))!=NULL){
                    strcpy(data->strData,"null");
                }
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
        if(jsonString[i]=='\"'&&jsonString[i-1]!='\\'){
            quoteCounter++;
        }
        if(quoteCounter==2){
            if((strVar=malloc(sizeof(char)*(strSize-1)))!=NULL){
                strncpy(strVar,jsonString+i+1,strSize-2);
                strVar[strSize-2]='\0';
            }
            return strVar;
        }
        strSize++;
    }
    return NULL;
}
