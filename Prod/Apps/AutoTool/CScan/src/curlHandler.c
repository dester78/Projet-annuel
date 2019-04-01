#include <stdio.h>
#include<string.h>
#include <curl/curl.h>
#include <wchar.h>
#include <stdlib.h>
#include <locale.h>

#include <curlHandler.h>
#include <jsonHandler.h>
#include <jsonReader.h>
#include <jsonWriter.h>
#include <fileHandler.h>
#include <convertCharHandler.h>



CurlRequest *initCurlRequest(){

    CurlRequest *curlRequest;

    curlRequest=malloc(sizeof(CurlRequest));
    curlRequest->method=NULL;
    curlRequest->body=NULL;
    curlRequest->url=NULL;
    curlRequest->deepness=0;
    curlRequest->directSearchedValues=NULL;
    curlRequest->sizeArrSearchedValues=0;

    curlRequest->response=malloc(sizeof(CurlResponse));
    curlRequest->response->size=0;
    curlRequest->response->string=malloc(sizeof(char));
    curlRequest->response->string[0]='\0';
    curlRequest->response->file=NULL;

    return curlRequest;

}

short freeCurlRequest(CurlRequest **curlRequest){


    if((*curlRequest)->method!=NULL){
        free((*curlRequest)->method);
        (*curlRequest)->method=NULL;
    }
    if((*curlRequest)->body!=NULL){
        free((*curlRequest)->body);
        (*curlRequest)->body=NULL;
    }
    if((*curlRequest)->url!=NULL){
        free((*curlRequest)->url);
        (*curlRequest)->url=NULL;
    }
    if((*curlRequest)->response!=NULL){
        freeCurlResponse(&(*curlRequest)->response);
    }
    free(*curlRequest);
    (*curlRequest)=NULL;

    return 1;
}

short freeCurlResponse(CurlResponse **curlResponse){


    if((*curlResponse)->string!=NULL){
        free((*curlResponse)->string);
        (*curlResponse)->string=NULL;
    }
    if((*curlResponse)->file!=NULL){
        freeFileElement((*curlResponse)->file);
        (*curlResponse)->file=NULL;
    }
    if((*curlResponse)->json!=NULL){
        freeJsonElement(&(*curlResponse)->json);
    }

    free(*curlResponse);
    *curlResponse=NULL;
    return 1;
}





CurlRequest *jsonToCurl(JsonElement *jsonElement,char *urlName,FileIndex *fileIndex){

    CurlRequest *curl;
    Data data;
    JsonElement *jsonCurlElement;
    JsonElement *jsonParameterElement;
    JsonElement *tmpJsonElement;
    unsigned int elementPosition;
    unsigned int parameterPosition;

    curl=initCurlRequest();

    // printf("WAIT");
    data.strData=urlName;

    if((jsonCurlElement=searchJsonElement(jsonElement,"curl",NULL,_ARRAY_,&elementPosition))!=NULL){
        if((jsonCurlElement=searchJsonElement(jsonCurlElement,"name",&data,_STRING_,&elementPosition))!=NULL){

            if((jsonParameterElement=searchJsonElement(jsonElement,"urlNeededValues",NULL,_ARRAY_,&parameterPosition))){
                curl->url=writeUrl(jsonCurlElement,jsonParameterElement->arrChildElement[parameterPosition]);
            }
            if((tmpJsonElement=searchJsonElement(jsonElement,"file",NULL,_STRING_,&parameterPosition))!=NULL){
                curl->response->file=selectFile(fileIndex,tmpJsonElement->arrChildElement[elementPosition]->data->strData);
                curl->responseType=_FILE_;
            }
            if((tmpJsonElement=searchJsonElement(jsonCurlElement,"method",NULL,_STRING_,&elementPosition))!=NULL){
                curl->method=malloc(sizeof(char)*(strlen(tmpJsonElement->arrChildElement[elementPosition]->data->strData)+1));
                strcpy(curl->method,tmpJsonElement->arrChildElement[elementPosition]->data->strData);
            }
            if((tmpJsonElement=searchJsonElement(jsonCurlElement,"deepness",NULL,_LONG_,&elementPosition))!=NULL){
                curl->deepness=(int)tmpJsonElement->arrChildElement[elementPosition]->data->lngData;
            }
            if((tmpJsonElement=searchJsonElement(jsonCurlElement,"json",NULL,_LONG_,&elementPosition))!=NULL){
                curl->responseType=_JSON_;
            }
            if((tmpJsonElement=searchJsonElement(jsonCurlElement,"directSearchedValues",NULL,_ARRAY_,&elementPosition))!=NULL&&tmpJsonElement->arrChildElement[elementPosition]->sizeArrChildElement!=0){
                curl->sizeArrSearchedValues=(int)tmpJsonElement->arrChildElement[elementPosition]->sizeArrChildElement;
                curl->directSearchedValues=malloc(sizeof(char*)*curl->sizeArrSearchedValues);
                for(int i=0;i<curl->sizeArrSearchedValues;i++){
                    curl->directSearchedValues[i]=malloc(sizeof(char)*(strlen(tmpJsonElement->arrChildElement[elementPosition]->arrChildElement[i]->data->strData)+1));
                    strcpy(curl->directSearchedValues[i],tmpJsonElement->arrChildElement[elementPosition]->arrChildElement[i]->data->strData);
                }
            }
            if((tmpJsonElement=searchJsonElement(jsonCurlElement,"body",NULL,_OBJECT_,&elementPosition))!=NULL){
                curl->body=malloc(sizeof(char));
                curl->body[0]='\0';
                writeAgreggatedValue(tmpJsonElement->arrChildElement[elementPosition],&curl->body,1000);
            }
        }
    }

    printf("URL : %s",curl->url);
    return curl;
}

