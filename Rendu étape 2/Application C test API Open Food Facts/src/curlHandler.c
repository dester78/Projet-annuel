#include <stdio.h>
#include <curl.h>
#include <wchar.h>
#include <locale.h>

#include <curlHandler.h>
#include <jsonHandler.h>
#include <jsonReader.h>
#include <jsonWriter.h>
#include <fileHandler.h>
#include <wCharHandler.h>


CurlRequest *initCurlRequest(){

    CurlRequest *curlRequest;

    curlRequest=malloc(sizeof(CurlRequest));
    curlRequest->method=NULL;
    curlRequest->body=NULL;
    curlRequest->url=NULL;
    curlRequest->deepness=0;
    curlRequest->directSearchedValues=NULL;
    curlRequest->sizeArrSearchedValues=0;

    return curlRequest;

}


CurlRequest *jsonToCurl(JsonElement *jsonElement,wchar_t *urlName){

    CurlRequest *curl;
    Data data;
    JsonElement *jsonCurlElement;
    JsonElement *jsonParameterElement;
    JsonElement *tmpJsonElement;
    unsigned int elementPosition;
    unsigned int parameterPosition;
    wchar_t *tmpWchar;

    curl=initCurlRequest();

    data.strData=urlName;

    if((jsonCurlElement=searchJsonElement(jsonElement,L"curl",NULL,_ARRAY_,&elementPosition))!=NULL){
        if((jsonCurlElement=searchJsonElement(jsonCurlElement,L"name",&data,_STRING_,&elementPosition))!=NULL){

            if((jsonParameterElement=searchJsonElement(jsonElement,L"urlNeededValues",NULL,_ARRAY_,&parameterPosition))){
                curl->url=writeUrl(jsonCurlElement,jsonParameterElement->arrChildElement[parameterPosition]);
            }
            if((tmpJsonElement=searchJsonElement(jsonCurlElement,L"method",NULL,_STRING_,&elementPosition))!=NULL){
                wCharToChar(&tmpJsonElement->arrChildElement[elementPosition]->data->strData,&curl->method,_CONSERV_,_CONSERV_);
            }
            if((tmpJsonElement=searchJsonElement(jsonCurlElement,L"deepness",NULL,_LONG_,&elementPosition))!=NULL){
                curl->deepness=(int)tmpJsonElement->arrChildElement[elementPosition]->data->lngData;
            }
            if((tmpJsonElement=searchJsonElement(jsonCurlElement,L"directSearchedValues",NULL,_ARRAY_,&elementPosition))!=NULL&&tmpJsonElement->arrChildElement[elementPosition]->sizeArrChildElement!=0){
                curl->sizeArrSearchedValues=(int)tmpJsonElement->arrChildElement[elementPosition]->sizeArrChildElement;
                curl->directSearchedValues=malloc(sizeof(char*)*curl->sizeArrSearchedValues);
                for(int i=0;i<curl->sizeArrSearchedValues;i++){
                    wCharToChar(&tmpJsonElement->arrChildElement[elementPosition]->arrChildElement[i]->data->strData,&curl->directSearchedValues[i],_CONSERV_,_CONSERV_);
                }
            }
            if((tmpJsonElement=searchJsonElement(jsonCurlElement,L"body",NULL,_OBJECT_,&elementPosition))!=NULL){
                tmpWchar=malloc(sizeof(wchar_t));
                tmpWchar[0]='\0';
                writeAgreggatedValue(tmpJsonElement->arrChildElement[elementPosition],&tmpWchar,1000);
                wCharToChar(&tmpWchar,&curl->body,_FREE_,_CONSERV_);
            }
        }
    }

    return curl;
}

char *writeUrl(JsonElement *jsonElement,  JsonElement *urlNeededValues){

    wchar_t *wUrl;
    char *cUrl;
    JsonElement *tmpElement;
    unsigned int elementPosition;

    wUrl=malloc(sizeof(wchar_t));
    wUrl[0]='\0';

    for(unsigned long i=0;i<urlNeededValues->sizeArrChildElement;i++){
        if((tmpElement=searchJsonElement(jsonElement,urlNeededValues->arrChildElement[i]->arrChildElement[0]->name,NULL,_ARRAY_,&elementPosition))!=NULL){
            catTokenWithUrl(urlNeededValues->arrChildElement[i]->arrChildElement[0],tmpElement->arrChildElement[elementPosition],&wUrl);
        }
    }
    wCharToChar(&wUrl,&cUrl,_FREE_,_CONSERV_);

    return cUrl;
}

short catTokenWithUrl(JsonElement *urlToken, JsonElement *urlElement,wchar_t **url){

    for(unsigned long i=0;i<urlElement->sizeArrChildElement;i++){
        if(urlElement->arrChildElement[i]->arrChildElement==NULL){
            if(wcslen(urlElement->arrChildElement[i]->data->strData)>0){
                *url=realloc(*url,sizeof(wchar_t)*(wcslen(*url)+wcslen(urlToken->parentElement->arrChildElement[i]->arrChildElement[0]->data->strData)+wcslen(urlElement->arrChildElement[i]->data->strData)+wcslen(urlToken->parentElement->arrChildElement[i]->arrChildElement[1]->data->strData)+1));
                wcscat(*url,urlToken->parentElement->arrChildElement[i]->arrChildElement[0]->data->strData);
                wcscat(*url,urlElement->arrChildElement[i]->data->strData);
                wcscat(*url,urlToken->parentElement->arrChildElement[i]->arrChildElement[1]->data->strData);
            }
        }
        else{
            catTokenWithUrl(urlToken->arrChildElement[0],urlElement->arrChildElement[i],url);
        }
    }
    return 0;
}



short curlConnect(CurlRequest *curlRequest,JsonElement **jsonElement){

    CURL *curl = curl_easy_init();
    struct curl_slist *headers = NULL;
    CURLcode result;
    wchar_t *tmpWChar=NULL;
    unsigned long cursor=0;
    CurlResponse curlResponse;
    curlResponse.size=0;
    curlResponse.data=malloc(sizeof(char));
    curlResponse.data[0]='\0';

    //Requête HTTP méthode GET
    if(curlRequest->method!=NULL){
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, curlRequest->method);
    }
    //URL de la requête
    curl_easy_setopt(curl, CURLOPT_URL, curlRequest->url);
    //Ne vérifie pas l'authenticité du certificat envoyé par le serveur quand la valeur est à 0, vérifie quand la valeur est à 1.
    //J'ai mis la valeur à 0 car CURL stopait la connexion avec l'API openfoodfacts lorsqu'une vérification était effectuée.
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);

    //Création d'un header vide
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlToString);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curlResponse);

    if((result = curl_easy_perform(curl)) != CURLE_OK){
        fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(result));
    }
    if(curlRequest->directSearchedValues!=NULL){
        for(int i = 0; i < curlRequest->sizeArrSearchedValues ; i++){
            if(strpbrk(curlResponse.data,curlRequest->directSearchedValues[i])!=NULL){
                printf("OK CURL FIND");
                return 1;
            }
        }
    }
    charToWchar(&curlResponse.data,&tmpWChar,_FREE_,_CONSERV_);
    *jsonElement=readJsonString(tmpWChar,&cursor,0,NULL,curlRequest->deepness);
    free(tmpWChar);
    tmpWChar=malloc(sizeof(wchar_t));
    tmpWChar[0]='\0';
    writeJsonElement(*jsonElement,&tmpWChar,1000);

    curl_easy_cleanup(curl);
    curl_global_cleanup();


    wprintf(L"CHAR END : %ls",tmpWChar);

    return 0;
}


size_t curlToString(void *ptr, size_t size, size_t nmemb, CurlResponse *curlResponse){


    size_t newSize=curlResponse->size+size*nmemb;
    curlResponse->data=realloc(curlResponse->data,newSize+1);
    memcpy(curlResponse->data+curlResponse->size,ptr,size*nmemb);
    curlResponse->data[newSize]='\0';
    curlResponse->size=newSize;
    return size*nmemb;

}