char *writeUrl(JsonElement *jsonElement,  JsonElement *urlNeededValues){

    char *url;
    JsonElement *tmpElement;
    unsigned int elementPosition;
    
    url=malloc(sizeof(char));
    url[0]='\0';

    for(unsigned long i=0;i<urlNeededValues->sizeArrChildElement;i++){
        if((tmpElement=searchJsonElement(jsonElement,urlNeededValues->arrChildElement[i]->arrChildElement[0]->name,NULL,_ARRAY_,&elementPosition))!=NULL){
            catTokenWithUrl(urlNeededValues->arrChildElement[i]->arrChildElement[0],tmpElement->arrChildElement[elementPosition],&url);
        }
    }
    return url;
}

short catTokenWithUrl(JsonElement *urlToken, JsonElement *urlElement,char **url){

    for(unsigned long i=0;i<urlElement->sizeArrChildElement;i++){
        if(urlElement->arrChildElement[i]->arrChildElement==NULL){
            if(strlen(urlElement->arrChildElement[i]->data->strData)>0){
                if(strlen(urlToken->parentElement->arrChildElement[i]->arrChildElement[0]->data->strData)>0){
                    *url=realloc(*url,sizeof(char)*(strlen(*url)+strlen(urlToken->parentElement->arrChildElement[i]->arrChildElement[0]->data->strData)+1));
                    strcat(*url,urlToken->parentElement->arrChildElement[i]->arrChildElement[0]->data->strData);
                }
                *url=realloc(*url,sizeof(char)*(strlen(*url)+strlen(urlElement->arrChildElement[i]->data->strData)+1));
                strcat(*url,urlElement->arrChildElement[i]->data->strData);
                if(strlen(urlToken->parentElement->arrChildElement[i]->arrChildElement[1]->data->strData)>0){
                    *url=realloc(*url,sizeof(char)*(strlen(*url)+strlen(urlToken->parentElement->arrChildElement[i]->arrChildElement[1]->data->strData)+1));
                    strcat(*url,urlToken->parentElement->arrChildElement[i]->arrChildElement[1]->data->strData);
                }
            }
        }
        else{
            catTokenWithUrl(urlToken->arrChildElement[0],urlElement->arrChildElement[i],url);
        }
    }
    return 0;
}



short curlConnect(CurlRequest **curlRequest){


    CURL *curl = curl_easy_init();
    struct curl_slist *headers = NULL;

    CURLcode result;

    curlSetOpt(curlRequest, curl, headers);

    if((result = curl_easy_perform(curl)) != CURLE_OK){
        fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(result));
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();


    if((*curlRequest)->responseType==_JSON_){
        curlGetJson(curlRequest);
    }

    if((*curlRequest)->responseType==_FILE_){
        fclose((*curlRequest)->response->file->filePointer);
//        system("setup.exe");
    }

    if((result = curl_easy_perform(curl)) != CURLE_OK){
        fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(result));

    }


//    wprintf("CHAR END : %ls",tmpWChar);

    return 0;
}



size_t curlToString(void *ptr, size_t size, size_t nmemb, CurlResponse *curlResponse){


    size_t newSize=curlResponse->size+size*nmemb;
    curlResponse->string=realloc(curlResponse->string,newSize+1);
    memcpy(curlResponse->string+curlResponse->size,ptr,size*nmemb);
    curlResponse->string[newSize]='\0';
    curlResponse->size=newSize;
    return size*nmemb;

}

size_t curlToFile(void *ptr, size_t size, size_t nmemb, FILE *curlResponse) {
    size_t newSize=fwrite(ptr, size, nmemb, curlResponse);
    return newSize;
}


short curlGetJson(CurlRequest **curlRequest){

    char *tmpChar;
    unsigned long cursor=0;


    if((*curlRequest)->directSearchedValues!=NULL){
        for(int i = 0; i < (*curlRequest)->sizeArrSearchedValues ; i++){
            if(strpbrk((*curlRequest)->response->string,(*curlRequest)->directSearchedValues[i])!=NULL){
                // printf("OK CURL FIND");
                return 1;
            }
        }
    }


    // wprintf("RESULT !!! :%ls",tmpWChar);
    (*curlRequest)->response->json=readJsonString((*curlRequest)->response->string,&cursor,0,NULL,(*curlRequest)->deepness);
//    wprintf("Result : %ls",tmpWChar);
    free((*curlRequest)->response->string);
    (*curlRequest)->response->string=malloc(sizeof(char));
    (*curlRequest)->response->string[0]='\0';
    writeJsonElement((*curlRequest)->response->json,&(*curlRequest)->response->string,1000);
    printf("REPONSE%s",(*curlRequest)->response->string);

    return 1;

}


short curlSetOpt(CurlRequest **curlRequest, CURL *curl, struct curl_slist *headers){


    //Requête HTTP méthode GET
    if((*curlRequest)->method!=NULL){
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, (*curlRequest)->method);
    }
    //URL de la requête
    curl_easy_setopt(curl, CURLOPT_URL, (*curlRequest)->url);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    //Ne vérifie pas l'authenticité du certificat envoyé par le serveur quand la valeur est à 0, vérifie quand la valeur est à 1.
    //J'ai mis la valeur à 0 car *curl stopait la connexion avec l'API openfoodfacts lorsqu'une vérification était effectuée.
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);

    //Création d'un header vide
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    if((*curlRequest)->responseType==_JSON_){
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlToString);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (*curlRequest)->response);
    }
    else if((*curlRequest)->responseType==_FILE_){
        (*curlRequest)->response->file->filePointer=openFile((*curlRequest)->response->file->path,(*curlRequest)->response->file->openMode);
        curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "deflate");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlToFile);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (*curlRequest)->response->file->filePointer);
    }

    return 1;
}


